#include "xg_packet.h"

namespace server
{
	namespace crossgate
	{
#define add_opcode_entry(opcode, opstring, handler) \
		{ opcode, opstring, sizeof(opstring) / sizeof(char) - 1, handler }

		opcode_entry xg_opcode_table[XG_OPCODE_COUNT] =
		{
			add_opcode_entry(XG_FC,						"FC",					&xg_session::handle_fc),
			add_opcode_entry(XG_ECHO,					"Echo",					&xg_session::handle_echo),
			add_opcode_entry(XG_CLIENT_LOGIN,			"ClientLogin",			&xg_session::handle_client_login),
			add_opcode_entry(XG_CHAR_LIST,				"CharList",				&xg_session::handle_char_list),
			add_opcode_entry(XG_CHAR_LOGIN,				"CharLogin",			&xg_session::handle_char_login),
			add_opcode_entry(XG_CHAR_LOGOUT,			"CharLogout",			&xg_session::handle_char_logout),
		};

		void xg_dispatch_packet(xg_session* session, core::packet&& packet)
		{
			xg_packet xg_packet(std::move(packet));

			for (uint32 i = 0; i < XG_OPCODE_COUNT; i++)
			{
				auto opstring = xg_opcode_table[i].opstring;
				auto opstring_size = xg_opcode_table[i].opstring_size;
				if (!strncmp((char*)packet.data, opstring, opstring_size))
				{
					auto handler = xg_opcode_table[i].handler;
					(session->*handler)(&xg_packet);
					return;
				}
			}

			printf("Unhandled packet:%s\n", xg_packet.data);
		}
	}
}

