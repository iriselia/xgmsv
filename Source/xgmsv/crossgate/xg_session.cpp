#pragma once

#include "core/shared.h"

#include "core/network/timer.h"

#include "crossgate/xg_session.h"
#include "crossgate/xg_packet.h"

namespace server
{
	namespace crossgate
	{
		struct xg_session_pool xg_session_pool;

		xg_session::xg_session(core::tcp_connection* connection, connection_type type) :
			connections{}
		{
			connections[(uint32)type] = connection;

			connection->rqueue = &rqueue;
			connection->wqueue = &wqueue;
			connection->wsignal = &signal;
		}

		xg_session_pool::xg_session_pool() :
			session_count(0)
		{}

		xg_session* xg_session_pool::spawn_session(core::tcp_connection* connection, connection_type type)
		{
			for (int id = 0; id < this->size(); id++)
			{
				auto& new_session = this->at(id);
				if (!new_session)
				{
					this->session_count++;
					new_session = new xg_session(connection, type);
					new_session->id = id;
					return new_session;
				}
			}

			return nullptr;
		}

		void init_user_session_pool(uint32 max_session_count)
		{
			core::async_every(1000ms, []()
			{

				int sessions_purged = 0;
				for (auto& session : xg_session_pool)
				{
					if (session)
					{
						uint32 id = session->id;

						auto inactive_seconds = std::chrono::duration_cast<std::chrono::seconds>(core::server_time - session->last_active);

						if (inactive_seconds.count() > 60)
						{
							delete session;
							session = nullptr;
							xg_session_pool.session_count--;
							sessions_purged++;
						}

						//if (session->marked_for_delete)
						//{
						//}
					}
				}

				if (sessions_purged)
				{
					printf("%d sessions purged.\n", sessions_purged);
				}

			});

			xg_session_pool.resize(max_session_count);
		}


		void xg_session::handle_echo(xg_packet* packet)
		{
			if (!send_msg(R"(Echo nr )"))
			{
				printf("handle_echo failed.\n");
			}

			printf("handle_echo!\n");
		}


		void xg_session::handle_fc(xg_packet* packet)
		{
			printf("handle_fc!\n");
		}

		void xg_session::handle_client_login(xg_packet* packet)
		{
			enum status_code
			{
				success = 0,
				wrong_login_or_banned = 1,
				refuse = 2
			};

			if (!send_msg(R"(ClientLogin 0 block )"))
			{
				printf("handle_client_login failed.\n");
			}

			printf("handle_client_login!\n");
		}

		void xg_session::handle_char_list(xg_packet* packet)
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

			send_msg(R"(PV 8 )");
			send_msg(R"(CharList 0 jason|0\\z241401\\z1\\z15\\z0\\z0\\z0\\z15\\z0\\z0\\z100\\z0\\z12\\zjason\\z2\\z见习传教士\\z2\\z106002\\z-1| )");

			printf("handle_char_list!\n");
		}

		void xg_session::handle_char_login(xg_packet* packet)
		{
			send_raw(R"(EJQ35XO0sXyvF5j6Y2HPBZM)");
			//send_msg(R"(PRV 3|5|100|0 )");

			send_msg(R"(CharLogin successful  )");
			
			send_msg(R"(LI 3H9 1JbdtH 1 0 )");
			
			send_msg(R"(CC 0 hV p J c 6 0 0 0 0 -1 )");
			
			send_msg(R"(MN 城西医院\\z0 )");
			
			send_msg(R"(CP 1|155|155|185|185|15|0|0|0|15|50|50|50|0|16|1|23|23|23|107|107|60|100|100|0|0|0|5000|0|jason|| )");
			
			send_msg(R"(CP2 1|0|0|0|0|0|0|0|0|0|0|100|0|0|0|0|241400|10|100| )");
			
			send_msg(R"(CJ 1 见习传教士 )");
			
			//send_msg(R"(CS 0|||||||||||1|||||||||||2|||||||||||3|||||||||||4|||||||||||5|||||||||||6|||||||||||7|||||||||||8|||||||||||9|||||||||||10|||||||||||11|||||||||||12|||||||||||13|||||||||||14||||||||||| )");
			
			//send_msg(R"(TITLE 敬畏的寂静|0|17|见习传教士|1|161|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| )");
			
			//send_msg(R"(I 7|出生水晶|0|$4等级\S1\\n$4耐久\S1000/1000\S$0类别\S水晶\\n\\n\\n\\n\\n\\n\\n$4初期装备的水晶\\n\\n$1放地上便会消失\S\\n$1无法用宠物邮件发送\S\\n|27511|1|2|0|1|184|9200|22|0||8|沾着泪水的信|0|$4等级\S1\\n$0类别\S未知\\n\\n\\n\\n\\n\\n\\n$4\\n\\n$1放地上便会消失\S\\n$1无法用宠物邮件发送\S\\n|26579|1|0|0|1|56|22238|26|0||9|时间水晶Lv6|0|$4等级\S1\\n$4耐久\S0001/0001\S$0类别\S未知\\n能恢复作业时间的水晶，恢复后的小时数与水晶等级相等。\\n\\n\\n\\n\\n$4能恢复作业时间的水 晶，恢复后的小时数与水晶等级相等。\\n\\n$1放地上便会消失\S\\n$1无法用宠物邮件发送\S\\n|99123|1|0|0|1|56|22693|26|1||10| 随意工作卡|0|$4等级\S1\\n$4耐久\S0005/0005\S$0类别\S未知\\n\\n\\n\\n\\n\\n\\n$4\\n\\n|27487|1|0|0|1|25|22022|26|0||11|生命回复药水(200)|0|$4等级\S3\\n$0类别\S药品\\n恢复目标生命值约200点\\n\\n\\n\\n\\n\\n$4回复生命力200点的药\\n\\n|26218|1|1|93|3|25|15608|43|20||12|传送羽毛-西医|0|$4等级\S1\\n$4耐久\S0004/0005\S$0类别\S未知\\n\\n\\n\\n\\n\\n\\n$4瞬间就传送到西医院的神奇羽毛。\\n\\n|27828|1|0|0|1|25|18779|26|0||13|僧侣适性检查合格证|0|$4等级\S1\\n$0类别\S未知\\n\\n\\n\\n\\n\\n\\n$4传教士职业的就职推荐信\\n\\n$1放地上便会消失\S\\n$1无法用宠物邮件发送\S\\n|27881|0|0|0|1|56|18106|26|0||14|报酬的银币|0|$4等级\S1\\n$0类别\S未知\\n\\n\\n\\n\\n\\n\\n$4法兰王国的银币\S\S\\n\\n$1放地上便会消失\S\\n$1无法用宠物邮件发送\S\\n|26573|0|0|72|1|56|18786|26|0| )");
			
			send_msg(R"(EP 0 0 )");
			
			//send_msg(R"(KP 0 1|101321|4|93|93|99|99|1|16|1|48|40|34|101|100|24|0|40|60|0|10|1|新手红螳螂||0|0| )");
			
			//send_msg(R"(KP2 0 1|2|9|4|5|3|25|0|45|-10|0|0|0|0|0|0|1| )");
			
			//send_msg(R"(PT 0 0|7300|攻击|能以普通物理攻击给与打击|0|1|1141|0|1|7400|防御|能防守来自物理攻击的打击|0|1|72|1|2|407|气功弹\SLV8|给予对象前后列位置一体或数体的伤害，依等级改变攻击数。|40|1|117|2|3|1238|明净止水-Ⅴ|集中精神回复一定比例的体力，技 能和最大生命值越高回复比例上限越高(注意和回复力无关)，使用后将无法闪躲物理攻击。\S|135|1|72|3| )");
			
			send_msg(R"(FS 0 )");
			
			send_msg(R"(MC 0 hV 0 0 p k bFX agh 0 )");
			
			//send_msg(R"(AB ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| )");
			
			//send_msg(R"(ABG ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| )");
			
			send_raw(R"(PBt-ztsrSCXizpHiNuKPg5AuyS6PzdstfC7blf3rb-Dbo7tf5NhhQ-+Pg4--7kE6qjGnMfl6th1N20udvQ)");
			//send_msg(R"(C 1|3H9|12|6|0|106002|1|0|jason|\\z\\z|0|0|0|见习传教士|0 )");
			
			send_msg(R"(C 2|1tq|10|5|4|14088|1|0|伯舒医师|\\z\\z|0|0|0||0	37|1ty|10|17|4|14088|1|0|姆涅医师|\\z\\z|0|0|0||0	2|1ti|12|5|4|14090|1|0|实习药剂师吉可|\\z\\z|0|0|0||0	29|1xH|16|9|6|14151|1|0|实习生蜜雅|\\z\\z|0|0|0||0 )");
			
			send_msg(R"(BT 5Co 0 )");
			send_msg(R"(POS 0 )");
			send_msg(R"(AL 1 0|0|0|0|0|0|0|0|0|0| )");
			send_msg(R"(IP 127.0.0.1 )");
			send_msg(R"(MAC Y O )");
			send_msg(R"(EF 0 0  )");
			send_msg(R"(TK -1 P|欢迎来到芝士的魔力宝贝服务器~ 4 0 )");
			send_msg(R"(CA 3H9|12|6|0|0 )");

			core::async_after(1s,
				[this]()
			{
			});
			/*
			send_raw(R"(TK -1 P|感谢购买大灰狼魔力。wow335.taobao.com 4 0 )");
			send_raw(R"(TK -1 P|[版本申明]GMSV\SAvaritia\SFeb\S\S1\S2014\S共享版 4 0 )");
			send_raw(R"(TK -1 P|本服务端仅供研究使用，请勿用作商业用途。 4 0 )");
			send_raw(R"(TK -1 P|项目主页\S&\S交流论坛：http://www.cgdev.me/ 4 0 )");
			send_raw(R"(STK GA\SLua引擎运行正常。 )");
			send_raw(R"(STK [二键魔力公告]可用\S/help\S指令查看当前可用LuaTalk指令 )");
			*/
			//send_raw(R"()");
			/*
			*/
		}

		void xg_session::handle_char_logout(xg_packet* packet)
		{
			send_msg(R"(CharLogout successful )");
			printf("handle_char_logout!\n");

			//signal.fire(core::signal_code::shutdown);

			//marked_for_delete = true;
		}

	}
}
