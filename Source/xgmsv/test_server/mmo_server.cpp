#include "docopt.h"

#include "core/shared.h"

#include "core/network/network_service.h"
#include "core/network/timer.h"
#include "core/network/xg_session.h"


#include "mmo_server.h"

#include "xg_crypto.h"
#include "xg_packet.h"

#pragma warning(push)
#pragma warning(disable : 26444)

namespace mmo_server
{
	using namespace server;
	extern const char* usage;

	volatile int stop_signal = 0;

	void init(int argc, char** argv)
	{
		std::map<std::string, docopt::value> args_map = docopt::docopt(mmo_server::usage, { argv + 1, argv + argc }, true, "MMO Server 1.0");
		if (args_map.empty())
		{
			assert(false);
		}

		auto signal_handler = [&](int signal)
		{
			printf("exit signal: %d.\n", signal);
			core::server_status = core::e_server_status::stopped;
			core::io_context.stop();
		};

		core::init_signals(signal_handler);
		//core::init_gui();

		auto accept_handler = [](core::tcp_connection* new_connection)
		{
			printf("connection %d accepted\n", new_connection->id);
			auto session = core::xg_session_pool.spawn_session(new_connection, core::connection_type::CONNECTION_TYPE_GAME);

			if (!session)
			{
				printf("session spawn failed.\n");
			}
		};

		auto receive_handler = [](uint8*& data, uint32& bytes_read)
		{
			crossgate::decrypt_message((char*&)data, bytes_read);

		};

		auto send_handler = [](uint8*& data, uint32& bytes_to_send)
		{
			crossgate::decrypt_message((char*&)data, bytes_to_send);

		};

		unsigned short port = 9030;
		uint32 max_session_count = 300;

		core::init_user_session_pool(max_session_count);

		core::spawn_network_service("127.0.0.1", port, 300, accept_handler, receive_handler, send_handler);
		//core::spawn_network_service("127.0.0.1", port + 1, 300, accept_handler);
		//core::async_accept("127.0.0.1", port, accept_handler, false);

		/*
		core::spawn_worker_threads(2, []()
		{
			while (core::server_status == core::e_server_status::initializing)
			{
				std::this_thread::sleep_for(10ms);
			}

			core::io_context.run();
		});
		*/

		core::start_network();

		core::async_after(200s, []()
		{
			printf("server network stopped.\n");
			core::stop_network();
		});

		//char packet4[] = "MLVlll6KdKyJmGKKII5pnmuNyMUp31qWQ7QqzcfGII5pnmuNN8X5wcMqzCuyJMAX2";
		char packet[] = "CmMMlxipiG42g1E";
		//crossgate::decrypt_message(packet, sizeof(packet));

		char packet2[] = "Echo nr ";

		crossgate::encrypt_message(packet2, sizeof(packet2));

		core::async_every(1000ms, []()
		{
			if (core::server_status == core::e_server_status::running)
			{
				for (auto& session : core::xg_session_pool)
				{
					if (session)
					{
						core::packet_queue& packets = session->connections[0]->rqueue;

						if (!packets.begin_read())
						{
							continue;
						}

						while (packets.has_next())
						{
							core::packet* packet = packets.dequeue();
							crossgate::xg_dispatch_packet(std::move(*packet));

							//std::string packet_str((char*)packet->data, packet->length);



							//packet_str = crossgate::decrypt_message(packet_str);
							//printf("new packet:%s\n", packet_str.c_str());
							//xg_dispatch_packet()
						}


						packets.end_read();
					}
				}
			}
		});

		//core::async([]() {printf("async_task\n"); });
		/*
		core::async_after(5s, []() {printf("async_task\n"); });
		core::async_every(1000ms, []()
			{
				printf("update from: %s\n", core::this_thread::get_debug_name().c_str());
			});
		*/

		//core::async_every(5s, stop_signal, []() {printf("stoppable periodic_async_task\n"); });
	}


	void run()
	{
		core::main_loop();
	}

	void stop()
	{
		core::purge_worker_threads();
		//core::stop_gui();
	}
}

#pragma warning(pop)