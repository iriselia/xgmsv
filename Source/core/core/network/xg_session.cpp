#pragma once

#include "core/shared.h"

#include "core/network/xg_session.h"
#include "core/network/connection.h"

#include "core/network/timer.h"

namespace server
{
	namespace core
	{
		struct xg_session_pool xg_session_pool;

		xg_session::xg_session(tcp_connection* connection, connection_type type) :
			id(-1),
			marked_for_delete(false),
			connections{}
		{
			connection->parent = this;
			connections[(uint32)type] = connection;
		}

		xg_session_pool::xg_session_pool() :
			session_count(0)
		{
			async_every(1000ms, [this]()
			{

				int sessions_purged = 0;
				for (auto& session : *this)
				{
					if (session)
					{
						uint32 id = session->id;

						if (!session->has_connection())
						{

							delete session;
							session = nullptr;
							this->session_count--;
							sessions_purged++;
						}
					}
				}

				if (sessions_purged)
				{
					printf("%d sessions purged.\n", sessions_purged);
				}

			});
		}

		xg_session* xg_session_pool::spawn_session(tcp_connection* connection, connection_type type)
		{
			for (int id = 0; id < this->size(); id++)
			{
				auto& new_session = this->at(id);
				if (!new_session)
				{
					this->session_count++;
					new_session = new xg_session(connection, type);
					new_session->id = id;
					return new_session;
				}
			}

			return nullptr;
		}

		void init_user_session_pool(uint32 max_session_count)
		{
			xg_session_pool.resize(max_session_count);
		}
	}
}
