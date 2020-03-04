#pragma once

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

			template<typename buffer_t>
			auto async_recv(tcp_connection& connection, buffer_t buffer);

			std::future<void> begin_async_recv();
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

			async_recv_frame(connection_t& connection, buffer_t&& buffer) : connection(connection), buffer(std::move(buffer)) {}

			bool await_ready()
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				connection.socket.async_read_some(buffer, [this, coro](auto error_code, size_t bytes_read)
				{
					this->error_code = error_code;
					printf("%lld bytes read from connection %d\n", bytes_read, connection.id);
					return_value = bytes_read;
					coro.resume();
				});

				/*
				asio::co_spawn(io_context,
					[this]()
					{
						return async_read();
					}, asio::detached);


				co_await socket.async_receive(buffer, [this, coro](auto error_code, auto bytes_read)
					{
						coro.resume();
					});

				asio::async_read_until(socket, buffer, "\n", [this, coro](auto error_code, auto bytes_read) {
					this->error_code = error_code;
				coro.resume();
					});

				asio::async_read(socket, buffer, [this, coro](auto error_code, auto bytes_read) {
					this->error_code = error_code;
					coro.resume();
					});
				*/


				//socket.async_receive(buffer, [this, coro](auto error_code) { this->error_code = error_code; coro.resume(); });
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
		auto tcp_connection::async_recv(tcp_connection& connection, buffer_t buffer)
		{
			return async_recv_frame<tcp_connection, buffer_t>(connection, std::forward<buffer_t>(buffer));
		}
	}
}