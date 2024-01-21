#include "core/utils/index.h"

#include "core/shared.h"

#include "core/network/connection.h"
#include "core/network/timer.h"

namespace core
{
	tcp_connection::tcp_connection(asio::ip::tcp::socket&& new_socket) :
		id(-1),
		address(new_socket.remote_endpoint().address()),
		port(new_socket.remote_endpoint().port()),
		marked_for_delete(false),
		//parent(nullptr),
		socket(std::move(new_socket)),
		rbuffer(4096),
		wbuffer(4096)
	{
	}

	void tcp_connection::mark_for_delete()
	{
		asio::error_code error_code;
		socket.shutdown(asio::ip::tcp::socket::shutdown_both, error_code);

		/*
		if (error_code)
			printf("network: tcp_connection::close: %s errored when shutting down socket: %i (%s)\n", address.to_string().c_str(),
				error_code.value(), error_code.message().c_str());
		*/
			
		socket.close();

		wsignal->fire(signal_code::shutdown);

		marked_for_delete = true;
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
						if (!connection->wqueue && !connection->rqueue)
						{
							delete connection;
							connection = nullptr;
							this->conneciton_count--;
							connections_purged++;
						}


						//*
						//if (connection->parent)
						//{
						//	connection->parent->remove_connection(connection);
						//}

						//*/
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
