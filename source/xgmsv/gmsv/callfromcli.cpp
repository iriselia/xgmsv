#include "gmsv/map/readmap.h"

#define CHECKFD	if( CONNECT_isCLI( fd ) == FALSE )return;	if( CONNECT_isLOGIN(fd) == FALSE )return;
#define CHECKFDANDTIME	if( CONNECT_isCLI(fd) == FALSE )return;	if( CONNECT_isLOGIN(fd) == FALSE )return;

void nrproto_M_recv( int fd, int fl, int x1, int y1, int x2, int y2 )
{
    /*
    char* mapdata;
    RECT_SA    seek = { x1,y1,x2 - x1,y2 - y1 }, ret;
    CHECKFD;

    mapdata = MAP_getdataFromRECT( fl, &seek, &ret );
    if ( mapdata ) {
        nrproto_M_send( fd, fl, ret.x, ret.y,
            ret.x + ret.width, ret.y + ret.height, mapdata );
    }
    */
}