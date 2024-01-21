#pragma once

struct db_query_t
{

};

int DB_proc_getCharList( db_query_t query ) {}
int DB_proc_createNewChar( db_query_t query ) {}
int DB_proc_CharLogin( db_query_t query ) {}
int DB_proc_CharLogout( db_query_t query ) {}
int DB_proc_CharSave( db_query_t query ) {}
int DB_proc_UnLock( db_query_t query ) {}
int DB_proc_AskJob( db_query_t query ) {}
int DB_proc_getSkillReasonablePopulation( db_query_t query ) {}
int DB_proc_SaveJob( db_query_t query ) {}
int DB_proc_ClrBadMsg( db_query_t query ) {}
int DB_proc_UpdateDuelRanking( db_query_t query ) {}
int DB_proc_GetNormalDuelRanking( db_query_t query ) {}
int DB_proc_GetMyDuelRanking( db_query_t query ) {}
int DB_proc_CharDelete( db_query_t query ) {}
int DB_proc_Graduation( db_query_t query ) {}
int DB_proc_Cdkeysearch( db_query_t query ) {}
int DB_proc_Changeisaoid( db_query_t query ) {}
int DB_proc_Isaosearch( db_query_t query ) {}
int DB_proc_Settrial( db_query_t query ) {}
int DB_proc_MoveChar( db_query_t query ) {}
int DB_proc_RestoreChar( db_query_t query ) {}
int DB_proc_ResetProductKey( db_query_t query ) {}
int DB_proc_createGuild( db_query_t query ) {}
int DB_proc_guildInfo( db_query_t query ) {}
int DB_proc_killGuild( db_query_t query ) {}
int DB_proc_getGuildList( db_query_t query ) {}
int DB_proc_writeOffGuild( db_query_t query ) {}
int DB_proc_toPuk2( db_query_t query ) {}
int DB_proc_setGuildTitle( db_query_t query ) {}
int DB_proc_clearGuildMonsterCloseness( db_query_t query ) {}
int DB_proc_getGuildPetList( db_query_t query ) {}
int DB_proc_getGuildPet( db_query_t query ) {}
int DB_proc_putGuildPet( db_query_t query ) {}
int DB_proc_delGuildPetInRoom( db_query_t query ) {}
int DB_proc_toPuk3( db_query_t query ) {}

int DB_proc_getCharList_Result( db_query_t ) {}
int DB_proc_createNewChar_Result( db_query_t ) {}
int DB_proc_CharLogin_Result( db_query_t ) {}
int DB_proc_CharLogout_Result( db_query_t ) {}
int DB_proc_CharSave_Result( db_query_t ) {}
int DB_proc_UnLock_Result( db_query_t ) {}
int DB_proc_AskJob_Result( db_query_t ) {}
int DB_proc_getSkillReasonablePopulation_Result( db_query_t ) {}
int DB_proc_SaveJob_Result( db_query_t ) {}
int DB_proc_ClrBadMsg_Result( db_query_t ) {}
int DB_proc_UpdateDuelRanking_Result( db_query_t ) {}
int DB_proc_GetNormalDuelRanking_Result( db_query_t ) {}
int DB_proc_GetMyDuelRanking_Result( db_query_t ) {}
int DB_proc_CharDelete_Result( db_query_t ) {}
int DB_proc_Graduation_Result( db_query_t ) {}
int DB_proc_Cdkeysearch_Result( db_query_t ) {}
int DB_proc_Changeisaoid_Result( db_query_t ) {}
int DB_proc_Isaosearch_Result( db_query_t ) {}
int DB_proc_Settrial_Result( db_query_t ) {}
int DB_proc_MoveChar_Result( db_query_t ) {}
int DB_proc_RestoreChar_Result( db_query_t ) {}
int DB_proc_ResetProductKey_Result( db_query_t ) {}
int DB_proc_createGuild_Result( db_query_t ) {}
int DB_proc_guildInfo_Result( db_query_t ) {}
int DB_proc_killGuild_Result( db_query_t ) {}
int DB_proc_getGuildList_Result( db_query_t ) {}
int DB_proc_writeOffGuild_Result( db_query_t ) {}
int DB_proc_toPuk2_Result( db_query_t ) {}
int DB_proc_setGuildTitle_Result( db_query_t ) {}
int DB_proc_clearGuildMonsterCloseness_Result( db_query_t ) {}
int DB_proc_getGuildPetList_Result( db_query_t ) {}
int DB_proc_getGuildPet_Result( db_query_t ) {}
int DB_proc_putGuildPet_Result( db_query_t ) {}
int DB_proc_delGuildPetInRoom_Result( db_query_t ) {}
int DB_proc_toPuk3_Result( db_query_t ) {}

using db_proc_t = int (*)(db_query_t);
db_proc_t DB_proc_jmptbl[] =
{
	0,												// 0
	DB_proc_getCharList,							// 1
	DB_proc_createNewChar,							// 2
	DB_proc_CharLogin,								// 3
	DB_proc_CharLogout,								// 4
	DB_proc_CharSave,								// 5
	DB_proc_UnLock,									// 6
	DB_proc_AskJob,									// 7
	DB_proc_getSkillReasonablePopulation,			// 8
	DB_proc_SaveJob,								// 9
	DB_proc_ClrBadMsg,								// 10
	DB_proc_UpdateDuelRanking,						// 11
	DB_proc_GetNormalDuelRanking,					// 12
	DB_proc_GetMyDuelRanking,						// 13
	DB_proc_CharDelete,								// 14
	DB_proc_Graduation,								// 15
	DB_proc_Cdkeysearch,							// 16
	DB_proc_Changeisaoid,							// 17
	DB_proc_Isaosearch,								// 18
	DB_proc_Settrial,								// 19
	DB_proc_MoveChar,								// 20
	DB_proc_RestoreChar,							// 21
	DB_proc_ResetProductKey,						// 22
	DB_proc_createGuild,							// 23
	DB_proc_guildInfo,								// 24
	DB_proc_killGuild,								// 25
	DB_proc_getGuildList,							// 26
	DB_proc_writeOffGuild,							// 27
	DB_proc_toPuk2,									// 28
	DB_proc_setGuildTitle,							// 29
	DB_proc_clearGuildMonsterCloseness,				// 30
	DB_proc_getGuildPetList,						// 31
	DB_proc_getGuildPet,							// 32
	DB_proc_putGuildPet,							// 33
	DB_proc_delGuildPetInRoom,						// 34
	DB_proc_toPuk3,									// 35

	DB_proc_getCharList_Result,						// 0
	DB_proc_createNewChar_Result,					// 1
	DB_proc_CharLogin_Result,						// 2
	DB_proc_CharLogout_Result,						// 3
	DB_proc_CharSave_Result,						// 4
	DB_proc_UnLock_Result,							// 5
	DB_proc_AskJob_Result,							// 6
	DB_proc_getSkillReasonablePopulation_Result,	// 7
	DB_proc_SaveJob_Result,							// 8
	DB_proc_ClrBadMsg_Result,						// 9
	DB_proc_UpdateDuelRanking_Result,				// 10
	DB_proc_GetNormalDuelRanking_Result,			// 11
	DB_proc_GetMyDuelRanking_Result,				// 12
	DB_proc_CharDelete_Result,						// 13
	DB_proc_Graduation_Result,						// 14
	DB_proc_Cdkeysearch_Result,						// 15
	DB_proc_Changeisaoid_Result,					// 16
	DB_proc_Isaosearch_Result,						// 17
	DB_proc_Settrial_Result,						// 18
	DB_proc_MoveChar_Result,						// 19
	DB_proc_RestoreChar_Result,						// 20
	DB_proc_ResetProductKey_Result,					// 21
	DB_proc_createGuild_Result,						// 22
	DB_proc_guildInfo_Result,						// 23
	DB_proc_killGuild_Result,						// 24
	DB_proc_getGuildList_Result,					// 25
	DB_proc_writeOffGuild_Result,					// 26
	DB_proc_toPuk2_Result,							// 27
	DB_proc_setGuildTitle_Result,					// 28
	DB_proc_clearGuildMonsterCloseness_Result,		// 29
	DB_proc_getGuildPetList_Result,					// 30
	DB_proc_getGuildPet_Result,						// 31
	DB_proc_putGuildPet_Result,						// 32
	DB_proc_delGuildPetInRoom_Result,				// 33
	DB_proc_toPuk3_Result,							// 34
};													// 35