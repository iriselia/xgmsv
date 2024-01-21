#pragma once
#include <stdio.h>
#include <cstdint>

#include "crossgate/globals.h"

#define PacketVer 2

using nrproto_write_t = size_t (*)(FILE*, const void*, size_t);
extern nrproto_write_t nrproto;
extern uint8_t* nrproto_M_buffer;

size_t nrproto_default_write_wrap(FILE* fd, const void* buf, size_t n)
{
    return fwrite(buf, sizeof(uint8_t), n, fd );
}

int nrproto_AllocateCommonWork(uint32_t buffer_size)
{
    nrproto_M_buffer = (uint8_t*)malloc(buffer_size);
    assert(nrproto_M_buffer);
    memset(nrproto_M_buffer, 0, buffer_size);
}

int nrproto_InitServer(nrproto_write_t write_func, uint32_t buffer_size)
{
    if ( write_func )
    {
        nrproto = write_func;
    }
    else
    {
        nrproto = nrproto_default_write_wrap;
    }

    nrproto_AllocateCommonWork(buffer_size);
    //nrproto_stringwrapper = malloc(18 * sizeof(char*));
    /*
    if ( !nrproto_stringwrapper )
    {
        return -1;
    }
    for ( int i = 0; i < 18; i++ )
    {
        nrproto_stringwrapper[i] = malloc(buffer_size);
        if ( !nrproto_stringwrapper[i] )
        {
            free(nrproto_stringwrapper);
            return -1;
        }
    }
    */

}
void nrproto_M_send( int fd, int fl, int x1, int y1, int x2, int y2, char* data );
