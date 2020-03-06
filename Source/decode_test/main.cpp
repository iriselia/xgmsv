

#include "string.h"
#include "stdio.h"
#include <assert.h>

//#include <ddraw.h>
//IDirectDraw7

#define SEED 4595

char mapping_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

char packet1[] = "0ClientLogin jpark730 abCD1212 jpark730 1321213 ";
char packet2[] = "0 l e t   i   p r   0 a C 1     p r 7   1 2 2 3  ";
char packet3[] = "0 l e t   i   p r   0 a C 1     p r 7   1 2 2 3  ";
char packet4[] = "MLVlll6KdKyJmGKKII5pnmuNyMUp31qWQ7QqzcfGII5pnmuNN8X5wcMqzCuyJMAX2";
char packet5[] = "0 l e t   i   p r   0 a C 1     p r 7   1 2 2 3  ";
char packet6[] = "0 l e t   i   p r   0 a C 1     p r 7   1 2 2 3  ";
char packet7[] = "0 l e t   i   p r   0 a C 1     p r 7   1 2 2 3 ";

char packet2_1[] = "c+4Ce8NuJGQVxVLRgDKvRWEySJC1YMGpwavBv+f9Z6y-4M6ik+AuJ1kGzKSjrNLw+JXlIZKHhLmRbv1zK5VYsLn6JipH0LwVe+42KkEBMLc";
char packet2_2[] = "c+4Ce8NuJGQVxVLRgDKvRWEySJC1YMGpwavBv+f9Z6y-4M6ik+AuJ1kGzKSjrNLw+JXlIZKHhLmRbv";
char packet2_3[] = "c+4Ce8NuJGQVxVLRgDKvRWEySJC1YMGpwavBv+f9Z6y-4M6ik+AuJ1kGzKSjrNLw+JXlIZKHhLmRbv";
char packet2_4[] = "c+4Ce8NuJGQVxVLRgDKvRWEySJC1YMGpwavBv+f9Z6y-4M6ik+AuJ1kGzKSjrNLw+JXlIZKHhLmRbv";
char packet2_5[] = "c+4Ce8NuJGQVxVLRgDKvRWEySJC1YMGpwavBv+f9Z6y-4M6ik+AuJ1kGzKSjrNLw+JXlIZKHhLmRbv                                    ";
char packet2_6[] = R"(CharList 0 jason|0\\z241400\\z1\\z15\\z0\\z0\\z0\\z15\\z0\\z0\\z100\\z0\\z2\\zjason\\z2\\z游民\\z2\\z106002\\z-1| )";
char packet2_7[] = R"(CharList 0 jason|0\\z241400\\z1\\z15\\z0\\z0\\z0\\z15\\z0\\z0\\z100\\z0\\z2\\zjason\\z2\\z游民\\z2\\z106002\\z-1| )";

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

#define MESSAGE_SIZE(a) sizeof(a)/sizeof(char)

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

int		read_counter = 0;
char*	read_buffer = 0;
int		read_length = 0;

int read_bits(int input)
{
	int mod8 = read_counter % 8; // row    0 1 2 3 4 5 6 7
	int div8 = read_counter / 8; // column 0 1 2 3 4 5 6 7 8

	// valid input: 1 2 3 4 5 6 7 8
	if (div8 >= read_length) // || input < 1 || input > 8) unnecessary for now
	{
		return 0;
	}

	int index = input + mod8 * 9;

	int lo = (*(read_buffer + div8)		& weird_kernel1[index]) >> mod8;
	int hi = (*(read_buffer + div8 + 1)	& weird_kernel2[index]) << (8 - mod8);

	read_counter += input;

	return hi | lo;

}

int read_9bits(int input = 9)
{
	int lo = read_bits(8); // row + 1
	int hi = read_bits(1) << 8; // column + 1
	return hi + lo;
}

int		write_counter = 0;
char*	write_buffer = 0;
int		write_length = 0;

int write_bits(int num_bits, char input)
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

}

int write_9bits(int num_bits, int input)
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
}

char* index(char* s, int c)
{
	return strchr(s, c);
}

int util_shr_64to256(char* dst, char* src, char* table, char* key)
{
	unsigned int dw, dwcounter, i, j;
	char* ptr = 0;

	if (!key || (strlen(key) < 1)) return 0;	// must have key

	dw = 0;
	dwcounter = 0;
	j = 0;
	if (!dst || !src || !table) return 0;
	for (i = 0; i < strlen(src); i++) {
		ptr = (char*)index(table, src[i]);
		if (!ptr) return 0;
		if (i % 4) {
			// check!
			dw = ((((unsigned int)(ptr - table) & 0x3f) + key[j]) % 64)
				<< ((4 - (i % 4)) * 2) | dw;
			j++;  if (!key[j]) j = 0;
			dst[dwcounter++] = dw & 0xff;
			dw = dw >> 8;
		}
		else {
			// check!
			dw = (((unsigned int)(ptr - table) & 0x3f) + key[j]) % 64;
			j++;  if (!key[j]) j = 0;
		}
	}
	if (dw) dst[dwcounter++] = dw & 0xff;
	dst[dwcounter] = '\0';
	return dwcounter;
}

int util_64to256(char* dst, char* src, char* table)
{
	unsigned int dw, dwcounter, i;
	char* ptr = NULL;

	dw = 0;
	dwcounter = 0;
	if (!dst || !src || !table) return 0;
	for (i = 0; i < strlen(src); i++) {
		ptr = (char*)index(table, src[i]);
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



int main()
{
	read_buffer = char9_str;
	read_length = sizeof(char9_str);
	read_counter = 0;

	for (int i = 0; i < sizeof(char9_str); i++)
	{
		char out_char = '?';

		int bits = read_9bits();

		if (bits < 0x7F)
		{
			out_char = bits;
		}

		if (i && !(i % 8))
		{
			printf("\n");
		}

		printf("|%c:0x%03x, ", out_char, bits);
	}
	printf("\n");

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

	unsigned char dictionary_indices[512];
	dictionary_entry dictionary_entries[256];
	unsigned char dictionary_size = 0;

	memset(dictionary_indices, 0, 512);
	memset(dictionary_entries, 0, sizeof(dictionary_entry) * 256);
	dictionary_size = 0;

	unsigned curr_char9;
	unsigned next_char9;
	
	int bytes_encoded = 0;

	write_buffer = packet2_5;
	write_length = sizeof(packet2_5) + 1;
	write_counter = 0;
	memset(packet2_5, 0, sizeof(packet2_5));

	printf("Compression:\n");

	curr_char9 = packet2_6[0];
	for (int index = 1; index < (MESSAGE_SIZE(packet2_6) + 1); index++)
	{
		if (index < MESSAGE_SIZE(packet2_6) - 1)
		{
			next_char9 = packet2_6[index] & 0xFF; // 0xFF required because of visual studio bug
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
					if (index != MESSAGE_SIZE(packet2_6) + 2)
					{
						// bytes_encoded = bytes_encoded_old;
						curr_char9 = curr_entry_index + 0x101; // 0x101 is the offset to make extended 9 bit ascii
						goto skip_char;
					}
					break;
				}
				else
				{
					printf("error");
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
			printf("\n");
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
		printf("|%c:0x%03x, ", out_char, curr_char9);
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
		if (index == MESSAGE_SIZE(packet2_6) - 1 + 1)
		{
			break;
		}
	}

	printf("\n");

	//input variables
	read_counter = 0;
	read_buffer = &packet2_6[0];
	read_length = sizeof(packet2_6) / sizeof(char);


	int message_size = sizeof(packet2_2);
	int message_size2 = sizeof(packet2_2) + 1;
	int message_size3 = sizeof(packet2_2) - 1;

	/*
	int result = 0;
	int out_int = 0;
	int counter = 0;
	char sum = 0;


	out_int = message_size2; // 50
	// conditional bit reverse, seed = 4595
	for (int i = 0; i < message_size; i++)
	{
		sum += packet1[i];
		if (SEED % 7 == i % 5 || SEED % 2 == i % 2)
		{
			packet2[i] = ~packet1[i];
		}
		result = message_size;
	}

	// conditional salt
	int seed = SEED % message_size; //38 when message_size = 49
	for (int i = 0; i < message_size2; i++)
	{
		if (i < seed)
		{
			packet3[i] = packet2[i] + sum * (i * i % 3);
		}

		if (i == seed)
		{
			packet3[i] = sum;
		}

		if (i > seed)
		{
			packet3[i] = packet2[i - 1] + sum * (i * i % 7);
		}

		result = out_int;
	}

// 30 b5 65 96 5e 8a 74 ac
// 89 98 62 8a 20 8e 69 9e
// 6b 8d c8 c5 29 df 5a 96
// 43 b4 2a cd c7 c6 20 8e
// 69 9e 6b 8d 37 c5 f9 c1
// c3 2a cc 2b b2 24 c0 17
// d8 ff

	// encryption stage 2
	int i = 0;
	int j = 0;
	while (true)
	{
		int mode = 0;
		char next_byte = 0;
		char next_next_byte = 0;

		if (i < message_size2 - 1) // 50 - 1 = 49
		{
			next_byte = packet3[i + 1];

			if (i < message_size3) // 48
			{
				mode = 4;
				next_next_byte = packet3[i + 2];
			}
			else
			{
				mode = 3;
				next_next_byte = 0;
			}
		}
		else
		{
			next_byte = 0;
		}

		if (mode < 2)
		{
			j = i;
		}
		if (mode >= 2)
		{
			packet4[j++] = mapping_table[(packet3[i] >> 2) & 0b111111];
			packet4[j++] = mapping_table[((next_byte >> 4) & 0b1111) | ((packet3[i] & 0b11) * 0b10000)];
		}
		if (mode >= 3)
		{
			packet4[j++] = mapping_table[((next_next_byte >> 6) & 0b11) | 4 * (next_byte & 0b1111)];
		}
		if (mode >= 4)
		{
			packet4[j++] = mapping_table[next_next_byte & 0x3F];
		}

		i += 3;
		if (i >= message_size2)
		{
			break;
		}
	}

	// MLVlll6K
	// dKyJmGKK
	// II5pnmuN
	// yMUp31qW
	// Q7QqzcfG
	// II5pnmuN
	// N8X5wcMq
	// zCuyJMAX
	// 2P8
	packet4;
*/
	/*
					73 ee 02 7b c3 6e 24 64  s?.{?n$d
0x00007FF6D8C8C258  15 c5 52 d1 80 32 af 45  .?R?�2?E
0x00007FF6D8C8C260  61 32 48 90 b5 60 c1 a9  a2H??`??
0x00007FF6D8C8C268  c1 ab c1 bf e7 fd 67 ac  ??????g?
0x00007FF6D8C8C270  bf e0 ce a2 93 e0 2e 27  ??????.'
0x00007FF6D8C8C278  59 06 cc a4 a3 ac d2 f0  Y.??????
0x00007FF6D8C8C280  f8 95 e5 21 92 87 84 b9  ???!????
0x00007FF6D8C8C288  91 6e fd 73 2b 95 58 b0  ?n?s+?X?
0x00007FF6D8C8C290  b9 fa 26 2a 47 d0 bc 15  ??&*G??.
0x00007FF6D8C8C298  7b ee 36 2a 41 01 30 b7  {?6*A.0?
0x00007FF6D8C8C2A0  00
*/

	// 6 bit to 8 bit string
	util_64to256(packet2_2, packet2_1, mapping_table);

	// remove conditional salt
	int seed = SEED % message_size; //38 when message_size = 49
	int sum = packet2_2[seed] & 0x000000ff;
	for (int i = 0; i < message_size2; i++)
	{
		if (i < seed)
		{
			packet2_3[i] = packet2_2[i] - sum * (i * i % 3);
		}

		if (i == seed)
		{
			//packet3[i] = sum;
		}

		if (i > seed)
		{
			packet2_3[i - 1] = packet2_2[i] - sum * (i * i % 7);
		}
	}

	// remove conditional bit reverse, seed = 4595
	for (int i = 0; i < message_size; i++)
	{
		//sum += packet1[i];
		if (SEED % 7 == i % 5 || SEED % 2 == i % 2)
		{
			packet2_4[i] = ~packet2_3[i];
		}
		else
		{
			packet2_4[i] = packet2_3[i];
		}
		int result = message_size;
	}

	if (packet2_4[0] % 2)
	{
		packet2_4[0] = packet2_4[0];
	}

	// odd length decryption
	printf("Decompression:\n");

	//input variables
	read_counter = 0;
	read_buffer = &packet2_5[0];
	read_length = message_size3; // message_size - 1
	memset(dictionary_indices, 0, 512);
	memset(dictionary_entries, 0, sizeof(dictionary_entry) * 256);
	memset(packet2_7, 0, sizeof(packet2_7));
	dictionary_size = 0;

	next_char9 = 0;
	curr_char9 = 0;// packet2_6[0];
	unsigned char max_dictionary_size = 0;
	unsigned char curr_entry_index = 0;
	char char_stack[512];
	unsigned int char_stack_size = 0;
	memset(char_stack, 0, 512);

	int bytes_decoded = 0;

	for (int index = 0; index < (MESSAGE_SIZE(packet2_5) + 1); index++)
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
				return -1;
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
					printf("error\n");
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
			return -1;
		}

		if (char_stack_size)
		{
			int increment = char_stack_size - 1;

			while (bytes_decoded < MESSAGE_SIZE(packet2_5) && char_stack_size)
			{
				packet2_7[bytes_decoded] = char_stack[char_stack_size - 1];
				bytes_decoded++;
				char_stack_size--;
			}

			if (char_stack_size)
			{
				//error
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

	return 0;
}


/*
// kernel 2
int kernel_2_counter = 0;
static int kernel_2[512];

// kernel 1
int kernel_1_counter = 0;
//char v21[4];				// 0
//int v22;					// 0
//char v23;					// 1
static int v24_[3 + 1023 + 1024];	// 0 1 0 2 0 2 0 3 0 3 0, ...

int* kernel_1_v24 = v24_;
do
{
	*(kernel_1_v24 + 0) = kernel_1_counter;
	*(kernel_1_v24 + 1) = 0;
	*(kernel_1_v24 + 2) = kernel_1_counter + 1;
	*(kernel_1_v24 + 3) = 0;

	kernel_1_v24 += 4;
	kernel_1_counter += 1;
} while (kernel_1_counter < 256);

unsigned char* kernel_1_v21_c4 = (unsigned char*)v24_ + 0;
int* kernel_1_v22_i = v24_ + 1;
kernel_1_v24 = v24_ + 3;

// decryption loop
char* out_buffer = packet2_5;
int buffer_size = sizeof(packet2_5) / sizeof(packet2_5[0]);
int last_char9 = 0;
int* v13_p = 0;
int* v14_p = 0;
char v15 = 0;
int magic_seed = 0;
int char9 = 0;
int char9_old = 0;

int index = 0;

int n257 = 257;
int nn257 = 0;

// kernel 3
int* unused_kernel_3_v25 = v24_ + 3 + 1023 + 0; // takes 2 bytes actually
*unused_kernel_3_v25 = 0;
char* kernel_3_v26p = (char*)(v24_ + 3 + 1023 + 2);
char* unused_kernel_3_v10 = (char*)(v24_ + 3 + 1023 + 2); // = kernel_3_v26
while (true)
{
	char9 = read_9bits(9);
	printf("magic_number_9: %02x\n", char9);
	char9_old = char9;

	if (char9 == 0x100) // 9 bit new line
	{
		return index;
	}

	nn257 = n257;

	if (char9 < n257)              // if ( magic_number(9) < 257 )
	{
		magic_seed = char9;
	}
	else
	{
		kernel_2[kernel_2_counter++] = magic_seed;
		if (kernel_2_counter >= 512)
		{
			return -1;
		}
		magic_seed = last_char9;
	}

	if (magic_seed > 256)
	{
		break;
	}

LABEL_16:
	kernel_2[kernel_2_counter++] = magic_seed;
	if (kernel_2_counter >= 512)
	{
		return -1;
	}

	if (kernel_2_counter)
	{
		v14_p = &kernel_2[kernel_2_counter];
		while (++index <= buffer_size)
		{
			v15 = *((unsigned char*)v14_p - 4);
			--v14_p;
			--kernel_2_counter;
			*out_buffer++ = v15;
			if (!kernel_2_counter)
			{
				nn257 = n257;
				char9 = char9_old;
				goto LABEL_22;
			}
		}
		return -1;
	}
LABEL_22:
	if (index > 1 && nn257 < 512)
	{
		*unused_kernel_3_v10 = magic_seed;
		*((unsigned int*)unused_kernel_3_v10 + 1) = last_char9;
		if (last_char9 >= 512)
		{
			return -1;
		}
		*((unsigned int*)unused_kernel_3_v10 + 2) = kernel_1_v24[4 * last_char9];
		*((unsigned int*)unused_kernel_3_v10 + 3) = 0;
		kernel_1_v24[4 * last_char9] = 257;
		assert((unused_kernel_3_v10 - kernel_3_v26p) < 2048);
		unused_kernel_3_v10 += 16;
		n257 = nn257 + 1;
	}
	last_char9 = char9;
}

v13_p = &kernel_2[kernel_2_counter];
while ((unsigned int)magic_seed < 512)
{
	++kernel_2_counter;
	++v13_p;
	*(v13_p - 1) = (unsigned __int8)kernel_1_v21_c4[16 * magic_seed];
	magic_seed = *(kernel_1_v22_i + (size_t)magic_seed * 4);
	if (kernel_2_counter >= 512)
		break;
	if (magic_seed <= 256)
		goto LABEL_16;
}
*/