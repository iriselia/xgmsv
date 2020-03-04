#include "common.h"



void assert_impl(char const* file, int line, char const* function, char const* message)
{
	fprintf(stderr, "\n%s:%i in %s ASSERTION FAILED:\n  %s\n", file, line, function, message);

	__debugbreak();
}

void assert_impl(char const* file, int line, char const* function, char const* message, char const* format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "\n%s:%i in %s ASSERTION FAILED:\n  %s ", file, line, function, message);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	fflush(stderr);
	va_end(args);

	__debugbreak();
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