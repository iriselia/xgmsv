#pragma once

namespace server
{
	namespace crossgate
	{
#define SEED 4595
		char mapping_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

		// decrypt stage 1
		int util_64to256(char* dst, const char* src, char* table, uint64 length)
		{
			unsigned int dw, dwcounter, i;
			char* ptr = NULL;

			dw = 0;
			dwcounter = 0;
			if (!dst || !src || !table) return 0;
			for (i = 0; i < length; i++) {
				ptr = (char*)strchr(table, src[i]);
				if (!ptr) return 0;
				if (i % 4) {
					dw = ((unsigned int)(ptr - table) & 0x3f) << (32 - 6 - ((4 - (i % 4)) * 2)) | dw;
					dst[dwcounter++] = (dw & 0xff000000) >> (32 - 8);
					dw = dw << 8;
				}
				else {
					dw = ((unsigned int)(ptr - table) & 0x3f) << (32 - 6);
				}
			}
			if (dw) dst[dwcounter++] = dw & 0xff;
			dst[dwcounter] = '\0';
			return dwcounter;
		}

		// decrypt stage 2
		uint64 remove_salt(char* dst, const char* src, uint64 length)
		{
			// remove conditional salt
			int seed = SEED % length; //38 when message_size = 49
			int sum = src[seed] & 0x000000ff;
			for (int i = 0; i < (length + 1); i++) // +1 for checksum
			{
				if (i < seed)
				{
					dst[i] = src[i] - sum * (i * i % 3);
				}

				if (i == seed)
				{
					//packet3[i] = sum;
				}

				if (i > seed)
				{
					dst[i - 1] = src[i] - sum * (i * i % 7);
				}
			}

			return length - 1;
		}

		void remove_conditional_bit_reverse(char* dst, const char* src, uint64 length)
		{
			// remove conditional bit reverse, seed = 4595
			for (int i = 0; i < (length + 1); i++) // +1 for checksum
			{
				//sum += packet1[i];
				if (SEED % 7 == i % 5 || SEED % 2 == i % 2)
				{
					dst[i] = ~src[i];
				}
				else
				{
					dst[i] = src[i];
				}
				uint64 result = length;
			}
		}

		std::string decrypt_message_str(std::string packet)
		{
			std::string stage_1(packet.length(), 0);

			// 6 bit to 8 bit string
			int size = util_64to256(stage_1.data(), packet.c_str(), mapping_table, packet.length());
			// important trim right here
			stage_1 = stage_1.substr(0, size - 1);

			std::string stage_2(stage_1.length(), 0);
			size = remove_salt(stage_2.data(), stage_1.c_str(), stage_1.length());
			stage_2 = stage_2.substr(0, strlen(stage_2.data()));

			std::string stage_3(stage_2.length(), 0);
			remove_conditional_bit_reverse(stage_3.data(), stage_2.c_str(), stage_2.length());
			stage_3 = stage_3.substr(1, strlen(stage_3.data()) - 1);


			return stage_3;
		}

		void decrypt_message(core::byte_buffer& payload)
		{
			char* buffer = (char*)payload.data() + payload.rpos;

			uint64 payload_size = payload.rend - payload.rpos;

			// 6 bit to 8 bit string
			uint64 descrypted_size = util_64to256(buffer, buffer, mapping_table, payload_size);

			descrypted_size--; // -1 for checksum

			descrypted_size = remove_salt(buffer, buffer, descrypted_size);
			remove_conditional_bit_reverse(buffer, buffer, descrypted_size);

			buffer[descrypted_size] = '\0';

			buffer++;
			payload.rpos++;
			payload.rend += descrypted_size - payload_size;
		}

		uint8 apply_conditional_bit_reverse(char* dst, const char* src, uint64 length)
		{
			char sum = 0;

			// conditional bit reverse, seed = 4595
			for (int i = 0; i < length; i++)
			{
				sum += src[i];
				if (SEED % 7 == i % 5 || SEED % 2 == i % 2)
				{
					dst[i] = ~src[i];
				}
				uint64 result = length;
			}

			return sum;
		}

		int apply_salt_and_add_checksum(char* dst, const char* src, uint64 length, uint8 checksum)
		{
			// conditional salt
			int seed = SEED % length; //38 when message_size = 49
			for (int i = 0; i < (length + 1); i++) // alloc for checksum
			{
				if (i < seed)
				{
					dst[i] = src[i] + checksum * (i * i % 3);
				}

				if (i == seed)
				{
					dst[i] = checksum;
				}

				if (i > seed)
				{
					dst[i] = src[i - 1] + checksum * (i * i % 7);
				}

				//uint64 result = out_int;
			}

			return 0;
		}

		int util_256to64(char *dst, char *src, char* table, uint64 length)
		{
			uint32 byte_counter, i;

			uint16 reg;

			if (!dst || !src || !table) return 0;
			reg = 0;
			byte_counter = 0;
			for (i = 0; i < length; i++) {

				reg = (src[i] << (8 - 2 * (i % 3))) | reg;
				dst[byte_counter++] = table[(reg & 0b1111110000000000) >> (8 + 2)];
				reg = (reg << 6);

				if (i % 3 == 2) {
					dst[byte_counter++] = table[(reg & 0b1111110000000000) >> (8 + 2)];
					reg = (reg << 6);
				}
			}
			if (reg) dst[byte_counter++] = table[(reg & 0b1111110000000000) >> (8 + 2)];
			dst[byte_counter] = '\0';
			return byte_counter;
		}


		void encrypt_message(core::byte_buffer& payload)
		{
			char* buffer = (char*)payload.data() + payload.rpos;
			const uint64 payload_size = payload.rend - payload.rpos;

			uint64 encrypted_size = payload_size + 1; // packet_length padding
			char header = (char)encrypted_size;

			if (encrypted_size >= 100)
			{
				if (encrypted_size % 2 == 0)
				{
					header = encrypted_size;
				}

				assert(false);
				/*
				*(_BYTE*)message_work_buffer2 = checksum;
				message_length = odd_length_encrypt(
					(char*)message_work_buffer2 + 1,
					3 * buffer_size - 1,
					(unsigned __int8*)message,
					strlen(message));
					*/
			}
			else
			{
				if (encrypted_size % 2 == 1)
				{
					header = encrypted_size + 1;
				}


				memmove(buffer + 1, buffer, encrypted_size);
			}

			buffer[0] = header;

			encrypted_size++; // null terminator;

			char workbuf[4096];
			uint8 checksum = apply_conditional_bit_reverse(buffer, buffer, encrypted_size);
			apply_salt_and_add_checksum(workbuf, buffer, encrypted_size, checksum);

			encrypted_size++; // add checksum;
			encrypted_size = util_256to64(buffer, workbuf, mapping_table, encrypted_size);
			buffer[encrypted_size++] = '\n';
			buffer[encrypted_size]   = '\0';

			payload.rend += encrypted_size - payload_size;
			payload.wpos = payload.rend;
		}
	}
}