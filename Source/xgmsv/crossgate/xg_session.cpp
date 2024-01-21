#pragma once

#include "core/shared.h"

#include "core/network/timer.h"

#include "crossgate/xg_packet.h"
#include "crossgate/xg_session.h"

namespace crossgate
{
	struct xg_session_pool xg_session_pool;

	xg_session::xg_session(core::tcp_connection* connection, connection_type type) :
		connections{},
		scratch_buffer{ 4096 }
	{
		connections[(uint32)type] = connection;

		connection->rqueue = &rqueue;
		connection->wqueue = &wqueue;
		connection->wsignal = &signal;
	}

	xg_session_pool::xg_session_pool() :
		session_count(0)
	{}

	xg_session* xg_session_pool::spawn_session(core::tcp_connection* connection, connection_type type)
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
		core::async_every(1000ms, []()
		{

			int sessions_purged = 0;
			for (auto& session : xg_session_pool)
			{
				if (session)
				{
					uint32 id = session->id;

					auto inactive_seconds = std::chrono::duration_cast<std::chrono::seconds>(core::server_time - session->last_active);

					if (inactive_seconds.count() > 60)
					{
						delete session;
						session = nullptr;
						xg_session_pool.session_count--;
						sessions_purged++;
					}

					//if (session->marked_for_delete)
					//{
					//}
				}
			}

			if (sessions_purged)
			{
				printf("%d sessions purged.\n", sessions_purged);
			}

		});

		xg_session_pool.resize(max_session_count);
	}
}
