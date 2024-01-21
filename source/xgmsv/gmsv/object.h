#pragma once

#include "common.h"

typedef enum
{
    OBJTYPE_NOUSE,      /*  このインデックスは使っていないという事を示す*/
    OBJTYPE_CHARA,      /*  キャラクタ CHAR_charaへのインデックス   */
    OBJTYPE_ITEM,       /*  アイテム    ITEM_item へのインデックス  */
    OBJTYPE_GOLD,       /*  ゴールド    お金の量がindexに入っている */
    OBJTYPE_NPCSCHARA,
    OBJTYPE_WARPPOINT,
    OBJTYPE_NUM
}OBJTYPE;

struct Object
{
    OBJTYPE    type;
    int        index;
    int        mapid;
    int        x;
    int        y;
    int        floor;
    int        warp_mapid;
    int        warp_floorid;
    int        warp_x;
    int        warp_y;
    int        time;
    Object*    next;
    int        objindex;
    //char		objname[256];
    //int			npcsindex;
    //int			dir;
    //int			imagenum;
    //int			chartype;
};


BOOL CHECKOBJECT( int index );
BOOL CHECKOBJECTUSE( int index );
BOOL initObjectArray( int num );
BOOL endObjectArray( void );

INLINE int _initObjectOne( char* file, int line, Object* ob );
#define initObjectOne( ob ) _initObjectOne( __FILE__, __LINE__, ob)

void endObjectOne( int index );
int initObjectFromObjectMember( OBJTYPE type, int index, int x, int y, int floor );

INLINE int OBJECT_getType( int index );
INLINE int OBJECT_setType( int index, int newvalue );
INLINE int OBJECT_getFloor( int index );
INLINE int OBJECT_setFloor( int index, int newvalue );
INLINE int OBJECT_getX( int index );
INLINE int OBJECT_setX( int index, int newvalue );
INLINE int OBJECT_getY( int index );
INLINE int OBJECT_setY( int index, int newvalue );
INLINE int OBJECT_getIndex( int index );
INLINE int OBJECT_setIndex( int index, int newvalue );
INLINE int OBJECT_getNum( void );

#ifdef _NPCSERVER_NEW
INLINE int OBJECT_getNpcIndex( int index );
INLINE char* OBJECT_getName( int index );
INLINE int OBJECT_getNpcImage( int index );
INLINE int OBJECT_getNpcdir( int index );
#endif
INLINE int OBJECT_getchartype( int index );
INLINE void OBJECT_setchartype( int index, int flg );

#ifndef _SIMPLIFY_ITEMSTRING
BOOL storeObjects( char* dirname );
BOOL restoreObjects( char* dirname );
#endif

int searchObjectFromCharaIndex( int index );
