
namespace server
{
	namespace core
	{
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
			{
			}

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
					rend += size;
					wpos = rend;
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
	}
}