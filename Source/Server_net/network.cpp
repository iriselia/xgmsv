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
		std::list<async_network_service> network_services;
		bool network_services_online;






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

		void spawn_worker_threads(uint32 worker_count)
		{
			if (worker_count == 0)
			{
				worker_count = std::thread::hardware_concurrency();
			}

			worker_threads.reserve(worker_count);
			for (int i = 0; i < worker_count; i++)
			{
				worker_threads.emplace_back(
					std::string("worker_thread_") + std::to_string(i),
					[i]()
				{
					while (!network_services_online)
					{
						std::this_thread::sleep_for(10ms);
					}

					io_context.run();
					printf("%s exited.\n", worker_threads[i].name.c_str());
				});
			}
		}

		void purge_worker_threads()
		{
			for (auto& worker : worker_threads)
			{
				if (worker.joinable())
				{
					worker.join();
				}
			}
			worker_threads.clear();
		}

		void start_network(uint32 worker_count)
		{
			assert(!network_services_online);

			//spawn_worker_threads(worker_count);

			network_services_online = true;
		}

		void stop_network()
		{
			assert(network_services_online);
			for (auto& i : network_services)
			{
				i.stop_signal = true;
			}

			io_context.stop();
			network_services_online = false;
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