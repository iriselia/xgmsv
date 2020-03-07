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

		auto receive_handler = [](core::byte_buffer& payload)
		{
			crossgate::decrypt_message(payload);

		};

		auto send_handler = [](core::byte_buffer& payload)
		{
			crossgate::encrypt_message(payload);

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
		//char packet[] = "CmMMlxipiG42g1E";
		//crossgate::decrypt_message(packet, sizeof(packet));
		//char packet2[] = R"(CharList 0 jason|0\\z241400\\z1\\z15\\z0\\z0\\z0\\z15\\z0\\z0\\z100\\z0\\z2\\zjason\\z2\\zÓÎÃñ\\z2\\z106002\\z-1| )";

		//char packet[] = "ClientLogin 0 block ";
		//char packet3[] = "FC ";

		//char packet[] = "QWHrI4t3y0x4gnZO+XBw+LGL7XYV3o5Fi98-507yuCnzU0T7YZRHP8OxQsbzdM9K-6fC9EnlhJ-eRRlhU0d4ZsWJzgBDs4Z1WPkbKs2kDNb47dw8Ff8w0tH3WMcheib0TxHZ1t0ZOrch9ID2nvd7xiZzTZHagPNmLT8RrtGLGXNAOzPph-w3eSC-HQps2DIekTuaxcYng1vgpYIgAZ8Tj9P2YjLMtU+Szf-1adkDOAcQstXdz3TukE85NMHYWfoHTv0lyhYeUk1qechP9j6FSj11vvX8HPEcvJyNLVhJI-o35jBF64unpAR+XsY1zfn3-ElvdsB7ZgXZDk7lkXoNQ6sbjmB1ath9brzXbJLzXdQb3mmDrRQ3F+4fjkiwoGn8hhzuxgLUb9BlXRc9PafSzn+C0HI5cSHLfyyoBI+41G5gQOr9-xzuxlPUb9BlXT49PaeDzn8C0mBExTCAagCAI8ZLc7W1qX5Rs0KATXAAO2Mc53Ip440JozG443chGFi-cBHklsg4kfAhEMIAyNOBlHrh3Ydz1os-c4sE-JDF9BDkhJSdGoN5LuLL3gopnMvN4DEQK7OevOQIGiO-0m3jc8t8bLhQWvUi4nse6zz7UwzdPN2wM9j95maFs6vA2EmW81VvOB5LoLDXMgIwWsE5FA44OR13216pPgyv2Lyxn6PUZLQoWYgjfYic9GY2c96MInI+OEVvSmqv3ccSysTev-4FxLjNpQDZDtG7wVyQRr3GZDBaHLFujbmI6vbvR6HeRCWzasfEcSa8gsNpZdlRC58Ce2e2JwSCdt+m50g8r22PSYCArfE8Gh87Wr3nnO4yQJSYNzBzIWV4TjEfhUwSefAmA5yd0oDzp0Ty4lHPQE9xEBJxfP1+eNGzQnwd5B1RSlWUifBr-j4A64GteBZPRZJy8Js5aQX4uJgWYCoVveOmZTAFfxWpr-cVJg3WYz6knLrpnUeFja544skgZILFG5+BKeBoUB42hoNjIeAlNwoHBRJemHwL5HPIzRWSW+9LOcHk1Z61KRU9sSBlq+oC-lf9ArCI7rk1U1v1GxngdFR+salmFfcMLX50HmY8NuQW5K9jjsUkFFan";
		char packet[] = "s7Oar2m1BIPYpdaZ1eXOVKDYpotoD5GWg-i3gjin0a3ddKFVX-TgJZBGp2PRbDC0VavRTa08l9drNWsItjIDQA";
		core::byte_buffer buf(4096);
		buf.write(packet, sizeof(packet) - 1);
		crossgate::decrypt_message(buf);
		//crossgate::encrypt_message(buf);

		//big5ToUtf8((char*)buf.data(), buf.wpos);

		//printf("string:%s", buf.data());

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