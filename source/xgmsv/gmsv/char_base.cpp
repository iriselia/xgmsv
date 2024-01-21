
#include "char_base.h"
#include "buf.h"

Char* CHAR_chara;

static int      CHAR_charanum;
static int      CHAR_playernum;
static int      CHAR_petnum;
static int      CHAR_othersnum;


INLINE BOOL _CHAR_CHECKINDEX( char* file, int line, int index )
{
    if ( CHAR_charanum <= index || index < 0 ) {
        return FALSE;
    }
    if ( CHAR_chara[(index)].use == FALSE ) {
        return FALSE;
    }
    return TRUE;
}


#if 1 // def _FIX_SETWORKINT
INLINE int _CHAR_getWorkInt( char* file, int line, int index, CHAR_WORKDATAINT element )
#else
INLINE int _CHAR_getWorkInt( int index, CHAR_WORKDATAINT element )
#endif
{
    if ( CHAR_WORKBATTLEMODE > element || element >= CHAR_WORKDATAINTNUM ) {
        print( "err Get WorkInt element=%d :: file:%s %d!\n", element, file, line );
        return -1;
    }
    return CHAR_chara[index].workint[element];
}