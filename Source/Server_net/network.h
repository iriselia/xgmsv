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

namespace server
{
	namespace core
	{
		using namespace std::chrono;
		extern asio::io_context io_context;


		namespace this_thread
		{
			void assign_main_thread();
			std::thread::id get_id();
			void set_debug_name(const std::string& name);
			std::string get_debug_name();
		};


		extern std::mutex connection_lock;
		extern std::vector<tcp_connection> connection_pool2;
		extern connection_pool connection_pool3;


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


		template<typename lambda>
		struct async_accept_frame
		{
			std::string ip;
			unsigned short port;
			lambda accept_handler;
			asio::error_code error_code;

			asio::ip::tcp::acceptor& acceptor;
			std::vector<tcp_connection*>& connection_pool;

			tcp_connection* return_value;
			//asio::ip::tcp::socket socket;

			async_accept_frame(asio::ip::tcp::acceptor& acceptor, std::vector<tcp_connection*>& connection_pool, std::string ip, unsigned short port, lambda& accept_handler) :
				ip(ip), port(port), accept_handler(accept_handler), acceptor(acceptor), connection_pool(connection_pool)
			{}

			bool await_ready()
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				acceptor.async_accept([this, coro](auto error_code, auto socket)
				{
					this->error_code = error_code;

					auto new_connection = new tcp_connection(std::move(socket));

					return_value = nullptr;
					uint32 id = 0;

					for (auto& i : this->connection_pool)
					{
						if (!i)
						{
							i = new_connection;
							i->id = id;
							return_value = i;
							break;
						}
						id++;
					}

					coro.resume();
				});
			}

			tcp_connection* await_resume()
			{
				if (error_code)
				{
					throw asio::system_error(error_code);
				}

				return return_value;
			}

		};

		struct async_network_service
		{
			std::string ip;
			unsigned short port;
			std::vector<tcp_connection*> connection_pool;
			uint32 connection_count;
			asio::ip::tcp::acceptor acceptor;
			volatile bool stop_signal;

			template<typename lambda>
			async_network_service(std::string ip, uint16 port, uint16 max_connection_count, lambda& accept_handler) :
				ip(ip), port(port), connection_count(0), acceptor(io_context, { asio::ip::address::from_string(ip), port }),
				stop_signal(false)
			{
				connection_pool.resize(max_connection_count);
				begin_async_accept(ip, port, accept_handler, stop_signal);
			}

			template<typename lambda>
			auto begin_async_accept(std::string ip, unsigned short port, lambda& accept_handler, volatile bool& stop_signal) -> std::future<void>
			{
				try
				{
					while (!stop_signal)
					{
						auto new_connection = co_await async_accept_frame<lambda>(acceptor, connection_pool, ip, port, accept_handler);

						connection_count++;

						if (new_connection)
						{
							printf("new connection accepted: %d from %s:%d\n", new_connection->id, new_connection->address, port);
							new_connection->async_recv();
						}
						else
						{
							printf("new connection rejected, connection_count: %d.\n", connection_count);
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
		extern bool network_services_running;

		template<typename lambda>
		void spawn_network_service(std::string ip, uint16 port, uint16 max_connection_count, lambda& accept_handler)
		{
			assert(!network_services_running);
			network_services.emplace_back(ip, port, max_connection_count, accept_handler);
		}

		void spawn_worker_threads(int thread_count);
		void stop_worker_threads();

		void start_network(int thread_count);
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