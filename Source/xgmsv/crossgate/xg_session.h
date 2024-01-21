#pragma once
#include "core/network/connection.h"
#include "core/network/session.h"

namespace crossgate
{
	static const uint32 max_session_count = 300;

	extern struct xg_session_pool xg_session_pool;

	enum class connection_type : uint8
	{
		CONNECTION_TYPE_GAME = 0,
		CONNECTION_TYPE_MAIL = 1,
		MAX_CONNECTION_TYPES,
	};

	struct xg_packet;
	enum xg_opcode;
	struct xg_session : public core::session
	{
		core::tcp_connection* connections[(uint32)connection_type::MAX_CONNECTION_TYPES];

		core::byte_buffer scratch_buffer;

		xg_session(core::tcp_connection* connection, connection_type type);

		xg_packet begin_packet( xg_opcode op );

		/*
		void remove_connection(tcp_connection* connection)
		{
			for (uint32 i = 0; i < (uint32)connection_type::MAX_CONNECTION_TYPES; i++)
			{
				if (connections[i] == connection)
				{
					connections[i] = nullptr;
				}
			}
		}

		bool has_connection()
		{
			for (uint32 i = 0; i < (uint32)connection_type::MAX_CONNECTION_TYPES; i++)
			{
				if (connections[i])
				{
					return true;
				}
			}

			return false;
		}
		*/

		void shutdown_connection()
		{
			signal.fire(core::signal_code::shutdown);
		}
	};

	struct xg_session_pool : std::vector<xg_session*>
	{
		uint32 session_count;

		xg_session_pool();
		xg_session* spawn_session(core::tcp_connection* connection, connection_type type);
	};

	void init_user_session_pool(uint32 max_session_count);
}
