#pragma once
//#include "docopt.h"
//#include "common.h"
//#include "asio.hpp"

//#include "asio/io_context.hpp"


using namespace std::chrono;

namespace asio
{
	class io_context;
}

namespace server
{
	extern volatile int signal_status;

	namespace core
	{
		extern asio::io_context io_context;




		void main_loop();

		void init_signals(::std::function<void(int)> custom_signal_handler);
		void init_io_context(int thread_count);

		void init_gui();
		void render_gui();
		void stop_gui();



		//void (*init)(std::map<std::string, docopt::value>) = [](std::map<std::string, docopt::value>) { assert(false); };
		//void (*stop)() = []() { assert(false); };
	};
}
