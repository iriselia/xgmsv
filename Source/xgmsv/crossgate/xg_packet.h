#pragma once

//#include "core/network/packet_queue.h"
#include "crossgate/xg_session.h"

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

		struct opcode_entry
		{
			enum xg_opcode opcode;
			const char* opstring;
			uint32 opstring_size;
			void (xg_session::*handler)(xg_packet* packet);
		};

		enum xg_opcode : uint16
		{
			XG_FC,
			XG_ECHO,
			XG_CLIENT_LOGIN,
			XG_CHAR_LIST,
			XG_CHAR_CREATE,
			XG_CHAR_DELETE,
			XG_CHAR_LOGIN,
			XG_CHAR_LOGOUT,

			XG_CHAR_WALK,
			XG_CHAR_INTERACT,

			XG_OPCODE_COUNT
		};

		extern opcode_entry xg_opcode_table[XG_OPCODE_COUNT];

		template<typename T, size_t N>
		int mystrncmp(const T* a, const T(&b)[N])
		{
			return _tcsnccmp(a, b, N - 1);
		}

		void xg_dispatch_packet(xg_session* session, core::packet&& packet);
	}
}
