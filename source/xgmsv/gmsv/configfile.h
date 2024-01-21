#pragma once

/* 設定を保持する構造体 */
struct Config
{
    /*プログラム名(自動的に求めたいけどまだ駄目*/
    char    progname[9];

    char    configfilename[32]; /* configファイル名 */
    unsigned char debuglevel;   /* デバッグレベル */

    unsigned int  usememoryunit;    /*メモリのユニットサイズ */
    unsigned int  usememoryunitnum; /*メモリのユニット数 */

    char    asname[32];         /*アカウントサーバの名前*/
    unsigned short  acservport; /*アカウントサーバのポート名*/
	char    acpasswd[32];       /*アカウントサーバへのパスワード*/
	char    mailservername[32];
	char    mailserverport[32];
    char    gsnamefromas[32];   /*
                                 * アカウントサーバから見える
                                 * ゲームサーバとしての名前
                                 */

                                 // Arminius 7.24 manor pk
    char gsid[32];	// game server chinese id
#ifdef _SERVICE    
    // Terry 2001/10/03
    char apid[32];      // service ap id
    unsigned short apport;	//service ap port
    int  looptime;	// 幾秒後設定離開
    int  enableservice;	// 是否使用石器服務員功能
#endif    
    unsigned short allowmanorpk;	// is this server allow manor pk

    unsigned short port;        /* サーバの待ち受けポート */

    int				servernumber;	/* ゲームサーバの番号 */
    int				reuseaddr;	/* Address already used... が止まらない時のために */
    int             do_nodelay;     /* TCP_NODELAY にするかどうか */
    int             log_write_time; /* 書きこみログをするかどうか。 */
    int             log_io_time;    /* I/O全体の時間はかるかどうか。 */
    int             log_game_time;  /* ゲームの処理全体の時間をはかる */
    int             log_netloop_faster; /* netloop_faster のログ */
    int				saacwritenum;	/* アカウントサーバへの何人毎にwriteするか */
    int				saacreadnum;	/* アカウントサーバからのdispatch を何回するか */

    unsigned short fdnum;           /*どれだけコネクションを保持するか */
	unsigned int   petcharnum;   	/* ペットの数    */
	unsigned int   othercharnum;    /*  その他のキャラの数  */


	unsigned int db_que_num;
	unsigned int auth_que_num;
	unsigned int objnum;            /* オブジェクトの最大数*/
	unsigned int itemnum;           /* アイテムの最大数*/
	unsigned int battlenum;         /* バトルの最大*/
	unsigned int auto_dungeon_num;
	unsigned int house_num;
	unsigned int mapjob_num;


    char    top_dir[64];         	/* トップディレクトリ名*/

    char    map_dir[64];         	/* マップディレクトリ名*/
	char    material_map_dir[64];
	char    auto_dungeon_map_dir[64];
	char    house_map_dir[64];
	char    house_room_file[64];
	char    byte_8D3B4A0[64];
    char    map_tile_file[64];    	/* マップ設定ファイル名*/
    char    battlemapfile[64];    	/* バトルマップ設定ファイル名*/
    char    mapdebugfile[64];       /* todo: does it make sense?*/
	char    itemfile[64];       	/* アイテム設定ファイル名*/
	char    item_recipe_file[64];
	char    item_material_file[64];
	char    title_name_file[64];
	char    title_config_file[64];
	char	encount_file[64];		/* エンカウント設定ファイル名 */
	char	enemy_base_file[64];		/* 敵基本設定ファイル名 */
	char	enemy_file[64];			/* 敵設定ファイル名 */
	char	group_file[64];			/* グループ設定ファイル名 */
	char	magic_file[64];			/* 呪術設定ファイル名 */
	char    item_atom_file[64];
	char    effect_settings_file[64];
	char    quiz_file[64];
	char    jobs_file[64];
	char    jobs_ancestry_file[64];
	char    skill_file[64];
	char    skill_exp_file[64];
	char    skill_lv_file[64];
	char    tech_file[64];
	char    byte_8D3BAE0[64];
	char    byte_8D3BB20[64];
	char    byte_8D3BB60[64];
	char    byte_8D3BBA0[64];
	char    byte_8D3BBE0[64];
	char    byte_8D3BC20[64];
	char    map_job_file[64];
	char    byte_8D3BCA0[64];
	char    byte_8D3BCE0[64];
	char    byte_8D3BD20[64];
	char    byte_8D3BD60[64];
	char    ls_gen_log[64];
	char    store_dir[64];
	char    npc_file[64];
	char    warp_file[64];
	char    dungeon_npc_file[64];
	char    dungeon_config_file[64];
	char    dungeon_encount_file[64];
	char    make_sequence_file[64];
	char    byte_8D3BFA0[64];
	char    byte_8D3BFE0[64];
	char    border_file[64];
	char    to_boss_npc_file[64];
	char    byte_8D3C0A0[64];
	char    pet_mail_impossible_file[64];
	char    pet_mail_mistake_file[64];
	char    enemy_ai_file[64];
	char    enemy_msg_file[64];
	char    ship_file[64];
	char    harbor_file[64];
	char    morph_file[64];
	char    byte_8D3C2A0[64];
	char    byte_8D3C2E0[64];
	char    guise_file[64];
	char    rss_file[64];
	char    fish_file[64];
	char    log_dir[64];
	char    log_config_file[64];
	char    chat_magic_password[64];
	char    end_of_gmsv_file[64];
	char    check_sys_log_file[64];
	char    extra_ip_address[16];
	char    invfile[64];        	/* 無敵設定ファイル名*/
    char    appearfile[64];     	/* 出現位置設定ファイル名*/
    char	titlenamefile[64];		/* 称号ファイル名 */
    char	titleconfigfile[64];	/* 称号設定ファイル名 */
#ifdef __ATTACK_MAGIC


    char    attmagicfile[64];       // 攻擊性咒術

#endif

    char	petskillfile[64];		/* ペット技呪術設定ファイル名 */
    char    itematomfile[64];       /* アイテムの材料表ファイル */
    char    effectfile[64];     	/* 演出設定ファイル名*/
    char    quizfile[64];     		/* クイズ設定ファイル名*/


    char    lsgenlog[64];       /*サーバのlsgen アウトプットファイル名*/

    char    storedir[64];       /*ストアディレクトリ名  */
    char    npcdir[64];         /*NPCの設定ファイルを置くディレクトリ   */

    char    logdir[64];         /*
                                 * ログディレクトリ
                                 */
    char    logconfname[64];    /*
                                 * ログ設定ファイル名
                                 */
    char	chatmagicpasswd[64];	/* チャット魔法パスワード */

#ifdef _STORECHAR
    char	storechar[64];
#endif

    unsigned int 	chatmagiccdkeycheck;	/* チャット魔法でCDKEYをチェックするか */

    unsigned int    filesearchnum;     /*ファイルを検索できるファイルの数*/
    unsigned int    npctemplatenum;     /*NPCのテンプレートファイルの数*/
    unsigned int    npccreatenum;       /*NPCの生産ファイルの数*/
    unsigned int    walksendinterval;   /* 歩くのを送る間隔 */
    unsigned int    CAsendinterval_ms;     /* CAを送る間隔 (ms)*/
    unsigned int    CDsendinterval_ms;     /* CDを送る間隔 (ms)*/
    unsigned int    Onelooptime_ms;     	/* 1ループにかける時間 */
    unsigned int	Petdeletetime;		/* ペットが消滅する時間 */
    unsigned int	Itemdeletetime;		/* アイテムが消滅する時間 */
    /* ログイン中のキャラのセーブをする間隔 */
    unsigned int    CharSavesendinterval;
    unsigned int    addressbookoffmsgnum;  /*
                                            * アドレスブックにオフライン
                                            * メッセージを
                                            * 何メッセージ残せるか
                                            */

    unsigned int    protocolreadfrequency;  /*
                                             * プロトコルを何ミリ秒毎
                                             * に読むか
                                             */
    unsigned int    allowerrornum;          /*
                                             * エラーを何個まで許すか
                                             */
    unsigned int    loghour;          		/*
                                             * ログを保存する時刻（？時）
                                             */
    unsigned int    battledebugmsg;    		/*
                                             * バトル中のデバッグメッセージを出すか。０なら出さない
                                             */
                                             //ttom add this because the second had this                                         
    unsigned int    encodekey;
    unsigned int    acwbsize;
    unsigned int    acwritesize;
    unsigned int    ErrUserDownFlg;
    //ttom end
#ifdef _GMRELOAD
    char	gmsetfile[64];				/* GM帳號、權限設定檔 */
#endif

#ifdef _AUCTIONEER
    char auctiondir[256];   // 拍賣資料目錄
#endif
#ifdef _BLACK_MARKET 
    char blackmarketfile[256];
#endif
#ifdef _M_SERVER
    char    msname[32];
    unsigned short  msport;
#endif
#ifdef _NPCSERVER_NEW
    char    nsaddress[64];
    unsigned short  nsport;
#endif

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
    char profession[64];
#endif

#ifdef _ITEM_QUITPARTY
    char itemquitparty[64];
#endif

#ifdef _MUSEUM
    int museum;
#endif

#ifdef _DEL_DROP_GOLD	
    unsigned int	Golddeletetime;
#endif

};

extern Config config;

bool readconfigfile(char* filename);
