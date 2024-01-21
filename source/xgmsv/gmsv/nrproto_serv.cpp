
#include "crossgate/xg_session.h"

#include "version.h"
//#include "autil.h"
#include "nrproto_serv.h"
//#include "common.h"	// for StoneAge
//#include "log.h"	// for StoneAge
#include "char.h"
#include "net.h"


#include <stdio.h>
#include <time.h>

namespace crossgate
{
    nrproto_write_t nrproto;
    uint64_t nrproto_buffer_size;
    uint8_t* nrproto_M_buffer;

    int nrproto_CreateHeader(uint8_t* s, int a2)
    {
        return sprintf((char*)s, "%s ", (const char*)a2);
    }

    void nrproto_M_send(xg_session* session, int map, int fl, int x1, int y1, int x2, int y2, char* data)
    {
        /*
        nrproto_CreateHeader(nrproto_M_buffer, 2160 * PacketVer + 0x8258368);


        char buffer[65500];
        int checksum = 0;

        strcpy(buffer, "");

        CONNECT_getCdkey(fd, PersonalKey, 4096);
        strcat(PersonalKey, _RUNNING_KEY);

        checksum += util_mkint(buffer, fl);
        checksum += util_mkint(buffer, x1);
        checksum += util_mkint(buffer, y1);
        checksum += util_mkint(buffer, x2);
        checksum += util_mkint(buffer, y2);
        checksum += util_mkstring(buffer, data);
        util_mkint(buffer, checksum);
        util_SendMesg(fd, LSSPROTO_M_SEND, buffer);
        */
    }
}
