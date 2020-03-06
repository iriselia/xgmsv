#pragma once
#include "core/network/connection.h"

namespace server
{
	namespace core
	{
		// public
		extern std::list<struct async_network_service> network_services;

		template<typename lambda>
		void spawn_network_service(std::string ip, uint16 port, uint16 max_connection_count, lambda& accept_handler);

		void start_network();
		void stop_network();

		// detail
		struct async_network_service
		{
			std::string					ip;
			unsigned short				port;
			connection_pool				connection_pool;
			asio::ip::tcp::acceptor		acceptor;
			volatile bool				stop_signal;

			template<typename lambda, typename lambda2, typename lambda3>
			async_network_service(std::string ip, uint16 port, uint16 max_connection_count, lambda& accept_handler, lambda2& receive_handler, lambda3& send_handler) :
				ip(ip), port(port), connection_pool(io_context, ip, port, max_connection_count), acceptor(io_context, { asio::ip::address::from_string(ip), port }), stop_signal(false)
			{
				async_accept_loop(ip, port, accept_handler, receive_handler, send_handler, stop_signal);
			}

			template<typename lambda, typename lambda2, typename lambda3>
			std::future<void> async_accept_loop(std::string ip, unsigned short port, lambda& accept_handler, lambda2& receive_handler, lambda3& send_handler, volatile bool& stop_signal);
		};

		template<typename lambda, typename lambda2, typename lambda3>
		std::future<void> async_network_service::async_accept_loop(std::string ip, unsigned short port, lambda& accept_handler, lambda2& receive_handler, lambda3& send_handler, volatile bool& stop_signal)
		{
			try
			{
				while (!stop_signal)
				{
					auto new_connection = co_await async_accept(acceptor, connection_pool, ip, port, accept_handler);


					if (new_connection)
					{
						//printf("new connection accepted: %d from %s:%d\n", new_connection->id, new_connection->address.to_string().c_str(), port);
						new_connection->async_recv_loop(receive_handler);
						new_connection->async_send_loop(send_handler);
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

		template<typename lambda, typename lambda2, typename lambda3>
		void spawn_network_service(std::string ip, uint16 port, uint16 max_connection_count, lambda& accept_handler, lambda2& receive_handler, lambda3& send_handler)
		{
			assert(server_status == e_server_status::initializing);
			network_services.emplace_back(ip, port, max_connection_count, accept_handler, receive_handler, send_handler);
		}
	}
}
