//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"

using asio::ip::tcp;

class chat_message
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 512 };

	chat_message()
		: body_length_(0)
	{
	}

	const char* data() const
	{
		return data_;
	}

	char* data()
	{
		return data_;
	}

	std::size_t length() const
	{
		return header_length + body_length_;
	}

	const char* body() const
	{
		return data_ + header_length;
	}

	char* body()
	{
		return data_ + header_length;
	}

	std::size_t body_length() const
	{
		return body_length_;
	}

	void body_length(std::size_t new_length)
	{
		body_length_ = new_length;
		if (body_length_ > max_body_length)
			body_length_ = max_body_length;
	}

	bool decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_, header_length);
		body_length_ = std::atoi(header);
		if (body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		char header[header_length + 1] = "";
		std::sprintf(header, "%4d", static_cast<int>(body_length_));
		std::memcpy(data_, header, header_length);
	}

private:
	char data_[header_length + max_body_length];
	std::size_t body_length_;
};

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
	chat_client(asio::io_context& io_context,
		const tcp::resolver::results_type& endpoints)
		: io_context_(io_context),
		socket_(io_context)
	{
		do_connect(endpoints);
	}

	void write(const chat_message& msg)
	{
		asio::post(io_context_,
			[this, msg]()
			{
				bool write_in_progress = !write_msgs_.empty();
				write_msgs_.push_back(msg);
				if (!write_in_progress)
				{
					do_write();
				}
			});
	}

	void close()
	{
		asio::post(io_context_, [this]() { socket_.close(); });
	}

private:
	void do_connect(const tcp::resolver::results_type& endpoints)
	{
		asio::async_connect(socket_, endpoints,
			[this](std::error_code ec, tcp::endpoint)
			{
				if (!ec)
				{
					do_read_header();
				}
			});
	}

	void do_read_header()
	{
		asio::async_read(socket_,
			asio::buffer(read_msg_.data(), chat_message::header_length),
			[this](std::error_code ec, std::size_t /*length*/)
			{
				if (!ec && read_msg_.decode_header())
				{
					do_read_body();
				}
				else
				{
					socket_.close();
				}
			});
	}

	void do_read_body()
	{
		asio::async_read(socket_,
			asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this](std::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					std::cout.write(read_msg_.body(), read_msg_.body_length());
					std::cout << "\n";
					do_read_header();
				}
				else
				{
					socket_.close();
				}
			});
	}

	void do_write()
	{
		asio::async_write(socket_,
			asio::buffer(write_msgs_.front().data(),
				write_msgs_.front().length()),
			[this](std::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					write_msgs_.pop_front();
					if (!write_msgs_.empty())
					{
						do_write();
					}
				}
				else
				{
					socket_.close();
				}
			});
	}

private:
	asio::io_context& io_context_;
	tcp::socket socket_;
	chat_message read_msg_;
	chat_message_queue write_msgs_;
};

void do_stuff()
{
	printf("working!!\n");
}

int main(int argc, char* argv[])
{
	try
	{
		/*
	   if (argc != 3)
	   {
		 std::cerr << "Usage: chat_client <host> <port>\n";
		 return 1;
	   }
	   */
		asio::io_context io_context;

		tcp::resolver resolver(io_context);
		//auto endpoints = resolver.resolve(argv[1], argv[2]);
		auto endpoints = resolver.resolve("127.0.0.1", "9006");
		auto endpoints2 = resolver.resolve("127.0.0.1", "9007");
		//chat_client c(io_context, endpoints);

		std::thread t([&io_context]() { io_context.run(); });

		std::vector<tcp::socket> sockets;
		sockets.resize(200);

		for (int i = 0; i < 100; i++)
		{
			asio::async_connect(sockets[2*i + 0], endpoints,
				[](std::error_code ec, tcp::endpoint)
				{
					printf("conn! ");
				});

			asio::async_connect(sockets[2*i + 1], endpoints2,
				[](std::error_code ec, tcp::endpoint)
				{
					printf("conn2! ");
				});
		}


		char line[chat_message::max_body_length + 1];
		while (std::cin.getline(line, chat_message::max_body_length + 1))
		{
			do_stuff();
			chat_message msg;
			msg.body_length(std::strlen(line) + 1);
			std::memcpy(msg.body(), line, msg.body_length());
			msg.body()[msg.body_length() - 1] = '\n';
			msg.body()[msg.body_length()] = '\0';
			msg.encode_header();
			c.write(msg);
		}

		c.close();
		t.join();
	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
