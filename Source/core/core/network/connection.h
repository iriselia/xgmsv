#pragma once

#include "asio/ip/tcp.hpp"
#include "asio/read.hpp"

#include "core/network/timer.h"

namespace server
{
	namespace core
	{
		struct packet
		{
			struct tcp_connection* connection;
			uint8* data;
			uint64 length;
		};

		static const uint32 max_frame_count = 2;

		struct packet_queue
		{
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
					data.resize(1024);
					packets.resize(128);
				}
			};

			frame_data frame[max_frame_count];

			struct tcp_connection* connection;

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


			packet_queue(struct tcp_connection* connection) :
				connection(connection), state(0), read_state(0), write_state(0), frame{}
			{}

			bool enqueue(uint8* packet_data, uint64 packet_size)
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
					f.packets[f.packet_count] = { connection, &f.data[f.wpos], packet_size };
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
				write_state = state.fetch_add(0x1 << packet_queue_state_bit::write_busy, std::memory_order_relaxed);
				assert((write_state & (0x1 << packet_queue_state_bit::write_busy)) == 0);
				state.fetch_or(0x1 << packet_queue_state_bit::dirty_flag, std::memory_order_relaxed);
			}

			void end_write()
			{
				write_state = state.fetch_sub(0x1 << packet_queue_state_bit::write_busy, std::memory_order_relaxed);
				assert((write_state & (0x1 << packet_queue_state_bit::write_busy)) != 0);
			}

			bool begin_read()
			{
				if (state.load() & (0x1 << packet_queue_state_bit::dirty_flag))
				{

					uint32 write_state = read_state ^ (0x1 << packet_queue_state_bit::active_frame);

					uint32 test_state;
					do 
					{
						test_state = read_state | (0x1 << packet_queue_state_bit::dirty_flag);
					} while (!state.compare_exchange_weak(test_state, write_state));

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

		struct byte_buffer
		{
			std::vector<uint8> buffer;
			uint64 capacity;
			uint64 rpos;
			uint64 rend;
			uint64 wpos;

			byte_buffer(uint64 capacity) :
				buffer(capacity),
				capacity(capacity),
				rpos(0),
				rend(0),
				wpos(0)
			{}

			uint64 size()
			{
				return wpos;
			}

			uint64 free_space()
			{
				return capacity - wpos;
			}

			uint8* data()
			{
				return buffer.data();
			}

			uint8* write(void* source, uint64 size)
			{
				uint8* wptr = buffer.data() + wpos;

				if (this->free_space() > size)
				{
					memcpy(wptr, source, size);
					rpos = wpos;
					wpos += size;
					return wptr;
				}

				return nullptr;
			}

			void erase(uint64 num_erase)
			{
				if (wpos >= num_erase)
				{
					memmove(buffer.data(), buffer.data() + wpos, wpos - num_erase);
					wpos -= num_erase;
				}
			}
		};

		struct tcp_connection
		{
			int id;
			asio::ip::address address;
			uint16 port;
			bool marked_for_delete;

			asio::ip::tcp::socket socket;
			byte_buffer rbuffer;
			byte_buffer wbuffer;

			struct xg_session* parent;

			packet_queue rqueue;
			packet_queue wqueue;
			async_signal wsignal;


			tcp_connection(asio::ip::tcp::socket&& new_socket);

			void shutdown_and_close();

			bool enqueue_response(uint8* packet_data, uint32 packet_size)
			{
				if (!wqueue.enqueue(packet_data, packet_size))
				{
					printf("enqueue failed.\n");
					return false;
				}

				wsignal.fire();

				return true;
			}

			//template<typename buffer_t>
			//auto async_recv(tcp_connection& connection, buffer_t buffer);

			template<typename lambda>
			std::future<void> async_recv_loop(lambda& receive_handler);

			template<typename lambda>
			std::future<void> async_send_loop(lambda& send_handler);
		};


		struct connection_pool : public std::vector<tcp_connection*>
		{
			std::string ip;
			unsigned short port;
			asio::io_context& io_context;
			uint32 conneciton_count = 0;

			connection_pool(asio::io_context& io_context, std::string ip, uint16 port, uint16 max_connection_count);

			tcp_connection* spawn_connection(asio::ip::tcp::socket&& socket)
			{
				for (int id = 0; id < this->size(); id++)
				{
					auto& connection = this->at(id);
					if (!connection)
					{
						this->conneciton_count++;
						connection = new tcp_connection(std::move(socket));
						connection->id = id;
						return connection;
					}
				}

				return nullptr;
			}
		};

		template<typename lambda>
		std::future<void> tcp_connection::async_send_loop(lambda& send_handler)
		{
			try
			{
				while (socket.is_open())
				{
					packet_queue& packets = wqueue;

					if (packets.begin_read())
					{
						while (packets.has_next())
						{
							core::packet* packet = packets.dequeue();

							if (wbuffer.free_space() < packet->length * 2)
							{
								assert(false);
							}
							uint8* packet_data = wbuffer.write(packet->data, packet->length);

							wbuffer.rpos = packet_data - wbuffer.data();
							wbuffer.rend = wbuffer.rpos + packet->length;

							//crossgate::xg_dispatch_packet(std::move(*packet));
							send_handler(wbuffer);

							//std::string packet_str((char*)packet->data, packet->length);

							//packet_str = crossgate::decrypt_message(packet_str);
							//printf("new packet:%s\n", packet_str.c_str());
							//xg_dispatch_packet()
						}
						packets.end_read();
					}

					if (wbuffer.size())
					{
						uint32 bytes_transferred = co_await async_send(*this, asio::buffer(wbuffer.data(), wbuffer.size()));

						wbuffer.erase(bytes_transferred);
					}
					else
					{
						co_await wsignal;
					}
				}
			}
			catch (const std::exception& exception)
			{
				//shutdown_and_close();
				
				//printf("connection %d marked for delete.\n", id);
				printf("exception: %s", exception.what());
			}
		}

		template<typename connection_t, typename buffer_t>
		struct async_send_frame
		{
			connection_t& connection;
			buffer_t buffer;
			asio::error_code error_code;
			uint32 return_value;

			async_send_frame(connection_t& connection, buffer_t&& buffer) :
				connection(connection), buffer(std::move(buffer)), error_code(), return_value(0) {}

			bool await_ready()
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				connection.socket.async_send(buffer, [this, coro](auto error_code, size_t bytes_sent)
				{
					this->error_code = error_code;
					printf("%zd bytes sent to connection %d\n", bytes_sent, connection.id);
					return_value = (uint32)bytes_sent;
					coro.resume();
				});
			}

			uint32 await_resume()
			{
				if (error_code)
				{
					throw asio::system_error(error_code);
				}

				return return_value;
			}
		};

		template<typename lambda>
		std::future<void> tcp_connection::async_recv_loop(lambda& receive_handler)
		{
			try
			{
				while (socket.is_open())
				{
					uint8* rdata = rbuffer.data();
					uint32 bytes_read = co_await async_recv(*this, asio::buffer(rdata, 1024));

					uint32 begin = 0;
					uint32 end = 0;
					uint32 index = 0;
					while (index != bytes_read)
					{
						if (rdata[index] == '\n')
						{
							rdata[index] = '\0';

							end = index;

							uint8* packet_data = rdata + begin;
							uint32 packet_size = end - begin;
							rbuffer.rpos = begin;
							rbuffer.rend = end;

							if (*(rdata + begin) == '\0')
							{
								printf("empty string\n");
								continue;
							}
							else
							{
								// receive assumes decrypted packet is <= encrypted packet.

								receive_handler(rbuffer);

								if (!rqueue.enqueue(rbuffer.data() + rbuffer.rpos, rbuffer.rend - rbuffer.rpos))
								{
									printf("enqueue failed.\n");
								}
							}
							begin = index + 1;
						}

						index++;
					}

					memset(rdata, 0, bytes_read);
				}
			}
			catch (const std::exception& /*exception*/)
			{
				shutdown_and_close();
				//printf("connection %d marked for delete.\n", id);
				//printf("exception: %s", exception.what());
			}
		}


		template<typename buffer_t>
		auto async_send(tcp_connection& connection, buffer_t buffer)
		{
			return async_send_frame<tcp_connection, buffer_t>(connection, std::forward<buffer_t>(buffer));
		}


		template<typename connection_t, typename buffer_t>
		struct async_recv_frame
		{
			connection_t& connection;
			buffer_t buffer;
			asio::error_code error_code;
			uint32 return_value;

			async_recv_frame(connection_t& connection, buffer_t&& buffer) :
				connection(connection), buffer(std::move(buffer)), error_code(), return_value(0) {}

			bool await_ready()
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				connection.socket.async_receive(buffer, [this, coro](auto error_code, size_t bytes_read)
				{
					this->error_code = error_code;
					printf("%zd bytes read from connection %d\n", bytes_read, connection.id);
					return_value = (uint32)bytes_read;
					coro.resume();
				});
			}

			uint32 await_resume()
			{
				if (error_code)
				{
					throw asio::system_error(error_code);
				}

				return return_value;
			}
		};

		template<typename buffer_t>
		auto async_recv(tcp_connection& connection, buffer_t buffer)
		{
			return async_recv_frame<tcp_connection, buffer_t>(connection, std::forward<buffer_t>(buffer));
		}

		template<typename lambda>
		struct async_accept_frame
		{
			async_accept_frame(asio::ip::tcp::acceptor& acceptor, connection_pool& connections, std::string ip, unsigned short port, lambda& accept_handler) :
				ip(ip), port(port), accept_handler(accept_handler), acceptor(acceptor), connection_pool(connections), connection_count(connection_pool.conneciton_count), return_value(nullptr) {}

			std::string ip;
			unsigned short port;
			lambda accept_handler;
			asio::error_code error_code;

			asio::ip::tcp::acceptor& acceptor;
			uint32& connection_count;

			tcp_connection* return_value;
			//asio::ip::tcp::socket socket;
			connection_pool& connection_pool;

			bool await_ready()
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				acceptor.async_accept([this, coro](auto error_code, auto socket)
				{
					this->error_code = error_code;

					return_value = this->connection_pool.spawn_connection(std::move(socket));

					if (return_value == nullptr)
					{
						socket.shutdown(asio::socket_base::shutdown_both);
						socket.close();
					}

					accept_handler(return_value);

					coro.resume();
				});
			}

			tcp_connection* await_resume()
			{
				if (error_code)
				{
					throw asio::system_error(error_code);
				}

				return return_value;
			}

		};

		template<typename lambda>
		auto async_accept(asio::ip::tcp::acceptor& acceptor, connection_pool& connections, std::string ip, unsigned short port, lambda& accept_handler)
		{
			return async_accept_frame<lambda>(acceptor, connections, ip, port, accept_handler);
		}

	}
}