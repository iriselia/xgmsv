#pragma once

#include "asio/co_spawn.hpp"
#include "asio/steady_timer.hpp"
#include "asio/detached.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/read_until.hpp"
#include "asio/read.hpp"

#include <mutex>

#include "assertion_macros.h"

#include "timer.h"
#include "connection.h"
#include "acceptor.h"

namespace server
{
	namespace core
	{
		extern asio::io_context io_context;


		namespace this_thread
		{
			void assign_main_thread();
			std::thread::id get_id();
			void set_debug_name(const std::string& name);
			std::string get_debug_name();
		};


		struct main_thread_info
		{
			std::thread::id id;
			std::string name = "main_thread";
		};

		struct worker_thread : public std::thread
		{
			std::thread::id id;
			std::string name;


			template<typename lambda>
			worker_thread(std::string&& new_name, lambda function) : id(this->get_id()), name(std::move(new_name)), std::thread(function)
			{
				set_thread_name(*this, name.c_str());
			}

		};

		extern main_thread_info main_thread;
		extern std::vector<worker_thread> worker_threads;

		struct async_network_service
		{
			std::string ip;
			unsigned short port;
			connection_pool connection_pool;
			asio::ip::tcp::acceptor acceptor;
			volatile bool stop_signal;

			template<typename lambda>
			async_network_service(std::string ip, uint16 port, uint16 max_connection_count, lambda& accept_handler) :
				ip(ip), port(port), connection_pool(io_context, ip, port, max_connection_count), acceptor(io_context, { asio::ip::address::from_string(ip), port }),
				stop_signal(false)
			{
				begin_async_accept(ip, port, accept_handler, stop_signal);
			}





			template<typename lambda>
			std::future<void> begin_async_accept(std::string ip, unsigned short port, lambda& accept_handler, volatile bool& stop_signal)
			{
				try
				{
					while (!stop_signal)
					{
						auto new_connection = co_await async_accept(acceptor, connection_pool, ip, port, accept_handler);


						if (new_connection)
						{
							//printf("new connection accepted: %d from %s:%d\n", new_connection->id, new_connection->address.to_string().c_str(), port);
							new_connection->begin_async_recv();
						}
						else
						{

							//printf("new connection rejected, connection_count: %d.\n", connection_pool.conneciton_count);
						}
						//co_await printf("something\n");
					}
				}
				catch (const std::exception & exception)
				{
					printf("exception: %s", exception.what());
				}
			}

		};

		extern std::list<async_network_service> network_services;
		extern bool network_services_online;

		template<typename lambda>
		void spawn_network_service(std::string ip, uint16 port, uint16 max_connection_count, lambda& accept_handler)
		{
			assert(!network_services_online);
			network_services.emplace_back(ip, port, max_connection_count, accept_handler);
		}

		void spawn_worker_threads(uint32 worker_count = 0);
		void purge_worker_threads();
		void start_network(uint32 thread_count = 0);
		void stop_network();

			/*
			asio::awaitable<void> reader()
			{
				try
				{
					for (std::string read_msg;;)
					{
						std::size_t n = co_await asio::async_read_until(socket_,
							asio::dynamic_buffer(read_msg, 1024), "\n", asio::use_awaitable);

						room_.deliver(read_msg.substr(0, n));
						read_msg.erase(0, n);
					}
				}
				catch (std::exception&)
				{
					stop();
				}
			}

			asio::awaitable<void> writer()
			{
				try
				{
					while (socket_.is_open())
					{
						if (write_msgs_.empty())
						{
							asio::error_code ec;
							co_await timer_.async_wait(redirect_error(asio::use_awaitable, ec));
						}
						else
						{
							co_await asio::async_write(socket_,
								asio::buffer(write_msgs_.front()), asio::use_awaitable);
							write_msgs_.pop_front();
						}
					}
				}
				catch (std::exception&)
				{
					stop();
				}
			}
			*/
	}
}