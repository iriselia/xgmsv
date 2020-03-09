#pragma once

#include "asio/steady_timer.hpp"

#include "core/shared.h"

namespace server
{
	namespace core
	{
		// public
		template<typename lambda>
		auto async(lambda task);

		template<typename duration = std::chrono::milliseconds, typename lambda>
		auto async_after(duration delay, lambda task) -> std::future<void>;

		template<typename duration = std::chrono::milliseconds, typename lambda>
		auto async_every(duration delay, lambda task) -> std::future<void>;

		template<typename duration = std::chrono::milliseconds, typename lambda>
		auto async_every(duration delay, volatile int& stop_signal, lambda task) -> std::future<void>;


		// detail
		template<typename duration>
		struct async_timer_frame
		{
			asio::steady_timer timer;
			duration delay;
			asio::error_code error_code;

			async_timer_frame(duration delay) : timer(io_context), delay(delay) {}

			bool await_ready()
			{
				return delay.count() == 0;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				timer.expires_from_now(delay);
				timer.async_wait([this, coro](auto error_code) { this->error_code = error_code; coro.resume(); });
			}

			void await_resume()
			{
				if (error_code)
				{
					throw asio::system_error(error_code);
				}
			}
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

			void await_suspend(std::experimental::coroutine_handle<> coro)
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
			return async_timer_frame<duration>(delay);
		}

		template<typename lambda>
		auto async(lambda task)
		{
			asio::co_spawn(io_context, [&task]() -> asio::awaitable<void>
			{
				auto executor = co_await asio::this_coro::executor;
				task();
			}, asio::detached);
		}

		template<typename duration, typename lambda>
		auto async_after(duration delay, lambda task) -> std::future<void>
		{
			co_await async_timer(delay);
			task();
		}

		template<typename duration, typename lambda>
		auto async_every(duration delay, lambda task) -> std::future<void>
		{
			while (true)
			{
				co_await async_timer(delay);
				task();
			}
		}

		template<typename duration, typename lambda>
		auto async_every(duration delay, volatile int& stop_signal, lambda task) -> std::future<void>
		{
			while (!stop_signal)
			{
				co_await async_timer(delay);
				task();
			}
		}
	}
}
