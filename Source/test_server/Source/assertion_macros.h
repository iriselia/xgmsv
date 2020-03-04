//#pragma once

#if ENABLED(DEBUG_PROGRAM)
void assert_impl(char const* file, int line, char const* function, char const* message);

void assert_impl(char const* file, int line, char const* function, char const* message, char const* format, ...);

#ifdef assert
#undef assert
#endif
#define assert(cond, ...) { if (!(cond)) assert_impl(__FILE__, __LINE__, __FUNCTION__, #cond, ##__VA_ARGS__); }
#else
#define assert(...)
#endif