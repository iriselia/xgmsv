#include <string>
#include "asio/write.hpp"

#include "common.h"

#include "network.h"

namespace server
{
	namespace core
	{
		main_thread_info main_thread;
		std::vector<worker_thread> worker_threads;
		std::vector<tcp_connection> connection_pool2;
		connection_pool connection_pool3;

		std::list<async_network_service> network_services;
		bool network_services_running;






		/*
		asio::awaitable<void> echo(asio::ip::tcp::socket socket)
		{
			try
			{
				char data[1024];
				for (;;)
				{
					std::size_t n = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);
					co_await async_write(socket, asio::buffer(data, n), asio::use_awaitable);
				}
			}
			catch (std::exception & e)
			{
				std::printf("echo Exception: %s\n", e.what());
			}
		}

		asio::awaitable<void> listener()
		{
			auto executor = co_await asio::this_coro::executor;
			asio::ip::tcp::acceptor acceptor(executor, { asio::ip::tcp::v4(), 55555 });
			for (;;)
			{
				asio::ip::tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
				asio::co_spawn(executor,
					[socket = std::move(socket)]() mutable
				{
					return echo(std::move(socket));
				},
					asio::detached);
			}
		}
		*/
		void spawn_worker_threads(/*asio::io_context& io_context, */int thread_count)
		{

			worker_threads.reserve(thread_count);
			for (int i = 0; i < thread_count; i++)
			{
				worker_threads.emplace_back(
					std::string("worker_thread_") + std::to_string(i),
					[i]()
					{
						io_context.run();
						printf("%s exited.\n", worker_threads[i].name.c_str());
					});
			}

			auto id = worker_threads.back().get_id();
		}

		void stop_worker_threads()
		{

		}


		void start_network(int thread_count)
		{
			assert(!network_services_running);

			spawn_worker_threads(thread_count);
		}

		void stop_network()
		{
			assert(network_services_running);
			for (auto& i : network_services)
			{
				i.stop_signal = true;
			}
			stop_worker_threads();
		}

		void this_thread::assign_main_thread()
		{
			main_thread.id = std::this_thread::get_id();
			this_thread::set_debug_name(main_thread.name.c_str());
		}

		std::thread::id this_thread::get_id()
		{
			return std::this_thread::get_id();
		}

		void this_thread::set_debug_name(const std::string& new_name)
		{
			if (std::this_thread::get_id() == main_thread.id)
			{
				main_thread.name = new_name;
				set_thread_name(new_name.c_str());
				return;
			}

			for (auto& i : worker_threads)
			{
				if (std::this_thread::get_id() == i.id)
				{
					i.name = new_name;
					set_thread_name(new_name.c_str());
					return;
				}
			}

			assert(false);
			return;
		}

		std::string this_thread::get_debug_name()
		{
			if (std::this_thread::get_id() == main_thread.id)
			{
				return main_thread.name;
			}

			for (auto& i : worker_threads)
			{
				if (std::this_thread::get_id() == i.id)
				{
					return i.name;
				}
			}

			assert(false);
			return std::string();
		}

	}
}