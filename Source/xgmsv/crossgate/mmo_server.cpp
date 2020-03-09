#include "docopt.h"

#include "core/shared.h"

#include "core/network/network_service.h"
#include "core/network/timer.h"


#include "crossgate/xg_session.h"

#include "mmo_server.h"

#include "crossgate/xg_crypto.h"
#include "crossgate/xg_packet.h"

#pragma warning(push)
#pragma warning(disable : 26444)

BOOL utf8ToGBK(char* str, int size)
{
	char temp[2048];

	// UTF-8 to UTF-16
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, size, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, size, (LPWSTR)temp, size_needed);

	if (size_needed > 1024)
	{
		return FALSE;
	}

	// UTF-16 to Big5(950)
	int size_needed2 = WideCharToMultiByte(936, 0, (LPCWCH)temp, size_needed, NULL, 0, NULL, NULL);
	WideCharToMultiByte(936, 0, (LPCWCH)temp, size_needed, str, size, NULL, NULL);

	return TRUE;
}

BOOL GBKToUtf8(char* str, int size)
{
	char temp[2048];

	// UTF-8 to UTF-16
	int size_needed = MultiByteToWideChar(936, 0, str, size, NULL, 0);
	MultiByteToWideChar(936, 0, str, size, (LPWSTR)temp, size_needed);

	if (size_needed > 1024)
	{
		return FALSE;
	}

	// UTF-16 to Big5(950)
	int size_needed2 = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)temp, size_needed, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)temp, size_needed, str, size, NULL, NULL);

	return TRUE;
}

BOOL utf8ToBig5(char* str, int size)
{
	char temp[2048];

	// UTF-8 to UTF-16
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, size, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, size, (LPWSTR)temp, size_needed);

	if (size_needed > 1024)
	{
		return FALSE;
	}

	// UTF-16 to Big5(950)
	int size_needed2 = WideCharToMultiByte(950, 0, (LPCWCH)temp, size_needed, NULL, 0, NULL, NULL);
	WideCharToMultiByte(950, 0, (LPCWCH)temp, size_needed, str, size, NULL, NULL);

	return TRUE;
}

BOOL big5ToUtf8(char* str, int size)
{
	char temp[2048];
	// UTF-8 to UTF-16
	int size_needed = MultiByteToWideChar(950, 0, str, size, NULL, 0);
	MultiByteToWideChar(950, 0, str, size, (LPWSTR)temp, size_needed);

	if (size_needed > 1024)
	{
		return FALSE;
	}

	// UTF-16 to Big5(950)
	int size_needed2 = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)temp, size_needed, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)temp, size_needed, str, size, NULL, NULL);

	return TRUE;
}

namespace mmo_server
{
	using namespace server;
	extern const char* usage;

	volatile int stop_signal = 0;

	void init(int argc, char** argv)
	{
		/*
		char* packet_de = "PBt-ztsrSCXizpHiNuKPg5AuyS6PzdstfC7blf3rb-Dbo7tf5NhhQ-+Pg4--7kE6qjGnMfl6th1N20udvQ";
		core::byte_buffer buf_de(4096);
		buf_de.write(packet_de, strlen(packet_de));
		crossgate::decrypt_message(buf_de);
		printf("string:\"%s\"\n", buf_de.data() + buf_de.rpos);

		exit(0);
		//*/

		//*
		char packet[] = R"(IP 127.0.0.1 )";
		core::byte_buffer buf(4096);
		buf.write(packet, sizeof(packet));
		crossgate::encrypt_message(buf);
		printf("string:%s\n", buf.data());
		//*/

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
			auto session = crossgate::xg_session_pool.spawn_session(new_connection, crossgate::connection_type::CONNECTION_TYPE_GAME);

			if (!session)
			{
				printf("session spawn failed.\n");
			}
		};

		auto receive_handler = [](core::byte_buffer& payload) -> bool
		{
			return crossgate::decrypt_message(payload);
		};

		auto send_handler = [](core::byte_buffer& payload) -> bool
		{
			return crossgate::encrypt_message(payload);
		};

		unsigned short port = 9030;
		uint32 max_session_count = 300;

		crossgate::init_user_session_pool(max_session_count);

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

		/*
		core::async_after(200s, []()
		{
			printf("server network stopped.\n");
			core::stop_network();
		});
		*/

		//char packet4[] = "MLVlll6KdKyJmGKKII5pnmuNyMUp31qWQ7QqzcfGII5pnmuNN8X5wcMqzCuyJMAX2";
		//char packet[] = "CmMMlxipiG42g1E";
		//crossgate::decrypt_message(packet, sizeof(packet));
		//char packet2[] = R"(CharList 0 jason|0\\z241400\\z1\\z15\\z0\\z0\\z0\\z15\\z0\\z0\\z100\\z0\\z2\\zjason\\z2\\z游民\\z2\\z106002\\z-1| )";

		//char packet[] = "ClientLogin 0 block ";
		//char packet[] = "CmMMlxipiG42g1E";

		//char packet[] = "eys-ewCrJKFSxY8OgB6CmT1TyPLPj1DMR2j85zoOSMqHnBHQGDkykUM7ckq3bpc1lSJorvUnhmBMh-olkZctmB8GrYSps0hvDHe-H66wzSo3fKWgB++75g";
		//char packet[] = "eyM3e-ijJJlKxYcGgBZ6mTVLyOrHj0jER2D05zIGSMJ-nAnIGGeYkS07zNl3FjzELZUalXxUVO3xRCNZhC2Pvv9MZXuxobNA7Gev-6awxQonbIWYB+eb";
		//char packet[] = R"(CharList 0 jason|0\\z241400\\z1\\z15\\z0\\z0\\z0\\z15\\z0\\z0\\z100\\z0\\z12\\zjason\\z2\\z见习传教士\\z2\\z106002\\z-1| )";
		/*

		char packet[] = "Echo nr ";
		core::byte_buffer buf(4096);
		buf.write(packet, sizeof(packet) - 1);
		//crossgate::decrypt_message(buf);
		crossgate::encrypt_message(buf);
		printf("string:%s\n", buf.data());
		//*/

		core::async_every(1000ms, []()
		{
			core::server_time = std::chrono::steady_clock::now();
			if (core::server_status == core::e_server_status::running)
			{
				for (auto session : crossgate::xg_session_pool)
				{
					if (session)
					{
						core::packet_queue& packets = session->rqueue;

						if (!packets.begin_read())
						{
							continue;
						}

						session->activate();

						while (packets.has_next())
						{
							core::packet* packet = packets.dequeue();
							crossgate::xg_dispatch_packet(session, std::move(*packet));

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