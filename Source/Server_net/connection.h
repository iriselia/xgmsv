#pragma once

namespace server
{
	namespace core
	{
		template<typename socket_t, typename buffer_t>
		struct async_recv_frame_impl
		{
			socket_t& socket;
			buffer_t buffer;
			asio::error_code error_code;

			async_recv_frame_impl(socket_t& socket, buffer_t&& buffer) : socket(socket), buffer(std::move(buffer))
			{
				this->buffer;
			}

			~async_recv_frame_impl()
			{
				this->buffer;
			}

			asio::awaitable<void> async_read()
			{
				try
				{
					std::size_t n = co_await socket.async_read_some(buffer, asio::use_awaitable);
					//printf("%s\n", /*this_thread::get_debug_name().c_str(),*/ buffer);
					//rdata.erase(rdata.begin(), rdata.begin() + n);

					//for (;;)
					{
						//std::size_t n;
						//socket.async_receive(asio::buffer(rdata, 12), [this, &n](auto error_code, auto bytes_read) { n = bytes_read; });

						//std::size_t n = co_await asio::async_read_until(socket, asio::dynamic_buffer(rdata, 12), "\n", asio::use_awaitable);


					}
				}
				catch (std::exception & e)
				{
					printf("exception: %s", e.what());
				}
			}

			bool await_ready()
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				socket.async_read_some(buffer, [this, coro](auto error_code, auto bytes_read)
				{
					this->error_code = error_code;
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

			void await_resume()
			{
				if (error_code)
				{
					throw asio::system_error(error_code);
				}
			}
		};

		struct tcp_connection
		{
			int id;
			asio::ip::address address;
			uint16 port;
			bool marked_for_delete;

			asio::ip::tcp::socket socket;
			std::vector<uint8> rdata;
			std::vector<uint8> wdata;



			tcp_connection(asio::ip::tcp::socket&& new_socket) :
				id(-1),
				address(new_socket.remote_endpoint().address()),
				port(new_socket.remote_endpoint().port()),
				marked_for_delete(false),
				socket(std::move(new_socket))
			{
				rdata.resize(12);
				//rdata.reserve(12);
			}

			void shutdown_and_close()
			{
				asio::error_code error_code;
				socket.shutdown(asio::ip::tcp::socket::shutdown_both, error_code);

				if (error_code)
					printf("network: tcp_connection::close: %s errored when shutting down socket: %i (%s)", address.to_string().c_str(),
						error_code.value(), error_code.message().c_str());

				socket.close();

				marked_for_delete = true;
			}

			template<typename socket_t, typename buffer_t>
			auto async_recv_frame(socket_t& socket, buffer_t buffer)
			{
				return async_recv_frame_impl<socket_t, buffer_t>(socket, std::forward<buffer_t>(buffer));
			}

			std::future<void> begin_async_recv()
			{
				try
				{
					while (socket.is_open())
					{
						co_await async_recv_frame(socket, asio::buffer(rdata, 5));

					}
				}
				catch (const std::exception& /*exception*/)
				{
					shutdown_and_close();
					//printf("connection %d marked for delete.\n", id);
					//printf("exception: %s", exception.what());
				}
			}
		};
	}
}