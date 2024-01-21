#pragma once
//#include "stdarg.h"
//#include "stdio.h"
/*
int my_printf( const char* fmt, ... ) //The custom function is the same as the snprintf() function provided by the system.
{
	va_list ap;
	int n = 0;
	va_start( ap, fmt ); //Get a list of variable parameters
	n = vprintf( fmt, ap ); //Write string s
	va_end( ap ); //Release resources
	return n; //Returns the number of characters written
}
*/

#define xg_log(x, ...) printf("[+] "); printf(x, ##__VA_ARGS__)
#define xg_warning(x, ...) printf("[!] "); printf(x, ##__VA_ARGS__)
#define xg_error(x, ...) printf("[-] "); printf(x, ##__VA_ARGS__)