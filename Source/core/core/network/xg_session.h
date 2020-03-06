#pragma once
//#include "core/network/connection.h"

namespace server
{
	namespace core
	{
		static const uint32 max_session_count = 300;

		extern struct xg_session_pool xg_session_pool;

		enum class connection_type : uint8
		{
			CONNECTION_TYPE_GAME = 0,
			CONNECTION_TYPE_MAIL = 1,
			MAX_CONNECTION_TYPES,
		};

		struct xg_session
		{
			uint32 id;
			bool marked_for_delete;

			struct tcp_connection* connections[(uint32)connection_type::MAX_CONNECTION_TYPES];

			xg_session(tcp_connection* connection, connection_type type);

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
		};

		struct xg_session_pool : std::vector<xg_session*>
		{
			uint32 session_count;

			xg_session_pool();
			xg_session* spawn_session(tcp_connection* connection, connection_type type);
		};

		void init_user_session_pool(uint32 max_session_count);
	}
}