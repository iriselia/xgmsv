#pragma once

namespace server
{
	namespace core
	{
		namespace this_thread
		{
			void assign_main_thread();
			std::thread::id get_id();
			void set_debug_name(const std::string& name);
			std::string get_debug_name();
		};


		struct main_thread_info
		{
			std::thread::id id;
			std::string name = "main_thread";
		};

		struct worker_thread : public std::thread
		{
			std::thread::id id;
			std::string name;


			template<typename lambda>
			worker_thread(std::string&& new_name, lambda function) : id(this->get_id()), name(std::move(new_name)), std::thread(function)
			{
				set_thread_name(*this, name.c_str());
			}

		};

		extern main_thread_info main_thread;
		extern std::vector<worker_thread> worker_threads;

		template<typename lambda>
		void spawn_worker_threads(uint32 worker_count, lambda& worker_main)
		{
			if (worker_count == 0)
			{
				worker_count = std::thread::hardware_concurrency();
			}

			worker_threads.reserve(worker_count);
			for (uint32 i = 0; i < worker_count; i++)
			{
				worker_threads.emplace_back(std::string("worker_thread_") + std::to_string(i), [i, &worker_main]()
				{
					worker_main();

					printf("%s exited.\n", worker_threads[i].name.c_str());
				});
			}
		}

		void purge_worker_threads();
	}
}

#ifdef _WIN32
namespace std { class thread; }
void set_thread_name(uint32_t dwThreadID, const char* threadName);
//*
void set_thread_name(const char* threadName);

void set_thread_name(std::thread& thread, const char* threadName);
//*/
#else
void set_thread_name(std::thread* thread, const char* threadName)
{
	auto handle = thread->native_handle();
	pthread_setname_np(handle, threadName);
}


#include <sys/prctl.h>
void set_thread_name(const char* threadName)
{
	prctl(PR_SET_NAME, threadName, 0, 0, 0);
}

#endif