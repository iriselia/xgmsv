#pragma once
#include "core/network/connection.h"
#include "core/network/session.h"

namespace server
{
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
		struct xg_session : public core::session
		{

			core::tcp_connection* connections[(uint32)connection_type::MAX_CONNECTION_TYPES];

			xg_session(core::tcp_connection* connection, connection_type type);

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

			bool enqueue_response(uint8* packet_data, uint32 packet_size, bool raw = false)
			{
				if (!wqueue.enqueue(packet_data, packet_size, raw))
				{
					assert(false);
					printf("enqueue failed.\n");
					return false;
				}

				signal.fire();

				return true;
			}

#define send_msg(str) enqueue_response((uint8*)str, sizeof(str))
#define send_raw(str) enqueue_response((uint8*)str "\n", sizeof(str), true)

			void handle_echo(xg_packet* packet);

			void handle_fc(xg_packet* packet);

			void handle_client_login(xg_packet* packet);

			void handle_char_list(xg_packet* packet);

			void handle_char_login(xg_packet* packet);

			void handle_char_logout(xg_packet* packet);
		};

		struct xg_session_pool : std::vector<xg_session*>
		{
			uint32 session_count;

			xg_session_pool();
			xg_session* spawn_session(core::tcp_connection* connection, connection_type type);
		};

		void init_user_session_pool(uint32 max_session_count);
	}
}