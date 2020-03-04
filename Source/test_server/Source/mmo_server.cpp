#include "docopt.h"


#include "common.h"

#include "core.h"
#include "network.h"

#include "mmo_server.h"

#pragma warning(push)
#pragma warning(disable : 26444)

namespace mmo_server
{
	using namespace server;
	extern const char* usage;

	volatile int stop_signal = 0;

	void init(int argc, char** argv)
	{
		std::map<std::string, docopt::value> args_map = docopt::docopt(mmo_server::usage, { argv + 1, argv + argc }, true, "MMO Server 1.0");
		if (args_map.empty())
		{
			assert(false);
		}

		auto signal_handler = [&](int signal)
		{
			printf("exit signal: %d.\n", signal);
			mmo_server::signal_status = signal;
			core::io_context.stop();
		};

		core::init_signals(signal_handler);
		//core::init_gui();

		auto accept_handler = [](core::tcp_connection* new_connection)
		{
			printf("connection %d accepted\n", new_connection->id);
		};

		unsigned short port = 9030;
		core::spawn_network_service("127.0.0.1", port, 300, accept_handler);
		//core::spawn_network_service("127.0.0.1", port + 1, 300, accept_handler);
		//core::async_accept("127.0.0.1", port, accept_handler, false);
		core::spawn_worker_threads(2);
		core::start_network(2);

		core::async_after(100s, []()
		{
			printf("server network stopped.\n");
			core::stop_network();
		});

		//core::async([]() {printf("async_task\n"); });
		/*
		core::async_after(5s, []() {printf("async_task\n"); });
		core::async_every(1000ms, []()
			{
				printf("update from: %s\n", core::this_thread::get_debug_name().c_str());
			});
		*/

		//core::async_every(5s, stop_signal, []() {printf("stoppable periodic_async_task\n"); });
	}


	void run()
	{
		core::main_loop();
	}

	void stop()
	{
		core::purge_worker_threads();
		//core::stop_gui();
	}
}

#pragma warning(pop)