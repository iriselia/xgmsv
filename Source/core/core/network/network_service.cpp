#include "core/utils/index.h"

#include "core/shared.h"

#include "core/network/network_service.h"

#include "core/utils/assertion_macros.h" // fix assert override from asio


namespace core
{
	std::list<async_network_service> network_services;

	void start_network()
	{
		assert(server_status != e_server_status::running);

		server_status = e_server_status::running;
	}

	void stop_network()
	{
		assert(server_status == e_server_status::running);
		for (auto& i : network_services)
		{
			i.stop_signal = true;
		}

		io_context.stop();
		server_status = e_server_status::stopped;
	}

}
