#pragma once

#include "asio/ip/tcp.hpp"
#include "asio/read.hpp"

#include "core/network/timer.h"
#include "core/network/packet_queue.h"
#include "core/network/byte_buffer.h"

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
			byte_buffer rbuffer;
			byte_buffer wbuffer;

			packet_queue* rqueue;
			packet_queue* wqueue;
			async_signal* wsignal;


			tcp_connection(asio::ip::tcp::socket&& new_socket);

			void mark_for_delete();

			//template<typename buffer_t>
			//auto async_recv(tcp_connection& connection, buffer_t buffer);

			template<typename lambda>
			std::future<void> async_recv_loop(lambda& receive_handler);

			template<typename lambda>
			std::future<void> async_send_loop(lambda& send_handler);
		};


		struct connection_pool : public std::vector<tcp_connection*>
		{
			std::string ip;
			unsigned short port;
			asio::io_context& io_context;
			uint32 conneciton_count = 0;

			connection_pool(asio::io_context& io_context, std::string ip, uint16 port, uint16 max_connection_count);

			tcp_connection* spawn_connection(asio::ip::tcp::socket&& socket)
			{
				for (int id = 0; id < this->size(); id++)
				{
					auto& connection = this->at(id);
					if (!connection)
					{
						this->conneciton_count++;
						connection = new tcp_connection(std::move(socket));
						connection->id = id;
						return connection;
					}
				}

				return nullptr;
			}
		};

		template<typename lambda>
		std::future<void> tcp_connection::async_send_loop(lambda& send_handler)
		{
			try
			{
				while (socket.is_open())
				{
					packet_queue& packets = *wqueue;

					if (packets.begin_read())
					{
						while (packets.has_next())
						{
							core::packet* packet = packets.dequeue();

							if (wbuffer.free_space() < packet->length * 2)
							{
								packets.requeue(packet);
								break;
							}
							uint8* packet_data = wbuffer.write(packet->data, packet->length);

							//wbuffer.rpos = packet_data - wbuffer.data();
							//wbuffer.rend = wbuffer.rpos + packet->length;

							//crossgate::xg_dispatch_packet(std::move(*packet));
							send_handler(wbuffer);

							//std::string packet_str((char*)packet->data, packet->length);

							//packet_str = crossgate::decrypt_message(packet_str);
							//printf("outgoing packet:%s\n", wbuffer.data());
							//xg_dispatch_packet()
						}
						packets.end_read();
					}

					if (wbuffer.size())
					{
						uint32 bytes_transferred = co_await async_send(*this, asio::buffer(wbuffer.data(), wbuffer.size()));

						wbuffer.erase(bytes_transferred);
					}
					else
					{
						co_await *wsignal;

						if (wsignal->val == signal_code::shutdown)
						{
							break;
						}
					}
				}
			}
			catch (const std::exception& exception)
			{
				//printf("connection %d marked for delete.\n", id);
				printf("exception: %s", exception.what());
			}
			catch (signal_code signal)
			{
				if (signal == signal_code::shutdown)
				{
					printf("async_send_loop exited.\n");
				}
			}

			wqueue = nullptr;
			mark_for_delete();
		}


		template<typename lambda>
		std::future<void> tcp_connection::async_recv_loop(lambda& receive_handler)
		{
			try
			{
				uint32 bytes_deferred = 0;

				while (socket.is_open())
				{
					uint8* rdata = rbuffer.data();
					uint32 bytes_read = co_await async_recv(*this, asio::buffer(rdata + bytes_deferred, 1024 - bytes_deferred));

					if (bytes_read == -1)
					{
						break;
					}

					uint32 total_bytes = bytes_deferred + bytes_read;

					uint32 begin = 0;
					uint32 end = 0;
					uint32 index = 0;
					while (index != total_bytes)
					{
						if (rdata[index] == '\n')
						{
							rdata[index] = '\0';

							end = index;

							rbuffer.rpos = begin;
							rbuffer.rend = end;

							if (rbuffer.data()[rbuffer.rpos] == '\0')
							{
								printf("empty string\n");
							}
							else
							{
								// receive assumes decrypted packet is <= encrypted packet.
								bool success = receive_handler(rbuffer);

								if (!success)
								{
									printf("bad packet, packet dropped.\n");
								}

								if (!rqueue->enqueue(rbuffer.data() + rbuffer.rpos, rbuffer.rend - rbuffer.rpos))
								{
									printf("packet_queue full, packet dropped.\n");
								}
							}
							begin = end + 1;
						}

						index++;
					}

					bytes_deferred = total_bytes - begin;
					memcpy(rdata, rdata + begin, bytes_deferred);
					memset(rdata + bytes_deferred, 0, total_bytes - bytes_deferred);
				}
			}
			catch (const std::exception& exception)
			{
				//printf("connection %d marked for delete.\n", id);
				printf("exception: %s", exception.what());
			}

			rqueue = nullptr;
			mark_for_delete();
		}


		template<typename connection_t, typename buffer_t>
		struct async_send_frame
		{
			connection_t& connection;
			buffer_t buffer;
			asio::error_code error_code;
			uint32 return_value;

			async_send_frame(connection_t& connection, buffer_t&& buffer) :
				connection(connection), buffer(std::move(buffer)), error_code(), return_value(0) {}

			bool await_ready()
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> coro)
			{
				connection.socket.async_send(buffer, [this, coro](auto error_code, size_t bytes_sent)
				{
					this->error_code = error_code;
					printf("%zd bytes sent to connection %d\n", bytes_sent, connection.id);
					return_value = (uint32)bytes_sent;
					coro.resume();
				});
			}

			uint32 await_resume()
			{
				if (error_code)
				{
					throw asio::system_error(error_code);
				}

				return return_value;
			}
		};

		template<typename connection_t, typename buffer_t>
		struct async_recv_frame
		{
			connection_t& connection;
			buffer_t buffer;
			asio::error_code error_code;
			uint32 return_value;

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
					if (bytes_read)
					{
						printf("%zd bytes read from connection %d\n", bytes_read, connection.id);
					}

					return_value = (uint32)bytes_read;
					coro.resume();
				});
			}

			uint32 await_resume()
			{
				if (error_code)
				{
					return_value = -1;
					//throw asio::system_error(error_code);
				}

				return return_value;
			}
		};

		template<typename buffer_t>
		auto async_send(tcp_connection& connection, buffer_t buffer)
		{
			return async_send_frame<tcp_connection, buffer_t>(connection, std::forward<buffer_t>(buffer));
		}

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

					return_value = this->connection_pool.spawn_connection(std::move(socket));

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