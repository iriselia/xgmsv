#pragma once

namespace server
{
	namespace core
	{
		template<typename socket_type, typename buffer_type>
		struct async_read_frame
		{
			socket_type& socket;
			buffer_type buffer;
			asio::error_code error_code;

			async_read_frame(socket_type& socket, buffer_type&& buffer) : socket(socket), buffer(std::move(buffer))
			{
				this->buffer;
			}

			~async_read_frame()
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
				*/
				/*
				asio::async_read_until(socket, buffer, "\n", [this, coro](auto error_code, auto bytes_read) {
					this->error_code = error_code;
				coro.resume();
					});
					*/
					/*
					asio::async_read(socket, buffer, [this, coro](auto error_code, auto bytes_read) {
						this->error_code = error_code;
						coro.resume();
						});
						*/
				socket.async_read_some(buffer, [this, coro](auto error_code, auto bytes_read) {
					this->error_code = error_code;
					coro.resume();
					});

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
			asio::ip::tcp::socket socket;
			std::vector<uint8> rdata;
			std::vector<uint8> wdata;

			int id;
			asio::ip::address address;
			uint16 port;
			bool marked_for_delete;



			tcp_connection(asio::ip::tcp::socket&& new_socket) :
				socket(std::move(new_socket)),
				id(-1),
				address(socket.remote_endpoint().address()),
				port(socket.remote_endpoint().port())
			{
				rdata.resize(12);
				//rdata.reserve(12);
			}

			void close()
			{
				asio::error_code error_code;
				socket.shutdown(asio::ip::tcp::socket::shutdown_both, error_code);

				if (error_code)
					printf("network: tcp_connection::close: %s errored when shutting down socket: %i (%s)", address.to_string().c_str(),
						error_code.value(), error_code.message().c_str());

				socket.close();

				marked_for_delete = true;
			}

			//*
			template<typename socket_type, typename buffer_type>
			auto async_recv_helper2(socket_type& socket, buffer_type&& buffer)
			{
				return async_read_frame<socket_type, buffer_type>(socket, std::forward<buffer_type>(buffer));
			}

			template<typename socket_type, typename buffer_type>
			auto async_recv_helper(socket_type& socket, buffer_type buffer) -> std::future<void>
			{
				try
				{
					while (socket.is_open())
					{
						co_await async_recv_helper2(socket, std::forward<buffer_type>(buffer));

					}
				}
				catch (const std::exception & /*exception*/)
				{
					printf("connection %d closed.", id);
					//printf("exception: %s", exception.what());
				}
			}

			auto async_recv()
			{
				auto buffer = asio::buffer(rdata, 5);
				async_recv_helper(socket, asio::buffer(rdata, 5));
			}
#if 0
			//*/
			asio::awaitable<void> async_read()
			{
				try
				{
					char data[1024];
					//for (rdata;;)
					for (;;)
					{
						//std::size_t n;
						//socket.async_receive(asio::buffer(rdata, 12), [this, &n](auto error_code, auto bytes_read) { n = bytes_read; });
						std::size_t n = co_await socket.async_read_some(asio::buffer(rdata, 12), asio::use_awaitable);

						//std::size_t n = co_await asio::async_read_until(socket, asio::dynamic_buffer(rdata, 12), "\n", asio::use_awaitable);

						printf("%s\n", /*this_thread::get_debug_name().c_str(),*/ rdata.data());

						rdata.erase(rdata.begin(), rdata.begin() + n);
					}
				}
				catch (std::exception & e)
				{
					printf("exception: %s", e.what());
				}
			}
#endif
		};

		static_assert(std::is_nothrow_move_constructible<tcp_connection>::value);

		struct connection_pool
		{
			std::vector<std::atomic<tcp_connection*>> connections;
			bool allow_overflow;

			void init(uint32 max_connection_count, bool allow_overflow = false)
			{
				//connections.reserve(max_connection_count);
				this->allow_overflow = allow_overflow;
			}

			tcp_connection* allocate(asio::ip::tcp::socket&& socket)
			{
				tcp_connection* test = nullptr;
				tcp_connection* taken = test + 1;

				int id = 0;
				for (auto& i : connections)
				{
					bool exchanged = i.compare_exchange_weak(test, taken);

					if (exchanged)
					{
						//auto new_connection = new tcp_connection(std::move(socket));
						//new_connection->id = id;
						//i = new_connection;
						//return new_connection;
					}

					id++;
				}

				return nullptr;
			}

			void release(int id)
			{
				//auto connection = connections[id].load();
				//delete connection;
				//connections[id].store(nullptr);
			}
		};
	}
}