#pragma once

#include "asio/ip/tcp.hpp"
#include "asio/read.hpp"

namespace server
{
	namespace core
	{
		struct tcp_connection
		{
			int id;
			asio::ip::address address;
			uint16 port;
			bool marked_for_delete;

			asio::ip::tcp::socket socket;
			std::vector<uint8> rdata;
			std::vector<uint8> wdata;

			tcp_connection(asio::ip::tcp::socket&& new_socket);

			void shutdown_and_close();

			//template<typename buffer_t>
			//auto async_recv(tcp_connection& connection, buffer_t buffer);

			std::future<void> async_recv_loop();
		};


		struct connection_pool : public std::vector<tcp_connection*>
		{
			std::string ip;
			unsigned short port;
			asio::io_context& io_context;
			uint32 conneciton_count = 0;

			connection_pool(asio::io_context& io_context, std::string ip, uint16 port, uint16 max_connection_count);
		};


		template<typename connection_t, typename buffer_t>
		struct async_recv_frame
		{
			connection_t& connection;
			buffer_t buffer;
			asio::error_code error_code;
			size_t return_value;

			async_recv_frame(connection_t& connection, buffer_t&& buffer) :
				connection(connection), buffer(std::move(buffer)), error_code(), return_value(0) {}

			bool await_ready()
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				connection.socket.async_receive(buffer, [this, coro](auto error_code, size_t bytes_read)
				{
					this->error_code = error_code;
					printf("%lld bytes read from connection %d\n", bytes_read, connection.id);
					return_value = bytes_read;
					coro.resume();
				});
			}

			size_t await_resume()
			{
				if (error_code)
				{
					throw asio::system_error(error_code);
				}

				return return_value;
			}
		};

		template<typename buffer_t>
		auto async_recv(tcp_connection& connection, buffer_t buffer)
		{
			return async_recv_frame<tcp_connection, buffer_t>(connection, std::forward<buffer_t>(buffer));
		}

		template<typename lambda>
		struct async_accept_frame
		{
			async_accept_frame(asio::ip::tcp::acceptor& acceptor, connection_pool& connections, std::string ip, unsigned short port, lambda& accept_handler) :
				ip(ip), port(port), accept_handler(accept_handler), acceptor(acceptor), connection_pool(connections), connection_count(connection_pool.conneciton_count), return_value(nullptr) {}

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
		auto async_accept(asio::ip::tcp::acceptor& acceptor, connection_pool& connections, std::string ip, unsigned short port, lambda& accept_handler)
		{
			return async_accept_frame<lambda>(acceptor, connections, ip, port, accept_handler);
		}

	}
}