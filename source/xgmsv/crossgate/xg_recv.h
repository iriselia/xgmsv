#pragma once
#include "gmsv/map/readmap.h"

namespace crossgate
{
#define send_msg(str) xg_send(session, (uint8*)str, sizeof(str))
#define send_raw(str) xg_send(session, (uint8*)str "\n", sizeof(str), true)


	void recv_W( xg_session* session, xg_packet* packet ) { }
	void recv_w( xg_session* session, xg_packet* packet ) { }
	void recv_EV( xg_session* session, xg_packet* packet ) { }
	void recv_M( xg_session* session, xg_packet* packet )
	{
		int32 map, floor, x1, y1, x2, y2;

        packet->r(&map);
        packet->r(&floor);
        packet->r(&x1);
        packet->r(&y1);
        packet->r(&x2);
        packet->r(&y2);

		RECT_SA seekr, realr;
		seekr.x = x1;
		seekr.y = y1;
		seekr.width = x2 - x1;
		seekr.height = y2 - y1;
		//CONNECT_isCLI
		//CONNECT_isLOGIN

        xg_packet p = xg_begin_packet(session, XG_M);
		char* data = MAP_getdataFromRECT(map, floor, &seekr, &realr);

		if ( data )
		{
			//nrproto_M_send()
		}
	}

	void recv_EN( xg_session* session, xg_packet* packet ) { }
	void recv_DU( xg_session* session, xg_packet* packet ) { }
	void recv_EO( xg_session* session, xg_packet* packet ) { }
	void recv_BU( xg_session* session, xg_packet* packet ) { }
	void recv_JB( xg_session* session, xg_packet* packet ) { }
	void recv_LB( xg_session* session, xg_packet* packet ) { }
	void recv_B( xg_session* session, xg_packet* packet ) { }
	void recv_ID( xg_session* session, xg_packet* packet ) { }
	void recv_IDF( xg_session* session, xg_packet* packet ) { }
	void recv_PI( xg_session* session, xg_packet* packet ) { }
	void recv_DI( xg_session* session, xg_packet* packet ) { }
	void recv_DG( xg_session* session, xg_packet* packet ) { }
	void recv_DP( xg_session* session, xg_packet* packet ) { }
	void recv_MI( xg_session* session, xg_packet* packet ) { }
	void recv_IR( xg_session* session, xg_packet* packet ) { }
	void recv_MSG( xg_session* session, xg_packet* packet ) { }
	void recv_ALI( xg_session* session, xg_packet* packet ) { }
	void recv_ALN( xg_session* session, xg_packet* packet ) { }
	void recv_PMSG( xg_session* session, xg_packet* packet ) { }
	void recv_DAB( xg_session* session, xg_packet* packet ) { }
	void recv_AAB( xg_session* session, xg_packet* packet ) { }
	void recv_GI( xg_session* session, xg_packet* packet ) { }
	void recv_GT( xg_session* session, xg_packet* packet ) { }
	void recv_GMR( xg_session* session, xg_packet* packet ) { }
	void recv_BGT( xg_session* session, xg_packet* packet ) { }
	void recv_AGM( xg_session* session, xg_packet* packet ) { }
	void recv_LG( xg_session* session, xg_packet* packet ) { }
	void recv_RGM( xg_session* session, xg_packet* packet ) { }
	void recv_GML( xg_session* session, xg_packet* packet ) { }
	void recv_PGML( xg_session* session, xg_packet* packet ) { }
	void recv_GD( xg_session* session, xg_packet* packet ) { }
	void recv_PRV( xg_session* session, xg_packet* packet ) { }
	void recv_PRS( xg_session* session, xg_packet* packet ) { }
	void recv_PRD( xg_session* session, xg_packet* packet ) { }
	void recv_PRE( xg_session* session, xg_packet* packet ) { }
	void recv_PRM( xg_session* session, xg_packet* packet ) { }
	void recv_PRO( xg_session* session, xg_packet* packet ) { }
	void recv_L( xg_session* session, xg_packet* packet ) { }
	void recv_TK( xg_session* session, xg_packet* packet ) { }
	void recv_FS( xg_session* session, xg_packet* packet ) { }
	void recv_HL( xg_session* session, xg_packet* packet ) { }
	void recv_PR( xg_session* session, xg_packet* packet ) { }
	void recv_KS( xg_session* session, xg_packet* packet ) { }
	void recv_MP( xg_session* session, xg_packet* packet ) { }
	void recv_GFL( xg_session* session, xg_packet* packet ) { }
	void recv_GPD( xg_session* session, xg_packet* packet ) { }
	void recv_GFLI( xg_session* session, xg_packet* packet ) { }
	void recv_GPDI( xg_session* session, xg_packet* packet ) { }
	void recv_IH( xg_session* session, xg_packet* packet ) { }
	void recv_AC( xg_session* session, xg_packet* packet ) { }
	void recv_ACS( xg_session* session, xg_packet* packet ) { }
	void recv_MU( xg_session* session, xg_packet* packet ) { }
	void recv_TU( xg_session* session, xg_packet* packet ) { }
	void recv_TRPL( xg_session* session, xg_packet* packet ) { }
	void recv_TRS( xg_session* session, xg_packet* packet ) { }
	void recv_TROP( xg_session* session, xg_packet* packet ) { }
	void recv_TRCL( xg_session* session, xg_packet* packet ) { }
	void recv_TROC( xg_session* session, xg_packet* packet ) { }
	void recv_PS( xg_session* session, xg_packet* packet ) { }
	void recv_ST( xg_session* session, xg_packet* packet ) { }
	void recv_DT( xg_session* session, xg_packet* packet ) { }
	void recv_FT( xg_session* session, xg_packet* packet ) { }
	void recv_LVUP( xg_session* session, xg_packet* packet ) { }
	void recv_PLVUP( xg_session* session, xg_packet* packet ) { }
	void recv_SKSW( xg_session* session, xg_packet* packet ) { }
	void recv_PSSW( xg_session* session, xg_packet* packet ) { }
	void recv_POS( xg_session* session, xg_packet* packet ) { }
	void recv_KN( xg_session* session, xg_packet* packet ) { }
	void recv_WN( xg_session* session, xg_packet* packet ) { }
	void recv_SP( xg_session* session, xg_packet* packet ) { }
	void recv_ClientLogin( xg_session* session, xg_packet* packet )
	{
		enum status_code
		{
			success = 0,
			wrong_login_or_banned = 1,
			refuse = 2
		};

		"SELECT CDKEY,AccountID,AccountPassWord,EnableFlg,UseFlg,BadMsg from tbl_user Where AccountID='%s' ";

		xg_packet p = xg_begin_packet( session, XG_ClientLogin );
		p.w( success );
		p.w( "block" ); // "failed"
		xg_send( session, p );
	}
	void recv_CreateNewChar( xg_session* session, xg_packet* packet )
	{
		//"CreateNewChar 1 Ï¡ÓÐÖ¥Ê¿ rDW 10ZE 0 0 0 f f 0 0 5 5 "
		//"CreateNewChar successful 3"
	}
	void recv_CharDelete( xg_session* session, xg_packet* packet )
	{
		//"CharDelete 2 " left
		//"CharDelete 3 " right
		//"CharDelete successful 3" ? not sure
	}
	void recv_CharLogin( xg_session* session, xg_packet* packet )
	{
		/*
		void lssproto_CharLogin_recv( int fd,char* charname )
		{
			char cdkey[CDKEYLEN], passwd[PASSWDLEN];

			if( CONNECT_isCLI( fd ) == FALSE )return;
			print( "Attempt to login: charaname=%s\n", charname);
			if( charname[0] == '\0' ){
				lssproto_CharLogin_send( fd, FAILED_STRING, "Can't access char have no name\n" );
				return;
			}
			if( CONNECT_isNOTLOGIN(fd) == FALSE ){
				lssproto_CharLogin_send( fd, FAILED_STRING, "Already Logged in\n" );
				return;
			}
			CONNECT_setCharname( fd, charname );
			CONNECT_getCdkey( fd, cdkey, sizeof( cdkey ));
			CONNECT_getPasswd( fd, passwd, sizeof(passwd));
			saacproto_ACCharLoad_send( acfd, cdkey,passwd, charname,1,"",
										CONNECT_getFdid(fd) );
			CONNECT_setState( fd, WHILELOGIN );
		}
		*/

		send_msg( R"(PRV 3|5|100|0 )" );

		send_msg( R"(CharLogin successful  )" );

		send_msg( R"(LI 3H9 1JbdtH 1 0 )" );

		send_msg( R"(CC 0 hV p J c 6 0 0 0 0 -1 )" );

		send_msg( R"(MN ³ÇÎ÷Ò½Ôº\\z0 )" );

		send_msg( R"(CP 1|155|155|185|185|15|0|0|0|15|50|50|50|0|16|1|23|23|23|107|107|60|100|100|0|0|0|5000|0|jason|| )" );

		send_msg( R"(CP2 1|0|0|0|0|0|0|0|0|0|0|100|0|0|0|0|241400|10|100| )" );

		send_msg( R"(CJ 1 ¼ûÏ°´«½ÌÊ¿ )" );

		//send_msg(R"(CS 0|||||||||||1|||||||||||2|||||||||||3|||||||||||4|||||||||||5|||||||||||6|||||||||||7|||||||||||8|||||||||||9|||||||||||10|||||||||||11|||||||||||12|||||||||||13|||||||||||14||||||||||| )");

		//send_msg(R"(TITLE ¾´Î·µÄ¼Å¾²|0|17|¼ûÏ°´«½ÌÊ¿|1|161|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| )");

		//send_msg(R"(I 7|³öÉúË®¾§|0|$4µÈ¼¶\S1\\n$4ÄÍ¾Ã\S1000/1000\S$0Àà±ð\SË®¾§\\n\\n\\n\\n\\n\\n\\n$4³õÆÚ×°±¸µÄË®¾§\\n\\n$1·ÅµØÉÏ±ã»áÏûÊ§\S\\n$1ÎÞ·¨ÓÃ³èÎïÓÊ¼þ·¢ËÍ\S\\n|27511|1|2|0|1|184|9200|22|0||8|Õ´×ÅÀáË®µÄÐÅ|0|$4µÈ¼¶\S1\\n$0Àà±ð\SÎ´Öª\\n\\n\\n\\n\\n\\n\\n$4\\n\\n$1·ÅµØÉÏ±ã»áÏûÊ§\S\\n$1ÎÞ·¨ÓÃ³èÎïÓÊ¼þ·¢ËÍ\S\\n|26579|1|0|0|1|56|22238|26|0||9|Ê±¼äË®¾§Lv6|0|$4µÈ¼¶\S1\\n$4ÄÍ¾Ã\S0001/0001\S$0Àà±ð\SÎ´Öª\\nÄÜ»Ö¸´×÷ÒµÊ±¼äµÄË®¾§£¬»Ö¸´ºóµÄÐ¡Ê±ÊýÓëË®¾§µÈ¼¶ÏàµÈ¡£\\n\\n\\n\\n\\n$4ÄÜ»Ö¸´×÷ÒµÊ±¼äµÄË® ¾§£¬»Ö¸´ºóµÄÐ¡Ê±ÊýÓëË®¾§µÈ¼¶ÏàµÈ¡£\\n\\n$1·ÅµØÉÏ±ã»áÏûÊ§\S\\n$1ÎÞ·¨ÓÃ³èÎïÓÊ¼þ·¢ËÍ\S\\n|99123|1|0|0|1|56|22693|26|1||10| ËæÒâ¹¤×÷¿¨|0|$4µÈ¼¶\S1\\n$4ÄÍ¾Ã\S0005/0005\S$0Àà±ð\SÎ´Öª\\n\\n\\n\\n\\n\\n\\n$4\\n\\n|27487|1|0|0|1|25|22022|26|0||11|ÉúÃü»Ø¸´Ò©Ë®(200)|0|$4µÈ¼¶\S3\\n$0Àà±ð\SÒ©Æ·\\n»Ö¸´Ä¿±êÉúÃüÖµÔ¼200µã\\n\\n\\n\\n\\n\\n$4»Ø¸´ÉúÃüÁ¦200µãµÄÒ©\\n\\n|26218|1|1|93|3|25|15608|43|20||12|´«ËÍÓðÃ«-Î÷Ò½|0|$4µÈ¼¶\S1\\n$4ÄÍ¾Ã\S0004/0005\S$0Àà±ð\SÎ´Öª\\n\\n\\n\\n\\n\\n\\n$4Ë²¼ä¾Í´«ËÍµ½Î÷Ò½ÔºµÄÉñÆæÓðÃ«¡£\\n\\n|27828|1|0|0|1|25|18779|26|0||13|É®ÂÂÊÊÐÔ¼ì²éºÏ¸ñÖ¤|0|$4µÈ¼¶\S1\\n$0Àà±ð\SÎ´Öª\\n\\n\\n\\n\\n\\n\\n$4´«½ÌÊ¿Ö°ÒµµÄ¾ÍÖ°ÍÆ¼öÐÅ\\n\\n$1·ÅµØÉÏ±ã»áÏûÊ§\S\\n$1ÎÞ·¨ÓÃ³èÎïÓÊ¼þ·¢ËÍ\S\\n|27881|0|0|0|1|56|18106|26|0||14|±¨³êµÄÒø±Ò|0|$4µÈ¼¶\S1\\n$0Àà±ð\SÎ´Öª\\n\\n\\n\\n\\n\\n\\n$4·¨À¼Íõ¹úµÄÒø±Ò\S\S\\n\\n$1·ÅµØÉÏ±ã»áÏûÊ§\S\\n$1ÎÞ·¨ÓÃ³èÎïÓÊ¼þ·¢ËÍ\S\\n|26573|0|0|72|1|56|18786|26|0| )");

		send_msg( R"(EP 0 0 )" );

		//send_msg(R"(KP 0 1|101321|4|93|93|99|99|1|16|1|48|40|34|101|100|24|0|40|60|0|10|1|ÐÂÊÖºìó«òë||0|0| )");

		//send_msg(R"(KP2 0 1|2|9|4|5|3|25|0|45|-10|0|0|0|0|0|0|1| )");

		//send_msg(R"(PT 0 0|7300|¹¥»÷|ÄÜÒÔÆÕÍ¨ÎïÀí¹¥»÷¸øÓë´ò»÷|0|1|1141|0|1|7400|·ÀÓù|ÄÜ·ÀÊØÀ´×ÔÎïÀí¹¥»÷µÄ´ò»÷|0|1|72|1|2|407|Æø¹¦µ¯\SLV8|¸øÓè¶ÔÏóÇ°ºóÁÐÎ»ÖÃÒ»Ìå»òÊýÌåµÄÉËº¦£¬ÒÀµÈ¼¶¸Ä±ä¹¥»÷Êý¡£|40|1|117|2|3|1238|Ã÷¾»Ö¹Ë®-¢õ|¼¯ÖÐ¾«Éñ»Ø¸´Ò»¶¨±ÈÀýµÄÌåÁ¦£¬¼¼ ÄÜºÍ×î´óÉúÃüÖµÔ½¸ß»Ø¸´±ÈÀýÉÏÏÞÔ½¸ß(×¢ÒâºÍ»Ø¸´Á¦ÎÞ¹Ø)£¬Ê¹ÓÃºó½«ÎÞ·¨ÉÁ¶ãÎïÀí¹¥»÷¡£\S|135|1|72|3| )");

		send_msg( R"(FS 0 )" );

		send_msg( R"(MC 0 hV 0 0 p k bFX agh 0 )" );

		//send_msg(R"(AB ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| )");

		//send_msg(R"(ABG ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| )");

		//send_raw(R"(PBt-ztsrSCXizpHiNuKPg5AuyS6PzdstfC7blf3rb-Dbo7tf5NhhQ-+Pg4--7kE6qjGnMfl6th1N20udvQ)");
		send_msg( R"(C 1|3H9|12|6|0|106003|1|0|jason|\\z\\z|0|0|0|¼ûÏ°´«½ÌÊ¿|0 )" );

		send_msg( R"(C 2|1tq|10|5|4|14088|1|0|²®ÊæÒ½Ê¦|\\z\\z|0|0|0||0	37|1ty|10|17|4|14088|1|0|Ä·ÄùÒ½Ê¦|\\z\\z|0|0|0||0	2|1ti|12|5|4|14090|1|0|ÊµÏ°Ò©¼ÁÊ¦¼ª¿É|\\z\\z|0|0|0||0	29|1xH|16|9|6|14151|1|0|ÊµÏ°ÉúÃÛÑÅ|\\z\\z|0|0|0||0 )" );

		send_msg( R"(BT 5Co 0 )" );
		send_msg( R"(POS 0 )" );
		send_msg( R"(AL 1 0|0|0|0|0|0|0|0|0|0| )" );
		send_msg( R"(IP 127.0.0.1 )" );
		send_msg( R"(MAC Y O )" );
		send_msg( R"(EF 0 0  )" );
		send_msg( R"(TK -1 P|»¶Ó­À´µ½Ö¥Ê¿µÄÄ§Á¦±¦±´·þÎñÆ÷~ 4 0 )" );
		send_msg( R"(CA 3H9|12|6|0|0 )" );

		/*
		send_raw(R"(TK -1 P|¸ÐÐ»¹ºÂò´ó»ÒÀÇÄ§Á¦¡£wow335.taobao.com 4 0 )");
		send_raw(R"(TK -1 P|[°æ±¾ÉêÃ÷]GMSV\SAvaritia\SFeb\S\S1\S2014\S¹²Ïí°æ 4 0 )");
		send_raw(R"(TK -1 P|±¾·þÎñ¶Ë½ö¹©ÑÐ¾¿Ê¹ÓÃ£¬ÇëÎðÓÃ×÷ÉÌÒµÓÃÍ¾¡£ 4 0 )");
		send_raw(R"(TK -1 P|ÏîÄ¿Ö÷Ò³\S&\S½»Á÷ÂÛÌ³£ºhttp://www.cgdev.me/ 4 0 )");
		send_raw(R"(STK GA\SLuaÒýÇæÔËÐÐÕý³£¡£ )");
		send_raw(R"(STK [¶þ¼üÄ§Á¦¹«¸æ]¿ÉÓÃ\S/help\SÖ¸Áî²é¿´µ±Ç°¿ÉÓÃLuaTalkÖ¸Áî )");
		*/
		//send_raw(R"()");
		/*
		*/
	}
	void recv_CharList( xg_session* session, xg_packet* packet )
	{
		enum fields
		{
			name = 0,
			something = 1,
			portrait = 2,
			level = 3,
			vitality = 4,
			strength = 5,
			toughness = 6,
			quickness = 7,
			magic = 8,
			fire = 9,
			wind = 10,
			earth = 11,
			water = 12,
			login_times = 13,
			name_again = 14,
			soemthing2 = 15,
			char_class = 16,
			something3 = 17,
			char_sprite = 18,
			something4 = 19
		};

		{
			xg_packet p = xg_begin_packet(session, XG_PV);
			p.w(8); // trial_flag
			xg_send(session, p);
		}

		{
			xg_packet p = xg_begin_packet(session, XG_CharList);

			//p.w("0 222|0\\z205425\\z1\\z15\\z15\\z0\\z0\\z0\\z0\\z0\\z100\\z0\\z2\\z222\\z10\\zYoumin\\z3\\z104468\\z-1|RareCheese|1\\z242125\\z1\\z0\\z15\\z0\\z15\\z0\\z0\\z0\\z50\\z50\\z9\\zRareCheese\\z11\\zYoumin\\z2\\z106258\\z-1|");
			p.w(R"(0 RareCheese|1\\z242125\\z1\\z0\\z15\\z0\\z15\\z0\\z0\\z0\\z50\\z50\\z9\\zRareCheese\\z11\\z游民\\z2\\z106258\\z-1|)");
			xg_send(session, p);
		}

		//send_msg( R"(CharList 0 jason|0\\z241401\\z1\\z15\\z0\\z0\\z0\\z15\\z0\\z0\\z100\\z0\\z12\\zjason\\z2\\z¼ûÏ°´«½ÌÊ¿\\z2\\z106002\\z-1| )" );

		printf( "handle_char_list!\n" );
	}
	void recv_CharLogout( xg_session* session, xg_packet* packet )
	{
		send_msg( R"(CharLogout successful )" );
		printf( "handle_char_logout!\n" );

		//signal.fire(core::signal_code::shutdown);

		//marked_for_delete = true;
	}
	void recv_ProcGet( xg_session* session, xg_packet* packet ) { }
	void recv_PlayerNumGet( xg_session* session, xg_packet* packet ) { }
	void recv_Echo( xg_session* session, xg_packet* packet )
	{
		char msg[64];
		snprintf( msg, sizeof( msg ), "%s nr \n", xg_opcodes[XG_Echo].send_op );
		//printf( "handle_echo!\n" );
	}
	void recv_Shutdown( xg_session* session, xg_packet* packet ) { }
	void recv_FC( xg_session* session, xg_packet* packet )
	{
		//printf( "handle_fc!\n" );
	}
	void recv_CH( xg_session* session, xg_packet* packet ) { }
	void recv_CharLoginGate( xg_session* session, xg_packet* packet ) { }
	void recv_PVUP( xg_session* session, xg_packet* packet ) { }
	void recv_StallStart( xg_session* session, xg_packet* packet ) { }
	void recv_StallEnd( xg_session* session, xg_packet* packet ) { }
	void recv_StallBrowse( xg_session* session, xg_packet* packet ) { }
	void recv_StallBuy( xg_session* session, xg_packet* packet ) { }
}
