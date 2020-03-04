#pragma once

#include "core/utils/standard_libraries.h"
#include "core/utils/defines.h"
#include "core/utils/types.h"
#include "core/utils/assertion_macros.h"
#include "core/utils/thread.h"

#include "asio/io_context.hpp"
#include "core/utils/assertion_macros.h" // fix assert override from asio

namespace server
{
	namespace core
	{
		// public
		extern asio::io_context io_context;
		extern volatile enum class e_server_status server_status;

		void main_loop();

		void init_signals(::std::function<void(int)> custom_signal_handler);

		void init_gui();
		void render_gui();
		void stop_gui();

		// detail
		enum class e_server_status
		{
			initializing = 0,
			running = 1,
			stopped = 2,
			MAX_SIGNAL_TYPES
		};
	};
}