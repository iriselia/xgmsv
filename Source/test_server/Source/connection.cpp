#include "asio/ip/tcp.hpp"

#include "common.h"

#include "core.h"

#include "connection.h"
#include "timer.h"

namespace server
{
	namespace core
	{
		tcp_connection::tcp_connection(asio::ip::tcp::socket&& new_socket) :
			id(-1),
			address(new_socket.remote_endpoint().address()),
			port(new_socket.remote_endpoint().port()),
			marked_for_delete(false),
			socket(std::move(new_socket))
		{
			rdata.resize(1024);
			//rdata.reserve(12);
		}

		void tcp_connection::shutdown_and_close()
		{
			asio::error_code error_code;
			socket.shutdown(asio::ip::tcp::socket::shutdown_both, error_code);

			if (error_code)
				printf("network: tcp_connection::close: %s errored when shutting down socket: %i (%s)", address.to_string().c_str(),
					error_code.value(), error_code.message().c_str());

			socket.close();

			marked_for_delete = true;
		}

		std::future<void> server::core::tcp_connection::begin_async_recv()
		{
			try
			{
				while (socket.is_open())
				{
					size_t bytes_read = co_await async_recv(*this, asio::buffer(rdata, 1024));

					printf("connection %d:%s\n", id,/*this_thread::get_debug_name().c_str(),*/ &rdata[0]);
					memset(&rdata[0], 0, bytes_read);

				}
			}
			catch (const std::exception& /*exception*/)
			{
				shutdown_and_close();
				//printf("connection %d marked for delete.\n", id);
				//printf("exception: %s", exception.what());
			}
		}

		connection_pool::connection_pool(asio::io_context& io_context, std::string ip, uint16 port, uint16 max_connection_count) :
			ip(ip), port(port), io_context(io_context)
		{
			this->resize(max_connection_count);

			async_every(1000ms, [this]()
			{
				int connections_purged = 0;
				for (auto& connection : *this)
				{
					if (connection)
					{
						uint32 id = connection->id;
						if (connection->marked_for_delete)
						{
							delete connection;
							connection = nullptr;
							this->conneciton_count--;
							connections_purged++;
						}
					}
				}

				if (connections_purged)
				{
					printf("%d connections purged for %s:%d.\n", connections_purged, this->ip.c_str(), this->port);
				}

			});
		}

	}
}
