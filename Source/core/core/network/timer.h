#pragma once

#include <coroutine>
#include <future>
#include "asio/steady_timer.hpp"
//#include "asio/awaitable.hpp"
//#include "asio/detached.hpp"
//#include "asio/impl/co_spawn.hpp"
//#include "asio/awaitable.hpp"

#include "core/shared.h"

namespace core
{
	struct task;

	// public
	template<typename lambda>
	task async(lambda task);

	template<typename duration = std::chrono::milliseconds, typename lambda>
	task async_after( duration delay, lambda task );

	template<typename duration = std::chrono::milliseconds, typename lambda>
	task async_every( duration delay, lambda task );

	template<typename duration = std::chrono::milliseconds, typename lambda>
	task async_every( duration delay, volatile int& stop_signal, lambda task );

	struct task
	{
		struct promise_type
		{
			task get_return_object() { return {}; }
			std::suspend_never initial_suspend() { return {}; }
			std::suspend_never final_suspend() noexcept { return {}; }
			void return_void() {}
			void unhandled_exception() {}
		};
	};


	enum class signal_code
	{
		no_error = 0,
		shutdown = 1
	};

	struct async_signal_frame
	{
		asio::steady_timer timer;
		signal_code val;

		async_signal_frame() : timer(io_context), val(signal_code::no_error) {}

		~async_signal_frame()
		{
			fire(signal_code::shutdown);
		}

		bool await_ready()
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<> coro)
		{
			timer.expires_at(std::chrono::steady_clock::time_point::max());
			timer.async_wait([this, coro](auto error_code)
			{
				//this->error_code = error_code;
				coro.resume();
			});
		}

		void await_resume()
		{
			switch (val)
			{
			case signal_code::no_error:
				break;
			//case 995:
			//	// signal fired by calling timer.cancel_one() or timer.cancel()
			//	break;
			default:
				break;
				//throw val;
			}
		}

		void fire(signal_code signal = signal_code::no_error)
		{
			val = signal;
			timer.cancel_one();
		}

		void shutdown()
		{
			val = signal_code::shutdown;
			timer.cancel();
		}
	};

	typedef async_signal_frame async_signal;

	template<typename duration>
	auto async_timer(duration delay)
	{
		// detail
		struct async_timer_frame
		{
			asio::steady_timer timer;
			duration delay;
			asio::error_code error_code;

			async_timer_frame( duration delay ) : timer( io_context ), delay( delay ) {}

			bool await_ready()
			{
				return delay.count() == 0;
			}

			void await_suspend( std::coroutine_handle<> coro )
			{
				timer.expires_from_now( delay );
				timer.async_wait( [this, coro]( auto error_code ) { this->error_code = error_code; coro.resume(); } );
			}

			void await_resume()
			{
				if (error_code)
				{
					throw asio::system_error( error_code );
				}
			}
		};

		return async_timer_frame{delay};
	}

	/* disabled due to vs2019 bug
	template<typename lambda>
	auto async(lambda task)
	{
		asio::co_spawn(io_context, [&task]() -> asio::awaitable<void>
		{
			auto executor = co_await asio::this_coro::executor;
			task();
		}, asio::detached);
	}
	*/

	template<typename duration, typename lambda>
	task async_after(duration delay, lambda task)
	{
		co_await async_timer(delay);
		task();
	}

	template<typename duration, typename lambda>
	task async_every(duration delay, lambda task)
	{
		while (true)
		{
			co_await async_timer(delay);
			task();
		}
	}

	template<typename duration, typename lambda>
	task async_every(duration delay, volatile int& stop_signal, lambda task)
	{
		while (!stop_signal)
		{
			co_await async_timer(delay);
			task();
		}
	}
}
