
namespace core
{
	struct byte_buffer
	{
		std::vector<uint8> v;
		uint64 capacity;
		//uint64 rpos;
		//uint64 rend;
		uint64 size;

		byte_buffer(uint64 capacity) :
			v(capacity),
			capacity(capacity),
			//rpos(0),
			//rend(0),
			size(0)
		{
		}

		uint64 len()
		{
			return size;
		}

		uint64 space()
		{
			return capacity - size;
		}

		uint8* data()
		{
			return v.data();
		}

		uint8* end()
		{
			return v.data() + size;
		}

		uint8* write(void* source, uint64 size)
		{
			uint8* wptr = this->end();

			if (this->space() > size)
			{
				memcpy(wptr, source, size);
				//rpos = size;
				//rend += size;
				this->size += size;
				return wptr;
			}

			return nullptr;
		}

		void erase(uint64 num_erase)
		{
			if (size >= num_erase)
			{
				memmove(v.data(), v.data() + size, size - num_erase);
				size -= num_erase;
			}
		}
	};
}
