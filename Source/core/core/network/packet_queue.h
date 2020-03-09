#pragma once

namespace server
{
	namespace core
	{
		struct packet
		{
			uint8* data;
			uint64 length;
			bool raw;
		};


		struct packet_queue
		{
			static const uint32 max_frame_count = 2;

			struct frame_data
			{
				std::vector<uint8> data;
				uint64 wpos;
				uint64 wsize;

				std::vector<packet> packets;
				uint64 rpos;
				uint64 packet_count;

				frame_data() : wpos(0), rpos(0), wsize(0), packet_count(0)
				{
					data.resize(4096);
					packets.resize(128);
				}
			};

			frame_data frame[max_frame_count];

			std::atomic<uint32> state;
			uint32 read_state;
			uint32 write_state;

			enum packet_queue_state_bit
			{
				active_frame = 0,
				write_busy = 1,
				dirty_flag = 2,
				total_bits
			};


			packet_queue() : state(0), read_state(0), write_state(0), frame{}
			{}

			bool enqueue(uint8* packet_data, uint64 packet_size, bool raw = false)
			{
				this->begin_write();

				bool result = false;

				uint32 write_idx = write_state & (0x1 << packet_queue_state_bit::active_frame);
				frame_data& f = frame[write_idx];
				uint64 buffer_space = f.data.size() - f.wpos;
				uint64 packet_slots = f.packets.size() - f.packet_count;

				if (buffer_space >= packet_size && packet_slots >= 1)
				{
					memcpy(&f.data[f.wpos], packet_data, packet_size);
					f.packets[f.packet_count] = { &f.data[f.wpos], packet_size, raw };
					f.wpos += packet_size;
					f.packet_count++;
					result = true;
				}

				this->end_write();
				return result;
			}

			packet* dequeue()
			{
				uint32 read_idx = read_state & (0x1 << packet_queue_state_bit::active_frame);
				frame_data& f = frame[read_idx];

				if (has_next())
				{
					return &f.packets[f.rpos++];
				}

				return nullptr;
			}

			bool requeue(packet* packet)
			{
				uint32 read_idx = read_state & (0x1 << packet_queue_state_bit::active_frame);
				frame_data& f = frame[read_idx];
				f.rpos--;

				return &f.packets[f.rpos] == packet;
			}

			bool has_next()
			{
				uint32 read_idx = read_state & (0x1 << packet_queue_state_bit::active_frame);
				frame_data& f = frame[read_idx];

				return f.rpos < f.packet_count;
			}

			void begin_write()
			{
				write_state = state.fetch_add(0x1 << packet_queue_state_bit::write_busy, std::memory_order_acq_rel);
				assert((write_state & (0x1 << packet_queue_state_bit::write_busy)) == 0);
				state.fetch_or(0x1 << packet_queue_state_bit::dirty_flag, std::memory_order_acq_rel);
			}

			void end_write()
			{
				write_state = state.fetch_sub(0x1 << packet_queue_state_bit::write_busy, std::memory_order_acq_rel);
				assert((write_state & (0x1 << packet_queue_state_bit::write_busy)) != 0);
			}

			bool begin_read()
			{
				if (state.load(std::memory_order_acquire) & (0x1 << packet_queue_state_bit::dirty_flag))
				{

					uint32 write_state = read_state ^ (0x1 << packet_queue_state_bit::active_frame);

					uint32 test_state;
					do
					{
						test_state = read_state | (0x1 << packet_queue_state_bit::dirty_flag);
					} while (!state.compare_exchange_weak(test_state, write_state, std::memory_order_acq_rel));

					return true;
				}

				return false;
			}

			void end_read()
			{
				uint32 read_idx = read_state & (0x1 << packet_queue_state_bit::active_frame);
				frame_data& f = frame[read_idx];

				f.rpos = 0;
				f.wpos = 0;
				f.wsize = 0;
				f.packet_count = 0;
				memset(&f.data[0], 0, sizeof(uint8) * f.data.size());
				memset(&f.packets[0], 0, sizeof(packet) * f.packets.size());

				read_state ^= (0x1 << packet_queue_state_bit::active_frame);
			}
		};
	}
}