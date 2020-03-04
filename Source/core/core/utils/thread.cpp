#include <string>
#include <vector>
#include <thread>

#include "core/utils/types.h"
#include "core/utils/assertion_macros.h"

#include "core/utils/thread.h"


namespace server
{
	namespace core
	{
		main_thread_info main_thread;
		std::vector<worker_thread> worker_threads;

		void purge_worker_threads()
		{
			for (auto& worker : worker_threads)
			{
				if (worker.joinable())
				{
					worker.join();
				}
			}
			worker_threads.clear();
		}

		namespace this_thread
		{
			void assign_main_thread()
			{
				main_thread.id = std::this_thread::get_id();
				this_thread::set_debug_name(main_thread.name.c_str());
			}

			std::thread::id this_thread::get_id()
			{
				return std::this_thread::get_id();
			}

			void set_debug_name(const std::string& new_name)
			{
				if (std::this_thread::get_id() == main_thread.id)
				{
					main_thread.name = new_name;
					set_thread_name(new_name.c_str());
					return;
				}

				for (auto& i : worker_threads)
				{
					if (std::this_thread::get_id() == i.id)
					{
						i.name = new_name;
						set_thread_name(new_name.c_str());
						return;
					}
				}

				assert(false);
				return;
			}

			std::string get_debug_name()
			{
				if (std::this_thread::get_id() == main_thread.id)
				{
					return main_thread.name;
				}

				for (auto& i : worker_threads)
				{
					if (std::this_thread::get_id() == i.id)
					{
						return i.name;
					}
				}

				assert(false);
				return std::string();
			}
		}
	}
}


#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <thread>

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void set_thread_name(uint32_t dwThreadID, const char* threadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

void set_thread_name(const char* threadName)
{
	set_thread_name(GetCurrentThreadId(), threadName);
}

void set_thread_name(std::thread& thread, const char* threadName)
{
	DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread.native_handle()));
	set_thread_name(threadId, threadName);
}

#endif