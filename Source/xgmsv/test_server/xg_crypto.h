#pragma once

namespace server
{
	namespace crossgate
	{
#define SEED 4595
		char mapping_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

		// decrypt stage 1
		int util_64to256(char* dst, const char* src, char* table)
		{
			unsigned int dw, dwcounter, i;
			char* ptr = NULL;

			dw = 0;
			dwcounter = 0;
			if (!dst || !src || !table) return 0;
			for (i = 0; i < strlen(src); i++) {
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
		int remove_salt(char* dst, const char* src, size_t length)
		{
			// remove conditional salt
			int seed = SEED % length; //38 when message_size = 49
			int sum = src[seed] & 0x000000ff;
			for (int i = 0; i < length; i++)
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

		void remove_conditional_bit_reverse(char* dst, const char* src, size_t length)
		{
			// remove conditional bit reverse, seed = 4595
			for (int i = 0; i < length; i++)
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
				int result = length;
			}
		}

		std::string decrypt_message_str(std::string packet)
		{
			std::string stage_1(packet.length(), 0);

			// 6 bit to 8 bit string
			int size = util_64to256(stage_1.data(), packet.c_str(), mapping_table);
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

		void decrypt_message(char*& data, uint32& size)
		{
			// 6 bit to 8 bit string
			size = util_64to256(data, data, mapping_table);

			data[strlen(data) - 1] = '\0';

			size = remove_salt(data, data, strlen(data));
			remove_conditional_bit_reverse(data, data, strlen(data));

			data[strlen(data) - 1] = '\0';

			data++;
			size = strlen(data) + 1;


			// important trim right here
			/*
			stage_1 = stage_1.substr(0, size - 1);

			std::string stage_2(stage_1.length(), 0);
			stage_2 = stage_2.substr(0, strlen(stage_2.data()));

			std::string stage_3(stage_2.length(), 0);
			remove_conditional_bit_reverse(stage_3.data(), stage_2.c_str(), stage_2.length());
			stage_3 = stage_3.substr(1, strlen(stage_3.data()) - 1);

			return stage_3;
			*/

		}

		void encrypt_message(char*& data, uint32& size)
		{

		}
	}
}