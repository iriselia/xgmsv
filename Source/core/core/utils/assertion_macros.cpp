#include <stdarg.h>
#include <cstdio>

#include "core/utils/assertion_macros.h"

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
