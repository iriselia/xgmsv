#pragma once

namespace server
{
	namespace core
	{
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