#pragma once
//#include "common.h"
#include "gmsv/util.h"


struct MAP_Objlink
{
    int objindex;
    MAP_Objlink* next;
};
using OBJECT = MAP_Objlink*;

#define GET_OBJINDEX(x) ((x)->objindex)
#define NEXT_OBJECT(x) ((x)->next)

struct MAP_Map
{
	int     id;             /*  フロアID    */
    int     cfgid;
    int     seq;
	int     xsiz, ysiz;      /*  サイズ  */
    int     padding1; // -1
	char    string[64];     /*  表示    */
	unsigned short* tile;           /*  タイル  */
	unsigned short* obj;            /*  オブジェクト    */
	unsigned short* encount;
	MAP_Objlink** olink;
	int     padding2; // -1
	int     has_extended_data;
	unsigned short* hit;
	int     unknown1; // 0x3A408 or 0x3A409
	int     unknown2; // 0xE5 or 0xE4
	int     padding3; // -1
#ifdef _MAP_NOEXIT
	unsigned int startpoint;
	int MapType;
#endif
};

struct MAP_Dungeon // 104 bytes
{
	int     id;             /*  フロアID    */
	int     cfgid;
	int     seq;
};

enum MAP_DATAINT
{
    MAP_WALKABLE,           /*
                             * パーツの場合
                             * 歩けなくてタイルを見ない 0
                             * 歩けてタイルを見る 1
                             * 歩けてタイルを見ない 2
                             */
    MAP_HAVEHEIGHT,         /*  高さがあったら非0 なかったら0   */
    MAP_DEFENCE,            /*  守れるなら、守備力、0 以下なら守れない  */

    MAP_INTODAMAGE,         /*  そこに入った時にHPに足する  */
    MAP_OUTOFDAMAGE,        /*  そこから出た時にHPに足する  */

    MAP_SETED_BATTLEMAP,    /* バトルマップ設定値 */
    MAP_BATTLEMAP,			/* バトルマップの番号 １*/
    MAP_BATTLEMAP2,			/* バトルマップの番号 ２*/
    MAP_BATTLEMAP3,			/* バトルマップの番号 ３*/

    /* 以下はＬＳ２で使っていてＳＡでは使わない(打ち合わせにだす) */

    MAP_INTODARKNESS,       /*  入った時の暗闇    */
    MAP_INTOCONFUSION,      /*  入った時の混乱    */

    MAP_OUTOFPOISON,         /*  入った時の毒    */
    MAP_OUTOFPARALYSIS,      /*  入った時のしびれ    */
    MAP_OUTOFSILENCE,        /*  入った時の沈黙  */
    MAP_OUTOFSTONE,          /*  入った時の石化    */
    MAP_OUTOFDARKNESS,       /*  入った時の暗闇    */
    MAP_OUTOFCONFUSION,      /*  入った時の混乱    */

    MAP_DATAINT_NUM,
};

enum MAP_DATACHAR
{
	MAP_DATACHAR_NUM,
};

struct MAP_ImageData
{
	int     data[MAP_DATAINT_NUM];
	STRING32    string[MAP_DATACHAR_NUM];
};

bool MAP_initReadMap(char* maptilefile, char* mapdir);

INLINE int MAP_getfloorIndex( int floorid );
int MAP_getfloorX( int floorid );
int MAP_getfloorY( int floorid );

char* MAP_getdataFromRECT(int mapid, int floor, RECT_SA* seekr, RECT_SA* realr );
BOOL MAP_getTileAndObjData( int mapid, int floorid, int fx, int fy, int* tile, int* obj );


#define		MAP_getTopObj( map, fl, x, y)	_MAP_getTopObj( __FILE__, __LINE__, map, fl, x, y)
MAP_Objlink* _MAP_getTopObj( char* file, int line, int floor, int x, int y );