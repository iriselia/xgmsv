#define __NET_C__ 
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if PLATFORM_WINDOWS
#include <WinSock2.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>     
#endif

#include <sys/types.h>
#include <time.h>
//ttom+1
#include <sys/timeb.h>
#include "net.h"


#define MIN(x,y)     ( ( (x) < (y) ) ? (x) : (y) )
#define IS_2BYTEWORD( _a_ ) ( (char)(0x80) <= (_a_) && (_a_) <= (char)(0xFF) )

#ifdef	_NEW_SERVER_
BOOL bNewServer = TRUE;
#else
BOOL bNewServer = FALSE;
#endif 

// Nuke +1 0901: For state monitor
int StateTable[WHILESAVEWAIT + 1];

int ITEM_getRatio();
int CHAR_players();
#define CONO_CHECK_LOGIN 0x001
#define CONO_CHECK_ITEM 0x010
#define CONO_CHECK_PET 0x100
int cono_check = 0x111;

int     AC_WBSIZE = (65536 * 16);
//ttom+1 for the performatce
static unsigned int MAX_item_use = 0;
int i_shutdown_time = 0;//ttom
BOOL b_first_shutdown = FALSE;//ttom

int mfdfulll = 0;

/*------------------------------------------------------------
 * サーバの状態
 ------------------------------------------------------------*/
typedef struct tag_serverState
{
    BOOL            acceptmore;     /*  1だったら、accept したあと
                                        ちょくで、close する */
    unsigned int    fdid;           /*  fd の通り値 */
    unsigned int    closeallsocketnum;  /*   closeallsocket 中の残りの
                                             数*/
    int				shutdown;		/*  サーバをshutdownするモード
                                     *	0:通常 それ以外:シャットシャットモード
                                     * モードになった時間が入ってる。
                                     */
    int				dsptime;		/* shutdown モードのワーク変数*/
    int				limittime;		/* これも */
}ServerState;

typedef struct tagCONNECT
{
    BOOL  use;

#if USE_MTIO
#define MTIO_FIXED_BUFSIZE (65536)
    char rb[MTIO_FIXED_BUFSIZE];
    int rbuse;
    char wb[MTIO_FIXED_BUFSIZE];
    int wbuse;
    pthread_mutex_t mutex;    /* Connectをいじるときにつかうロック */
#else
    char* rb;
    int rbuse;
    char* wb;
    int wbuse;
    int check_rb_oneline_b;
    int check_rb_time;
#endif    

    struct sockaddr_in sin; /* 接続先のアドレス */
    ConnectType ctype;       /* コネクションの種類 */

    char    cdkey[CDKEYLEN];    /* CDKEY */
    char    passwd[PASSWDLEN];  /* パスワード */
    LoginType state;        /* 現在のログイン状態 */
    int		nstatecount;
    char    charname[CHARNAMELEN];  /* ログイン中のキャラ名 */
    int     charaindex;     /* char配列へのインデックス。
                             * ログイン後に値が設定される。-1がデフォルト
                             * 配列にない時。
                             */
    char    CAbuf[2048];         /*  CA() を圧縮する為のバッファ */
    int     CAbufsiz;       /*  CAbuf のサイズ  */
    struct timeval  lastCAsendtime;     /*最後にCAを送った時間 */

    char    CDbuf[2048];         /*  CD() を圧縮する為のバッファ */
    int     CDbufsiz;       /*  CDbuf のサイズ  */
    struct timeval  lastCDsendtime;     /*最後にCDを送った時間 */

    struct timeval  lastCharSaveTime; /* 最後にキャラデータをセーブした時間 */

    struct timeval  lastprocesstime;    /* 最後にプロトコルを処理した時間*/
    struct timeval  lastreadtime;       /* 最後にreadした時間。上とはちがう*/

    // Nuke start 08/27 : For acceleration avoidance
    // WALK_TOLERANCE: Permit n W messages in a second (3: is the most restricted)
#define WALK_TOLERANCE 4
#define WALK_SPOOL 5
#define WALK_RESTORE 100
    unsigned int        Walktime;
    unsigned int        lastWalktime;
    unsigned int        Walkcount;
    int Walkspool;      // For walk burst after release key F10
    int Walkrestore;
    // B3_TOLERANCE: Time distance between recently 3 B message (8: is the latgest)
    // BEO_TOLERANCE: Time distance between the lastmost B and EO (5: is the largest)
#define B3_TOLERANCE 5
#define BEO_TOLERANCE 3
#define BEO_SPOOL 10
#define BEO_RESTORE 100
    unsigned int        Btime;
    unsigned int        lastBtime;
    unsigned int        lastlastBtime;
    unsigned int        EOtime;

#ifdef _BATTLE_TIMESPEED
    //	unsigned int		DefBtime;
    int		BDTime;
    int		CBTime;
#endif

#ifdef _TYPE_TOXICATION
    int		toxication;
#endif

#ifdef _CHECK_GAMESPEED
    int		gamespeed;
#endif
#ifdef _ITEM_ADDEXP	//vincent 經驗提昇
    int		EDTime;
#endif
    //    unsigned int      BEO;
    int BEOspool;
    int BEOrestore;
    // Nuke 0219: Avoid cheating
    int die;
    // Nuke end
    // Nuke 0310
    int credit;
    int fcold;
    // Nuke 0406: New Flow Control
    int nu;
    int nu_decrease;
    int ke;
    // Nuke 1213: Flow Control 2
    int packetin;

    // Nuke 0624: Avoid Null Connection
    unsigned int cotime;
    // Nuke 0626: For no enemy
    int noenemy;
    // Arminius 7.2: Ra's amulet
    int eqnoenemy;
#ifdef _Item_MoonAct
    int eqrandenemy;
#endif

#ifdef _CHIKULA_STONE
    int chistone;
#endif
    // Arminius 7.31: cursed stone
    int stayencount;

    /* close するべきかどうかの記憶 */
#if USE_MTIO    
    int     closed;
#endif    
    int		battlecharaindex[CONNECT_WINDOWBUFSIZE];
    int		duelcharaindex[CONNECT_WINDOWBUFSIZE];
    int		tradecardcharaindex[CONNECT_WINDOWBUFSIZE];
    int		joinpartycharaindex[CONNECT_WINDOWBUFSIZE];

    // CoolFish: Trade 2001/4/18
    int		tradecharaindex[CONNECT_WINDOWBUFSIZE];
    int     errornum;
    int     fdid;

    int     close_request; //the second have this

    int appendwb_overflow_flag;  /* 1回でもappendWbが失敗したら1にする */
    //ttom+1 avoidance the watch the battle be kept out
    BOOL in_watch_mode;
    BOOL b_shut_up;//for avoid the user wash the screen
    BOOL b_pass;      //for avoid the unlimited area
    struct timeval Wtime;
    struct timeval WLtime;
    BOOL b_first_warp;
    int  state_trans;

    // CoolFish: Trade 2001/4/18
    char TradeTmp[256];

#ifdef _ITEM_PILEFORTRADE
    int tradelist;
#endif
    // Shan Recvdata Time
    struct timeval lastrecvtime;      // 'FM' Stream Control time
    struct timeval lastrecvtime_d;    // DENGON Talk Control time

    // Arminius: 6.22 encounter
    int CEP;	// Current Encounter Probability
    // Arminius 7.12 login announce
    int announced;

    // shan battle delay time 2001/12/26
    struct timeval battle_recvtime;
#ifdef _NO_WARP
    // shan hjj add Begin 
    int seqno;
    int selectbutton;
    // shan End
#endif	
    BOOL confirm_key;    // shan  trade(DoubleCheck)

#ifdef _BLACK_MARKET
    int  BMSellList[12];
#endif
}
CONNECT;

CONNECT* Connect;      /*コネクション保ち用*/


/* 関数の先頭につけてわかるようにするだけのマクロ */
#define SINGLETHREAD
#define MUTLITHREAD
#define ANYTHREAD

ServerState servstate;
#if USE_MTIO
pthread_mutex_t MTIO_servstate_m;     /* servstate のロック */
#define SERVSTATE_LOCK() pthread_mutex_lock( &MTIO_servstate_m );
#define SERVSTATE_UNLOCK() pthread_mutex_unlock( &MTIO_servstate_m );

#if 1
#define CONNECT_LOCK_ARG2(i,j) fprintf(stderr,"LO T:%d(%d:%d) %s %d", (int)pthread_self(), i,j, __FILE__,__LINE__ );pthread_mutex_lock( &Connect[i].mutex );fprintf( stderr, "CK T:%d(%d:%d)\n" , (int)pthread_self(), i,j );
#define CONNECT_UNLOCK_ARG2(i,j) fprintf(stderr,"UNLO T:%d(%d:%d) %s %d", (int)pthread_self(), i,j, __FILE__,__LINE__ );pthread_mutex_unlock( &Connect[i].mutex );fprintf( stderr, "CK T:%d(%d:%d)\n",(int)pthread_self(), i,j);
#define CONNECT_LOCK(i) fprintf(stderr,"LO T:%d(%d) %s %d", (int)pthread_self(), i, __FILE__,__LINE__ );pthread_mutex_lock( &Connect[i].mutex );fprintf( stderr, "CK T:%d(%d)\n" ,(int)pthread_self(), i );
#define CONNECT_UNLOCK(i) fprintf(stderr,"UNLO T:%d(%d) %s %d", (int)pthread_self(), i, __FILE__,__LINE__ );pthread_mutex_unlock( &Connect[i].mutex );fprintf( stderr, "CK T:%d(%d)\n",(int)pthread_self(), i);
/* 上記のログはめちゃくちゃディスクを食うのでスリープをいれたり */
#define MTIO_DEBUG_LOG_REDUCE 1
#else
#define CONNECT_LOCK_ARG2(i,j) pthread_mutex_lock( &Connect[i].mutex );
#define CONNECT_UNLOCK_ARG2(i,j) pthread_mutex_unlock( &Connect[i].mutex );
#define CONNECT_LOCK(i) pthread_mutex_lock( &Connect[i].mutex );
#define CONNECT_UNLOCK(i) pthread_mutex_unlock( &Connect[i].mutex );
#define MTIO_DEBUG_LOG_REDUCE 0
#endif

#else
#define SERVSTATE_LOCK()
#define SERVSTATE_UNLOCK()
#define CONNECT_LOCK_ARG2(i,j) 
#define CONNECT_UNLOCK_ARG2(i,j)
#define CONNECT_LOCK(i) 
#define CONNECT_UNLOCK(i)
#endif

/*------------------------------------------------------------
 * fdに割り当てられた接続がクライアントだと、BOOLを返す
 * 細いエラーチェックはしない。
 * 引数
 *  fd  int     ファイルディスクリプタ
 ------------------------------------------------------------*/
ANYTHREAD BOOL CONNECT_isCLI( int fd )
{
    int a;
    CONNECT_LOCK( fd );
    a = (Connect[fd].ctype == CLI ? TRUE : FALSE);
    CONNECT_UNLOCK( fd );
    return a;
}


/*------------------------------------------------------------
 * ログインしている状態か
 * どうかを返す
 * 引数
 *  fd  int     ファイルディスクリプタ
 ------------------------------------------------------------*/
ANYTHREAD BOOL CONNECT_isLOGIN( int fd )
{
    int a;
    CONNECT_LOCK( fd );
    a = (Connect[fd].state == LOGIN ? TRUE : FALSE);
    CONNECT_UNLOCK( fd );
    return a;
}