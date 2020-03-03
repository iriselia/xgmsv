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


		struct connection_pool : public std::vector<tcp_connection*>
		{
			uint32 conneciton_count = 0;
		};

		template<typename lambda>
		struct async_accept_frame_impl
		{
			async_accept_frame_impl(asio::ip::tcp::acceptor& acceptor, connection_pool& connections, std::string ip, unsigned short port, lambda& accept_handler) :
				ip(ip), port(port), accept_handler(accept_handler), acceptor(acceptor), connection_pool(connections), connection_count(connection_count) {}

			std::string ip;
			unsigned short port;
			lambda accept_handler;
			asio::error_code error_code;

			asio::ip::tcp::acceptor& acceptor;
			uint32& connection_count;

			tcp_connection* return_value;
			//asio::ip::tcp::socket socket;
			connection_pool& connection_pool;

			bool await_ready()
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				acceptor.async_accept([this, coro](auto error_code, auto socket)
				{
					this->error_code = error_code;


					return_value = nullptr;

					for (int id = 0; id < this->connection_pool.size(); id++)
					{
						auto& connection = this->connection_pool[id];
						if (!connection)
						{
							this->connection_pool.conneciton_count++;
							connection = new tcp_connection(std::move(socket));
							connection->id = id;
							return_value = connection;
							break;
						}
					}

					if (return_value == nullptr)
					{
						socket.shutdown(asio::socket_base::shutdown_both);
						socket.close();
					}

					accept_handler(return_value);

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

		template<typename lambda>
		auto async_accept_frame(asio::ip::tcp::acceptor& acceptor, connection_pool& connections, std::string ip, unsigned short port, lambda& accept_handler)
		{
			return async_accept_frame_impl<lambda>(acceptor, connections, ip, port, accept_handler);
		}


		struct async_network_service
		{
			template<typename lambda>
			async_network_service(std::string ip, uint16 port, uint16 max_connection_count, lambda& accept_handler) :
				ip(ip), port(port), acceptor(io_context, { asio::ip::address::from_string(ip), port }),
				stop_signal(false)
			{
				connection_pool.resize(max_connection_count);

				async_every(1000ms, [this]()
				{
					int connections_purged = 0;
					for (auto& connection : this->connection_pool)
					{
						if (connection)
						{
							uint32 id = connection->id;
							if (connection->marked_for_delete)
							{
								delete connection;
								connection = nullptr;
								this->connection_pool.conneciton_count--;
								connections_purged++;
							}
						}
					}

					if (connections_purged)
					{
						printf("%d connections purged for %s:%d.\n", connections_purged, this->ip.c_str(), this->port);
					}

				});

				begin_async_accept(ip, port, accept_handler, stop_signal);
			}

			std::string ip;
			unsigned short port;
			connection_pool connection_pool;
			asio::ip::tcp::acceptor acceptor;
			volatile bool stop_signal;



			template<typename lambda>
			std::future<void> begin_async_accept(std::string ip, unsigned short port, lambda& accept_handler, volatile bool& stop_signal)
			{
				try
				{
					while (!stop_signal)
					{
						auto new_connection = co_await async_accept_frame(acceptor, connection_pool, ip, port, accept_handler);


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