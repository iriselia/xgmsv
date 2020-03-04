#pragma once
#include "connection.h"

namespace server
{
	namespace core
	{
		enum connection_type : uint8
		{
			CONNECTION_TYPE_GAME = 0,
			CONNECTION_TYPE_MAIL = 1,
			MAX_CONNECTION_TYPES,
		};

		struct session
		{
			tcp_connection* connections[MAX_CONNECTION_TYPES];

		};

		struct session_pool : std::vector<session>
		{
			uint32 session_count;
		};

	}
}