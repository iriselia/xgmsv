#include <future>
#include <chrono>
#include <asio/io_context.hpp>
#include <asio/system_timer.hpp>

using namespace std::chrono;

/*
template <typename R, typename... Args>
struct std::experimental::coroutine_traits<std::future<R>, Args...>
{
	std::future<R>&& input;
	std::future<R> output;

	struct promise_type
	{
		std::promise<R> p;
		auto get_return_object()
		{
			return p.get_future();
		}

		std::experimental::suspend_never initial_suspend()
		{
			return {};
		}
		std::experimental::suspend_never final_suspend()
		{
			return {};
		}

		void set_exception(std::exception_ptr e)
		{
			p.set_exception(std::move(e));
		}
		
		template <typename U> void return_value(U&& u)
		{
			p.set_value(std::forward<U>(u));
		}

		template <typename R> auto operator co_await(std::future<R>&& f) {
			struct Awaiter {
				std::future<R>&& input;
				std::future<R> output;
				//bool await_ready() { return false; }
				bool await_ready() {
					if (input.is_ready()) {
						output = std::move(input);
						return true;
					}
					return false;
				}
				auto await_resume() { return output.get(); }
				void await_suspend(std::experimental::coroutine_handle<> coro) {
					input.then([this, coro](auto result_future) {
						this->output = std::move(result_future);
						coro.resume();
						});
				}
			};
			return Awaiter{ static_cast<std::future<R>&&>(f) };
		}
	};
};
*/




template <typename R, typename P>
auto async_await(asio::system_timer& t, std::chrono::duration<R, P> d)
{
	struct Awaiter
	{
		asio::system_timer& t;
		std::chrono::duration<R, P> d;
		asio::error_code ec;

		bool await_ready()
		{
			return d.count() == 0;
		}

		void await_resume()
		{
			if (ec)
			{
				throw asio::system_error(ec);
			}
		}

		void await_suspend(std::experimental::coroutine_handle<> coro)
		{
			t.expires_from_now(d);
			t.async_wait(
				[this, coro](auto ec)
				{
					this->ec = ec;
					coro.resume();
				});
		}
	};

	return Awaiter{ t, d };
}

std::future<void> sleepy(asio::io_context& io_context)
{
	asio::system_timer timer(io_context);

	puts("tick0");

	co_await async_await(timer, 1s);
	puts("tick1");
	
	co_await async_await(timer, 1s);
	puts("tick2");
	
	co_await async_await(timer, 1s);
	puts("tick3");
}

//*
int main() {
	asio::io_context io_context;
	sleepy(io_context);
	sleepy(io_context);
	io_context.run();
};
//*/