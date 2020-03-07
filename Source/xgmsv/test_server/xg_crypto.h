#pragma once

namespace server
{
	namespace crossgate
	{
#define SEED 4595
#define MESSAGE_SIZE(a) sizeof(a)/sizeof(char)

		char mapping_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";


		char char9_str[] = {
			0x43, 0xd0, 0x84, 0x91, 0xc3, 0x24, 0xcd, 0x1c,
			0x3a, 0x20, 0x60, 0x80, 0x50, 0x13, 0x66, 0xce,
			0x1b, 0x37, 0x7c, 0x60, 0x70, 0xe1, 0xa2, 0x47,
			0x06, 0x8d, 0x18, 0x34, 0x60, 0x48, 0xa4, 0x18,
			0x63, 0xa2, 0x9e, 0x18, 0x35, 0x3c, 0x6e, 0xd4,
			0x33, 0x72, 0x24, 0x48, 0x91, 0x28, 0x39, 0x6a,
			0x4c, 0x59, 0xd1, 0x23, 0x43, 0x87, 0x6e, 0x3c,
			0xca, 0xf0, 0x38, 0xcd, 0xd9, 0xb0, 0x78, 0x32,
			0x3d, 0xc6, 0x80, 0x61, 0x43, 0xe3, 0x4c, 0x8a,
			0x2d, 0x62, 0xf0, 0x01, 0x01, 0x10, 0x00
		};


		int weird_kernel1[] =
		{
			// 0     1     2     3     4     5     6     7     8
			0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, // 0
			0x00, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, // 1
			0x00, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, // 2
			0x00, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, // 3
			0x00, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 4
			0x00, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 5
			0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 6
			0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  // 7
		};

		int weird_kernel2[] =
		{
			// 0     1     2     3     4     5     6     7     8
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, // 1
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, // 2
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, // 3
			0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, // 4
			0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, // 5
			0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, // 6
			0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F  // 7
		};

		// odd length encryption
		struct dictionary_entry
		{
			unsigned char9_left : 9;
			unsigned : 7;
			unsigned char9_right : 9;
			unsigned : 7;

			unsigned prev : 9;
			unsigned : 23;
		};


		// decrypt stage 1
		int util_64to256(uint8* dst, const uint8* src, char* table, uint64 length)
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
		uint64 remove_salt(uint8* dst, const uint8* src, uint64 length)
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

		// decrypt stage 3
		void remove_conditional_bit_reverse(uint8* dst, const uint8* src, uint64 length)
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

		// decrypt stage 4
		uint64 remove_conditional_compression(uint8* dst, uint64 dst_size, const uint8* src, uint64 length)
		{
			int read_counter = 0;
			const uint8* read_buffer = 0;
			int read_length = 0;

			auto read_bits = [&read_counter, &read_buffer, &read_length](int input) -> int
			{
				int mod8 = read_counter % 8; // row    0 1 2 3 4 5 6 7
				int div8 = read_counter / 8; // column 0 1 2 3 4 5 6 7 8

				// valid input: 1 2 3 4 5 6 7 8
				if (div8 >= read_length) // || input < 1 || input > 8) unnecessary for now
				{
					return 0;
				}

				int index = input + mod8 * 9;

				int lo = (*(read_buffer + div8) & weird_kernel1[index]) >> mod8;
				int hi = (*(read_buffer + div8 + 1) & weird_kernel2[index]) << (8 - mod8);

				read_counter += input;

				return hi | lo;

			};

			auto read_9bits = [&read_bits](int input = 9) -> int
			{
				int lo = read_bits(8); // row + 1
				int hi = read_bits(1) << 8; // column + 1
				return hi + lo;
			};


			//input variables
			read_counter = 0;
			read_buffer = src;
			read_length = length; // message_size - 1

			uint8 dictionary_indices[512];
			dictionary_entry dictionary_entries[256];
			uint8 dictionary_size = 0;

			memset(dictionary_indices, 0, 512);
			memset(dictionary_entries, 0, sizeof(dictionary_entry) * 256);
			memset(dst, 0, dst_size);
			dictionary_size = 0;

			uint16 next_char9 = 0;
			uint16 curr_char9 = 0;// packet2_6[0];
			unsigned char max_dictionary_size = 0;
			unsigned char curr_entry_index = 0;
			char char_stack[512];
			unsigned int char_stack_size = 0;
			memset(char_stack, 0, 512);

			int bytes_decoded = 0;

			for (int index = 0; index < length; index++)
			{
				int bits_read = read_9bits(9);
				int last_char_or_something = bits_read;

				if (bits_read == 0x100)
				{
					break; // EOF for extended ascii
				}

				//dictionary_size = max_dictionary_size;

				if (bits_read < (dictionary_size + 1 + 0x100))
				{
					next_char9 = bits_read;
				}
				else
				{
					char_stack[char_stack_size++] = next_char9;
					if (char_stack_size >= 512)
					{
						assert(false);
					}
					next_char9 = curr_char9;
				}

				if (next_char9 > 0x100)                 // if extended ascii
				{
					while ((unsigned int)next_char9 < 512)
					{
						const dictionary_entry& curr_entry = dictionary_entries[next_char9 - 0x101];

						char_stack[char_stack_size++] = curr_entry.char9_right;
						next_char9 = curr_entry.char9_left;
						if (char_stack_size >= 512)
						{
							break;
							printf("error\n");
							printf("so far:%s", dst);
							assert(false);
						}
						if (next_char9 <= 256)
						{
							break;
						}
					}
				}

				char_stack[char_stack_size++] = next_char9;
				if (char_stack_size >= 512)
				{
					assert(false);
					//return -1;
				}

				if (char_stack_size)
				{
					int increment = char_stack_size - 1;

					while (bytes_decoded < dst_size && char_stack_size)
					{
						dst[bytes_decoded] = char_stack[char_stack_size - 1];
						bytes_decoded++;
						char_stack_size--;
					}

					if (char_stack_size)
					{
						//error
						assert(false);
						break;
					}

					//dictionary_size = max_dictionary_size;
					bits_read = last_char_or_something;
				}

				if (bytes_decoded > 1 && dictionary_size < 256)
				{
					auto& new_entry = dictionary_entries[dictionary_size];
					new_entry.char9_left = curr_char9;
					new_entry.char9_right = next_char9;
					new_entry.prev = dictionary_size;

					bits_read = last_char_or_something;
					dictionary_indices[curr_char9] = dictionary_size++;
				}
				curr_char9 = bits_read;
			}

			return bytes_decoded;
		}

		void decrypt_message(core::byte_buffer& payload)
		{
			uint8* buffer = payload.data() + payload.rpos;

			uint64 payload_size = payload.rend - payload.rpos;

			// 6 bit to 8 bit string
			uint64 decrypted_size = util_64to256(buffer, buffer, mapping_table, payload_size);

			decrypted_size--; // -1 for checksum

			decrypted_size = remove_salt(buffer, buffer, decrypted_size);
			remove_conditional_bit_reverse(buffer, buffer, decrypted_size);

			uint8 workbuf[4096];
			if (buffer[0] % 2)
			{
				decrypted_size = remove_conditional_compression(workbuf, sizeof(workbuf), buffer + 1, decrypted_size);
				memcpy(buffer, workbuf, decrypted_size);
			}

			buffer[decrypted_size] = '\0';

			payload.rpos++;
			payload.rend += decrypted_size - payload_size;
		}

		uint64 apply_conditional_compression(uint8* dst, uint64 dst_size, const uint8* src, uint64 length)
		{
			int		write_counter = 0;
			uint8* write_buffer = 0;
			int		write_length = 0;

			auto write_bits = [&write_counter, &write_buffer, &write_length](int num_bits, char input) -> int
			{
				int mod8 = write_counter % 8; // row    0 1 2 3 4 5 6 7
				int div8 = write_counter / 8; // column 0 1 2 3 4 5 6 7 8
				int div8_plus_1 = div8 + 1;
				// valid input: 1 2 3 4 5 6 7 8
				if (write_length < div8) // || input < 1 || input > 8) unnecessary for now
				{
					return 0;
				}

				int index = num_bits + mod8 * 9;

				*(write_buffer + div8) &= weird_kernel1[index];
				*(write_buffer + div8) |= weird_kernel1[index] & (input << mod8);
				*(write_buffer + div8 + 1) &= weird_kernel2[index];
				*(write_buffer + div8 + 1) |= weird_kernel2[index] & (input >> (8 - mod8));

				write_counter += num_bits;

				return div8 + 1;

			};

			auto write_9bits = [&write_bits](int num_bits, int input) -> int
			{
				int result = 0;
				if (write_bits(8, (char)(input & 0xFF)) >= 0) // encode_magic_number(8, bits) -> encode_magic_number(1, bits)
				{
					result = write_bits(1, (char)((input >> 8) & 0x1));
					if (result < 0)
					{
						result = -1;
					}
				}

				return result;
			};

			unsigned char dictionary_indices[512];
			dictionary_entry dictionary_entries[256];
			unsigned char dictionary_size = 0;

			memset(dictionary_indices, 0, 512);
			memset(dictionary_entries, 0, sizeof(dictionary_entry) * 256);
			dictionary_size = 0;

			unsigned curr_char9;
			unsigned next_char9;

			int bytes_encoded = 0;

			write_buffer = dst;
			write_length = length;
			write_counter = 0;
			memset(dst, 0, dst_size);

			//printf("Compression:\n");

			curr_char9 = src[0];
			for (int index = 1; index < length + 1; index++)
			{
				if (index < length - 1)
				{
					next_char9 = src[index] & 0xFF; // 0xFF required because of visual studio bug
				}
				else
				{
					next_char9 = 0x100; // EOF in 9-bit extended ascii
				}

				// Is there a dictionary entry?
				unsigned char head_entry_index = dictionary_indices[curr_char9];
				dictionary_entry* head_entry = &dictionary_entries[head_entry_index];


				unsigned char curr_entry_index = head_entry_index;

				while (curr_entry_index)
				{
					const dictionary_entry* curr_entry = &dictionary_entries[curr_entry_index];

					if (curr_entry->char9_right == next_char9)
					{
						if (curr_entry_index > 0)
						{
							if (index != length + 1)
							{
								// bytes_encoded = bytes_encoded_old;
								curr_char9 = curr_entry_index + 0x101; // 0x101 is the offset to make extended 9 bit ascii
								goto skip_char;
							}
							break;
						}
						else
						{
							assert(false);
							//printf("error");
						}
					}

					curr_entry_index = curr_entry->prev;
				}
				char out_char = '?';

				if (curr_char9 < 0x7F)
				{
					out_char = curr_char9;
				}

				if ((index - 1) && !((index - 1) % 8))
				{
					//printf("\n");
				}

				/*
				char char9_str[] = {
					43, d0, 84, 91, c3, 24, cd, 1c,
					3a, 20, 60, 80, 50, 13, 66, ce,
					1b, 37, 7c, 60, 70, e1, a2, 47,
					06, 8d, 18, 34, 60, 48, a4, 18,
					63, a2, 9e, 18, 35, 3c, 6e, d4,
					33, 72, 24, 48, 91, 28, 39, 6a,
					4c, 59, d1, 23, 43, 87, 6e, 3c,
					ca, f0, 38, cd, d9, b0, 78, 32,
					3d, c6, 80, 61, 43, e3, 4c, 8a,
					2d, 62, f0, 01, 01, 10, 00
				};
				*/
				//printf("|%c:0x%03x, ", out_char, curr_char9);
				bytes_encoded = write_9bits(9, curr_char9);
				//bytes_encoded_old = bytes_encoded;

				//if (bytes_encoded > buffer_size)
				//	return -1;

				if (dictionary_size < 256)
				{
					auto& new_entry = dictionary_entries[dictionary_size];
					new_entry.char9_left = curr_char9;
					new_entry.char9_right = next_char9;
					new_entry.prev = head_entry_index;

					dictionary_indices[curr_char9] = dictionary_size++;
				}

				curr_char9 = next_char9;
			skip_char:
				if (index == length + 1)
				{
					break;
				}
			}

			return bytes_encoded;
		}

		uint8 apply_conditional_bit_reverse(uint8* dst, const uint8* src, uint64 length)
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

		int apply_salt_and_add_checksum(uint8* dst, const uint8* src, uint64 length, uint8 checksum)
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

		int util_256to64(uint8* dst, uint8* src, char* table, uint64 length)
		{
			uint32 byte_counter;

			uint16 reg, i;

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
			uint8* buffer = payload.data() + payload.rpos;
			const uint64 payload_size = payload.rend - payload.rpos;

			uint64 encrypted_size = payload_size + 1; // packet_length padding
			char header = (char)encrypted_size;

			if (encrypted_size >= 100)
			{
				if (encrypted_size % 2 == 0)
				{
					header = encrypted_size;
				}

				/*
				*(_BYTE*)message_work_buffer2 = checksum;
				message_length = odd_length_encrypt(
					(char*)message_work_buffer2 + 1,
					3 * buffer_size - 1,
					(unsigned __int8*)message,
					strlen(message));
					*/
				uint8 workbuf[4096];
				encrypted_size = apply_conditional_compression(workbuf + 1, sizeof(workbuf) - 1, buffer, encrypted_size);

				/*
char char9_str[] = {
	43, d0, 84, 91, c3, 24, cd, 1c,
	3a, 20, 60, 80, 50, 13, 66, ce,
	1b, 37, 7c, 60, 70, e1, a2, 47,
	06, 8d, 18, 34, 60, 48, a4, 18,
	63, a2, 9e, 18, 35, 3c, 6e, d4,
	33, 72, 24, 48, 91, 28, 39, 6a,
	4c, 59, d1, 23, 43, 87, 6e, 3c,
	ca, f0, 38, cd, d9, b0, 78, 32,
	3d, c6, 80, 61, 43, e3, 4c, 8a,
	2d, 62, f0, 01, 01, 10, 00
};
*/
				//workbuf[0] = header;
				memcpy(buffer, workbuf, encrypted_size + 1);
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

			uint8 workbuf[4096];
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