#pragma once

namespace server
{
	namespace crossgate
	{
		struct xg_packet : public core::packet
		{
			xg_packet(core::packet&& packet) :
				core::packet(packet)
			{}

		};

#define send_raw(str) enqueue_response((uint8*)str, sizeof(str) - 1)

		void handle_echo(xg_packet* packet)
		{
			auto connection = packet->connection;

			if (!connection->send_raw("Echo nr "))
			{
				printf("handle_echo failed.\n");
			}

			printf("handle_echo!\n");
		}

		void handle_fc(xg_packet* packet)
		{
			printf("handle_fc!\n");
		}

		void handle_client_login(xg_packet* packet)
		{
			printf("handle_client_login!\n");
		}

		struct opcode_entry
		{
			enum xg_opcode opcode;
			const char* opstring;
			uint32 opstring_size;
			void (*handler)(xg_packet* packet);
		};

		enum xg_opcode : uint16
		{
			XG_FC,
			XG_ECHO,
			XG_CLIENT_LOGIN,
			XG_OPCODE_COUNT
		};

#define add_opcode_entry(opcode, opstring, handler) \
		{ opcode, opstring, sizeof(opstring) / sizeof(char) - 1, handler }

		opcode_entry xg_opcode_table[XG_OPCODE_COUNT] =
		{
			add_opcode_entry(XG_FC,						"FC",					&handle_fc),
			add_opcode_entry(XG_ECHO,					"Echo",					&handle_echo),
			add_opcode_entry(XG_CLIENT_LOGIN,			"ClientLogin",			&handle_client_login)
		};

		template<typename T, size_t N>
		int mystrncmp(const T* a, const T(&b)[N])
		{
			return _tcsnccmp(a, b, N - 1);
		}

		void xg_dispatch_packet(core::packet&& packet)
		{
			xg_packet xg_packet(std::move(packet));

			for (uint32 i = 0; i < XG_OPCODE_COUNT; i++)
			{
				auto opstring = xg_opcode_table[i].opstring;
				auto opstring_size = xg_opcode_table[i].opstring_size;
				if (!strncmp((char*)packet.data, opstring, opstring_size))
				{
					auto handler = xg_opcode_table[i].handler;
					handler(&xg_packet);
				}
			}
		}
	}
}
