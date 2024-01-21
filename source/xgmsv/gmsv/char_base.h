#pragma once

#include "gmsv/util.h"

#define NONCHAR_DELIMITER "|"

typedef enum
{
    CHAR_HEAD,
    CHAR_BODY,
    CHAR_ARM,
    CHAR_DECORATION1,
    CHAR_DECORATION2,
#ifdef _ITEM_EQUITSPACE
    CHAR_EQBELT,	//腰帶
    CHAR_EQSHIELD,	//盾
    CHAR_EQSHOES,	//鞋子
#endif
#ifdef _EQUIT_NEWGLOVE
    CHAR_EQGLOVE,
#endif
    CHAR_EQUIPPLACENUM,
}CHAR_EquipPlace;

typedef enum
{
    CHAR_EVENT_NONE,
    CHAR_EVENT_NPC,
    CHAR_EVENT_ENEMY,
    CHAR_EVENT_WARP,
    CHAR_EVENT_DOOR,
    CHAR_EVENT_ALTERRATIVE,
    CHAR_EVENT_WARP_MORNING,
    CHAR_EVENT_WARP_NOON,
    CHAR_EVENT_WARP_NIGHT,
    CHAR_EVENT_9,
    CHAR_EVENT_10,
    CHAR_EVENTNUM,
}CHAR_EVENT;

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
#define CHAR_SKILLMAXHAVE   26		// 職業技能上限
#else
#define CHAR_SKILLMAXHAVE   5
#endif

#define CHAR_MAXITEMNUM  15
#define CHAR_STARTITEMARRAY     CHAR_EQUIPPLACENUM
#define CHAR_MAXITEMHAVE (CHAR_STARTITEMARRAY+CHAR_MAXITEMNUM)

typedef enum
{
    CHAR_DATAPLACENUMBER,
    CHAR_BASEIMAGENUMBER,
    CHAR_BASEBASEIMAGENUMBER,
    CHAR_FACEIMAGENUMBER,
    CHAR_FLOOR,
    CHAR_X,
    CHAR_Y,
    CHAR_DIR,
    CHAR_LV,
    CHAR_GOLD,
    CHAR_HP,
    CHAR_MP,
    CHAR_MAXMP,
    CHAR_VITAL,
    CHAR_STR,
    CHAR_TOUGH,
    CHAR_DEX,
    CHAR_CHARM,
    CHAR_LUCK,
    CHAR_EARTHAT,
    CHAR_WATERAT,
    CHAR_FIREAT,
    CHAR_WINDAT,
    CHAR_DEFAULTPET,
    CHAR_CRITIAL,
    CHAR_COUNTER,
    CHAR_RARE,
    CHAR_RADARSTRENGTH,
    CHAR_CHATVOLUME,
    CHAR_MERCHANTLEVEL,
    CHAR_HEALERLEVEL,
    CHAR_SAVEPOINT = CHAR_HEALERLEVEL,
    CHAR_DETERMINEITEM,
    CHAR_INDEXOFEQTITLE,
    CHAR_POISON,
    CHAR_PARALYSIS,
    CHAR_SLEEP,
    CHAR_STONE,
    CHAR_DRUNK,
    CHAR_CONFUSION,
    CHAR_LOGINCOUNT,
    CHAR_NPCCREATEINDEX = CHAR_LOGINCOUNT,
    CHAR_DEADCOUNT,
    CHAR_WALKCOUNT,
    CHAR_TALKCOUNT,
    CHAR_DAMAGECOUNT,
    CHAR_GETPETCOUNT,
    CHAR_KILLPETCOUNT,
    CHAR_DEADPETCOUNT,
    CHAR_SENDMAILCOUNT,
    CHAR_MERGEITEMCOUNT,
    CHAR_DUELBATTLECOUNT,
    CHAR_DUELWINCOUNT,
    CHAR_DUELLOSECOUNT,
    CHAR_DUELSTWINCOUNT,
    CHAR_DUELMAXSTWINCOUNT,
    CHAR_WHICHTYPE,
    CHAR_WALKINTERVAL,
    CHAR_LOOPINTERVAL,
#ifdef _NEWOPEN_MAXEXP
    CHAR_OLDEXP,
    CHAR_EXP,
#else
    CHAR_EXP,
#endif
    CHAR_LASTTALKELDER,
    CHAR_SKILLUPPOINT,
    CHAR_LEVELUPPOINT,
    CHAR_IMAGETYPE,
    CHAR_NAMECOLOR,
    CHAR_POPUPNAMECOLOR,
    CHAR_LASTTIMESETLUCK,
    CHAR_DUELPOINT,

                        CHAR_DATAINTNUM,

}CHAR_DATAINT;

typedef enum {
    CHAR_MODAI = CHAR_CHARM,    		/*  ペット毎のの忠誠心修正値    */
    CHAR_VARIABLEAI = CHAR_LUCK,			/*  ペットの忠誠心変化値（１００倍した値で持っておく） */
    CHAR_SLOT = CHAR_DEFAULTPET,		/* 石盤をセット出来るスロット数（ペットが使用）*/
    CHAR_PETGETLV = CHAR_CHATVOLUME,		/* ゲットされたレベル（ペットのみ使用 ）*/
    CHAR_PUTPETTIME = CHAR_LOGINCOUNT,		/*  ペットが地面に置かれた時間 */
    CHAR_MAILMODE = CHAR_RADARSTRENGTH,	/* （ペットのみ使用）メール送信状態 */
    CHAR_ALLOCPOINT = CHAR_LEVELUPPOINT,	/* レベルアップ時の割り振りポイント(ペット用) */
    CHAR_PETMAILBUFINDEX = CHAR_GETPETCOUNT, 	/* ペットメール(offmsg)へのindex */
    CHAR_PETMAILIDLETIME = CHAR_SENDMAILCOUNT,	/* ペットメールの為の時間など */
    CHAR_PETMAILFROMFLOOR = CHAR_DUELBATTLECOUNT,	/* メールを送った時のフロア */
    CHAR_PETMAILFROMX = CHAR_DUELWINCOUNT,	/* メールを送った時のＸ座標 */
    CHAR_PETMAILFROMY = CHAR_DUELLOSECOUNT,	/* メールを送った時のＹ座標 */
    CHAR_PETMAILEFFECT = CHAR_DUELSTWINCOUNT,	/* ペットメールの出現のエフェクト番号 */
    CHAR_PETSENDMAILCOUNT = CHAR_TALKCOUNT,		/* ペットがメールを運んだ回数 */
    CHAR_PETRANK = CHAR_LASTTIMESETLUCK, /* ペットのランク(ペット用）１〜６。０はエラー用 */
    CHAR_PETID = CHAR_DUELMAXSTWINCOUNT,/* ペットのテンプレートのＩＤ */
    //CHAR_PETFAMILY = CHAR_FMLEADERFLAG,		// CoolFish: Family 判斷是否為家族守護獸
}CHAR_PET;


typedef enum
{
    CHAR_NAME,
    CHAR_OWNTITLE,
    CHAR_USERPETNAME = CHAR_OWNTITLE,
    CHAR_NPCARGUMENT,
    CHAR_CDKEY = CHAR_NPCARGUMENT,
    CHAR_OWNERCDKEY,
    CHAR_OWNERCHARANAME,
    CHAR_FMNAME,
#ifdef _UNIQUE_P_I
    CHAR_UNIQUECODE,
#endif
#ifdef _ACTION_GMQUE
    CHAR_GMQUESTR1,
#endif
#ifdef _GM_IDENTIFY
    CHAR_GMIDENTIFY,
#endif
#ifdef _TEACHER_SYSTEM
    CHAR_TEACHER_ID,		// 導師帳號
    CHAR_TEACHER_NAME,	// 導師名字
#endif
    CHAR_DATACHARNUM,
}CHAR_DATACHAR;

typedef enum
{
    CHAR_ISATTACK,
    CHAR_ISATTACKED,
    CHAR_ISOVER,
    CHAR_ISOVERED,
    CHAR_HAVEHEIGHT,
    CHAR_ISVISIBLE,
    CHAR_ISTRANSPARENT,
    CHAR_ISFLYING,
    CHAR_ISDIE,
    CHAR_ISBIG,
    CHAR_ISSHOWBATTLEMSG,
    CHAR_ISPARTY,
    CHAR_ISWARP,
    CHAR_ISDUEL,
    CHAR_ISPARTYCHAT,
    CHAR_ISTRADECARD,
    CHAR_ISTRADE,
#ifdef _CHANNEL_MODIFY
    CHAR_ISTELL,				//密語頻道開關
    CHAR_ISFM,					//家族頻道開關
    CHAR_ISOCC,					//職業頻道開關
    CHAR_ISSAVE,				//對話儲存開關
    CHAR_ISCHAT,				//聊天室
#endif
#ifdef _AUCPROTOCOL				// (不可開) Syu ADD 拍賣頻道開關Protocol
    CHAR_ISAUC,
#endif
    CHAR_FLGNUM,
}CHAR_DATAFLG;

typedef enum
{
    CHAR_WORKBATTLEMODE,
    CHAR_WORKBATTLEINDEX,
    CHAR_WORKBATTLESIDE,
    CHAR_WORKBATTLECOM1,
    CHAR_WORKBATTLECOM2,
    CHAR_WORKBATTLECOM3,
#ifdef _PSKILL_MDFYATTACK
    CHAR_WORKBATTLECOM4,
#endif
    CHAR_WORKBATTLEFLG,
    CHAR_WORKBATTLEWATCH,

    CHAR_WORKFIXEARTHAT,
    CHAR_WORKFIXWATERAT,
    CHAR_WORKFIXFIREAT,
    CHAR_WORKFIXWINDAT,
#ifdef _PROFESSION_ADDSKILL
    CHAR_WORKFIXEARTHAT_BOUNDARY, //地結界  左16位元存放加強百分比 右16位元存放回合數
    CHAR_WORKFIXWATERAT_BOUNDARY, //水結界
    CHAR_WORKFIXFIREAT_BOUNDARY,  //火結界
    CHAR_WORKFIXWINDAT_BOUNDARY,  //風結界
#endif
    CHAR_WORKMAXHP,
    CHAR_WORKMAXMP,

    CHAR_WORKATTACKPOWER,
    CHAR_WORKDEFENCEPOWER,
    CHAR_WORKQUICK,

    CHAR_WORKFIXVITAL,
    CHAR_WORKFIXSTR,
    CHAR_WORKFIXTOUGH,
    CHAR_WORKFIXDEX,

#ifdef _ITEMSET5_TXT
    CHAR_WORKFIXARRANGE,
    CHAR_WORKARRANGEPOWER,
    CHAR_WORKFIXSEQUENCE,
    CHAR_WORKSEQUENCEPOWER,
    CHAR_WORKATTACHPILE,
    CHAR_WORKHITRIGHT,	//額外命中
#endif
#ifdef _ITEMSET6_TXT
    CHAR_WORKNEGLECTGUARD,
#endif
    CHAR_WORKMODATTACK,
    CHAR_WORKMODDEFENCE,
    CHAR_WORKMODQUICK,

    CHAR_WORKMODCAPTURE,
    CHAR_WORKMODCHARM,

    CHAR_WORKFIXCHARM,
    CHAR_WORKFIXLUCK,
    CHAR_WORKFIXAI,
    CHAR_WORKFIXAVOID,

    CHAR_WORKDAMAGEABSROB,
    CHAR_WORKDAMAGEREFLEC,
    CHAR_WORKDAMAGEVANISH,
    CHAR_WORKPOISON,
    CHAR_WORKPARALYSIS,
    CHAR_WORKSLEEP,
    CHAR_WORKSTONE,
    CHAR_WORKDRUNK,
    CHAR_WORKCONFUSION,
    CHAR_WORKWEAKEN,             //虛弱  vincent add  _MAGIC_WEAKEN
    CHAR_WORKDEEPPOISON,         //劇毒  vincent add _MAGIC_DEEPPOISON
    CHAR_WORKBARRIER,            //魔障  vincent add _MAGIC_BARRIER
    CHAR_WORKNOCAST,             //沉默  vincent add _MAGIC_NOCASTっ
    CHAR_WORKMODPOISON,
    CHAR_WORKMODPARALYSIS,
    CHAR_WORKMODSLEEP,
    CHAR_WORKMODSTONE,
    CHAR_WORKMODDRUNK,
    CHAR_WORKMODCONFUSION,
    CHAR_WORKMODWEAKEN,
    CHAR_WORKMODDEEPPOISON,
    CHAR_WORKMODBARRIER,		//魔障
    CHAR_WORKMODNOCAST,			//沉默
    CHAR_WORKMODCRITICAL,		//一擊必殺
    CHAR_WORKULTIMATE,
    CHAR_WORKPARTYMODE,
    CHAR_WORKTRADEMODE,         /* 交易模式
                                 * 0: CHAR_TRADE_FREE   :可交易
                                 * 1: CHAR_TRADE_SENDING:交易請求中
                                 * 2: CHAR_TRADE_TRADING:交易中
                                 * 3: CHAR_TRADE_LOCK   :交易鎖定中
                                */
                                CHAR_WORKPARTYINDEX1,
                                CHAR_WORKPARTYINDEX2,
                                CHAR_WORKPARTYINDEX3,
                                CHAR_WORKPARTYINDEX4,
                                CHAR_WORKPARTYINDEX5,
                                CHAR_WORKOBJINDEX,
                                CHAR_WORKWALKSTARTSEC,
                                CHAR_WORKWALKSTARTMSEC,
                                CHAR_WORKLOOPSTARTSEC,
                                CHAR_WORKLOOPSTARTMSEC,
                                CHAR_WORKLASTATTACKCHARAINDEX,
                                CHAR_WORKEVENTTYPE,
                                CHAR_WORKGETEXP,
                                CHAR_WORKMODCAPTUREDEFAULT,
                                CHAR_WORKACTION,
                                CHAR_WORKFD,
                                CHAR_WORKFLG,
                                CHAR_WORKGMLEVEL,
                                CHAR_NPCWORKINT1,
                                CHAR_NPCWORKINT2,
                                CHAR_NPCWORKINT3,
                                CHAR_NPCWORKINT4,
                                CHAR_NPCWORKINT5,
                                CHAR_NPCWORKINT6,
                                CHAR_NPCWORKINT7,
                                CHAR_NPCWORKINT8,
                                CHAR_NPCWORKINT9,
                                CHAR_NPCWORKINT10,
                                CHAR_NPCWORKINT11,
#ifdef _PETRACE
                                CHAR_NPCWORKINT12,
#endif
                                CHAR_NPCWORKINT13,
                                CHAR_WORKWARPCHECK,
                                CHAR_TENSEICHECKED,
                                CHAR_WORKFMINDEXI,		// 家族索引 index
                                CHAR_WORKFMCHANNEL,		// 家族頻道
                                CHAR_WORKFMCHANNELQUICK,
                                CHAR_WORKFMFLOOR,		// 家族據點圖層
                                CHAR_WORKFMDP,			// 家族ＤＰ值
                                CHAR_WORKFMPKFLAG,		// 是否正參與家族ＰＫ 1:Yes
                                CHAR_WORKFMSETUPFLAG,	// 家族是否已正式成立
                                CHAR_WORKFMMANINDEX,	// FMWARPMAN Index
                                CHAR_WORKFMCHARINDEX,	// 家族成員索引 index
                                CHAR_WORKBATTLEFLAG,    // -1(戰鬥不能)，1(可戰鬥)    

                                CHAR_WORK_PET0_STAT,
                                CHAR_WORK_PET1_STAT,
                                CHAR_WORK_PET2_STAT,
                                CHAR_WORK_PET3_STAT,
                                CHAR_WORK_PET4_STAT,

                                CHAR_WORKLOGINTIME,		// 玩家登入時間
                                CHAR_WORKTALKCOUNT,
                                CHAR_WORKTALKTIME,
                                CHAR_WORKPETFOLLOW,		// 寵物跟隨
                                CHAR_WORKPETFALL,		// 戰鬥落馬
                                CHAR_WORKLEADERCHANGE,	// 族長換位index
        CHAR_WORKDATAINTNUM,
}CHAR_WORKDATAINT;

struct Char
{
    BOOL        use;

    int			data[CHAR_DATAINTNUM];
    STRING128	string[CHAR_DATACHARNUM];
    char        flg[(CHAR_FLGNUM % (sizeof( char ) * 8)) ? (CHAR_FLGNUM / (sizeof( char ) * 8)) + 1 : (CHAR_FLGNUM / (sizeof( char ) * 8))];

    int         workint[CHAR_WORKDATAINTNUM];
    /*
    int			indexOfExistItems[CHAR_MAXITEMHAVE];
    int			indexOfExistPoolItems[CHAR_MAXPOOLITEMHAVE];

#ifdef _CHAR_POOLITEM
    int* indexOfExistDepotItems;
#endif
#ifdef _CHAR_POOLPET
    int* indexOfExistDepotPets;
#endif

    CHAR_HaveSkill		haveSkill[CHAR_SKILLMAXHAVE];
    int			indexOfHaveTitle[CHAR_TITLEMAXHAVE];
    ADDRESSBOOK_entry	addressBook[ADDRESSBOOK_MAX];
    CHAR_UNIONTABLE		unionTable;
    int			indexOfPoolPet[CHAR_MAXPOOLPETHAVE];
    STRING32    charfunctable[CHAR_FUNCTABLENUM];
    int         workint[CHAR_WORKDATAINTNUM];
    STRING64    workchar[CHAR_WORKDATACHARNUM];
#ifdef _NPC_SEPARATEDATA
    int* tempint;
#endif
    int         CharMakeSequenceNumber;
    void* functable[CHAR_FUNCTABLENUM];
#ifdef _STREET_VENDOR
    StreetVendor_t StreetVendor[MAX_SELL_ITEM];
#endif
    */
};

#define		CHAR_CHECKINDEX( index)	_CHAR_CHECKINDEX( __FILE__, __LINE__, index)
INLINE BOOL _CHAR_CHECKINDEX( const char* file, int line, int index );


#define CHAR_getWorkInt( index , element) _CHAR_getWorkInt( __FILE__, __LINE__, index , element)
INLINE int _CHAR_getWorkInt( const char* file, int line, int index, CHAR_WORKDATAINT element );