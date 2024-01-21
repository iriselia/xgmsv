#include "globals.h"
#include "xg_packet.h"
#include "xg_session.h"
#include "xg_recv.h"
#include "xg_send.h"

namespace crossgate
{
#ifdef _MSC_VER
#define atoll(S) _atoi64(S)
#endif

    void xg_packet::r(const char** str)
    {
		*str = (char*)data;
        while ( *data != ' ' && *data != '\n' && *data != '\r' ) data++; data++;
    }
    void xg_packet::r(int32* i32)
    {
		*i32 = atoi((char*)data);
        while ( *data != ' ' && *data != '\n' && *data != '\r' ) data++; data++;
    }
    void xg_packet::r(int64* i64)
    {
		*i64 = atoll((char*)data);
        while ( *data != ' ' && *data != '\n' && *data != '\r' ) data++; data++;
    }
    void xg_packet::r(uint32* u32)
    {
		*u32 = strtoul((char*)data, 0, 10);
        while ( *data != ' ' && *data != '\n' && *data != '\r' ) data++; data++;
    }
    void xg_packet::r(uint64* u64)
    {
        *u64 = strtoull((char*)data, 0, 10);
        while ( *data != ' ' && *data != '\n' && *data != '\r' ) data++; data++;
    }

    void xg_packet::w(const char* str)
    {
        size += snprintf((char*)data + size, xg_packet_capacity - size, "%s ", str);
    }
    void xg_packet::w(int32 i32)
    {
        size += snprintf((char*)data + size, xg_packet_capacity - size, "%d ", i32);
    }
    void xg_packet::w(int64 i64)
    {
        size += snprintf((char*)data + size, xg_packet_capacity - size, "%d ", i64);
    }
    void xg_packet::w(uint32 u32)
    {
        size += snprintf((char*)data + size, xg_packet_capacity - size, "%u ", u32);
    }
    void xg_packet::w(uint64 u64)
    {
        size += snprintf((char*)data + size, xg_packet_capacity - size, "%u ", u64);
    }

    void xg_packet::end()
    {
        size += snprintf((char*)data + size, xg_packet_capacity - size, "\n");
    }

#define add_opcode_entry(opcode, opstring, handler) \
	{ opcode, opstring, sizeof(opstring) / sizeof(char) - 1, handler }

	opcode_entry xg_recv_opcodes[XG_OPCODE_COUNT] =
	{
		add_opcode_entry( XG_W,					"W",				&recv_W ),
		add_opcode_entry( XG_w,					"w",				&recv_w ),
		add_opcode_entry( XG_EV,				"EV",				&recv_EV ),
		add_opcode_entry( XG_M,					"M",				&recv_M ),
		add_opcode_entry( XG_EN,				"EN",				&recv_EN ),
		add_opcode_entry( XG_DU,				"DU",				&recv_DU ),
		add_opcode_entry( XG_EO,				"EO",				&recv_EO ),
		add_opcode_entry( XG_BU,				"BU",				&recv_BU ),
		add_opcode_entry( XG_JB,				"JB",				&recv_JB ),
		add_opcode_entry( XG_LB,				"LB",				&recv_LB ),
		add_opcode_entry( XG_B,					"B",				&recv_B ),
		add_opcode_entry( XG_ID,				"ID",				&recv_ID ),
		add_opcode_entry( XG_IDF,				"IDF",				&recv_IDF ),
		add_opcode_entry( XG_PI,				"PI",				&recv_PI ),
		add_opcode_entry( XG_DI,				"DI",				&recv_DI ),
		add_opcode_entry( XG_DG,				"DG",				&recv_DG ),
		add_opcode_entry( XG_DP,				"DP",				&recv_DP ),
		add_opcode_entry( XG_MI,				"MI",				&recv_MI ),
		add_opcode_entry( XG_IR,				"IR",				&recv_IR ),
		add_opcode_entry( XG_MSG,				"MSG",				&recv_MSG ),
		add_opcode_entry( XG_ALI,				"ALI",				&recv_ALI ),
		add_opcode_entry( XG_ALN,				"ALN",				&recv_ALN ),
		add_opcode_entry( XG_PMSG,				"PMSG",				&recv_PMSG ),
		add_opcode_entry( XG_DAB,				"DAB",				&recv_DAB ),
		add_opcode_entry( XG_AAB,				"AAB",				&recv_AAB ),
		add_opcode_entry( XG_GI,				"GI",				&recv_GI ),
		add_opcode_entry( XG_GT,				"GT",				&recv_GT ),
		add_opcode_entry( XG_GMR,				"GMR",				&recv_GMR ),
		add_opcode_entry( XG_BGT,				"BGT",				&recv_BGT ),
		add_opcode_entry( XG_AGM,				"AGM",				&recv_AGM ),
		add_opcode_entry( XG_LG,				"LG",				&recv_LG ),
		add_opcode_entry( XG_RGM,				"RGM",				&recv_RGM ),
		add_opcode_entry( XG_GML,				"GML",				&recv_GML ),
		add_opcode_entry( XG_PGML,				"PGML",				&recv_PGML ),
		add_opcode_entry( XG_GD,				"GD",				&recv_GD ),
		add_opcode_entry( XG_PRV,				"PRV",				&recv_PRV ),
		add_opcode_entry( XG_PRS,				"PRS",				&recv_PRS ),
		add_opcode_entry( XG_PRD,				"PRD",				&recv_PRD ),
		add_opcode_entry( XG_PRE,				"PRE",				&recv_PRE ),
		add_opcode_entry( XG_PRM,				"PRM",				&recv_PRM ),
		add_opcode_entry( XG_PRO,				"PRO",				&recv_PRO ),
		add_opcode_entry( XG_L,					"L",				&recv_L ),
		add_opcode_entry( XG_TK,				"TK",				&recv_TK ),
		add_opcode_entry( XG_FS,				"FS",				&recv_FS ),
		add_opcode_entry( XG_HL,				"HL",				&recv_HL ),
		add_opcode_entry( XG_PR,				"PR",				&recv_PR ),
		add_opcode_entry( XG_KS,				"KS",				&recv_KS ),
		add_opcode_entry( XG_MP,				"MP",				&recv_MP ),
		add_opcode_entry( XG_GFL,				"GFL",				&recv_GFL ),
		add_opcode_entry( XG_GPD,				"GPD",				&recv_GPD ),
		add_opcode_entry( XG_GFLI,				"GFLI",				&recv_GFLI ),
		add_opcode_entry( XG_GPDI,				"GPDI",				&recv_GPDI ),
		add_opcode_entry( XG_IH,				"IH",				&recv_IH ),
		add_opcode_entry( XG_AC,				"AC",				&recv_AC ),
		add_opcode_entry( XG_ACS,				"ACS",				&recv_ACS ),
		add_opcode_entry( XG_MU,				"MU",				&recv_MU ),
		add_opcode_entry( XG_TU,				"TU",				&recv_TU ),
		add_opcode_entry( XG_TRPL,				"TRPL",				&recv_TRPL ),
		add_opcode_entry( XG_TRS,				"TRS",				&recv_TRS ),
		add_opcode_entry( XG_TROP,				"TROP",				&recv_TROP ),
		add_opcode_entry( XG_TRCL,				"TRCL",				&recv_TRCL ),
		add_opcode_entry( XG_TROC,				"TROC",				&recv_TROC ),
		add_opcode_entry( XG_PS,				"PS",				&recv_PS ),
		add_opcode_entry( XG_ST,				"ST",				&recv_ST ),
		add_opcode_entry( XG_DT,				"DT",				&recv_DT ),
		add_opcode_entry( XG_FT,				"FT",				&recv_FT ),
		add_opcode_entry( XG_LVUP,				"LVUP",				&recv_LVUP ),
		add_opcode_entry( XG_PLVUP,				"PLVUP",			&recv_PLVUP ),
		add_opcode_entry( XG_SKSW,				"SKSW",				&recv_SKSW ),
		add_opcode_entry( XG_PSSW,				"PSSW",				&recv_PSSW ),
		add_opcode_entry( XG_POS,				"POS",				&recv_POS ),
		add_opcode_entry( XG_KN,				"KN",				&recv_KN ),
		add_opcode_entry( XG_WN,				"WN",				&recv_WN ),
		add_opcode_entry( XG_SP,				"SP",				&recv_SP ),
		add_opcode_entry( XG_ClientLogin,		"ClientLogin",		&recv_ClientLogin ),
		add_opcode_entry( XG_CreateNewChar,		"CreateNewChar",	&recv_CreateNewChar ),
		add_opcode_entry( XG_CharDelete,		"CharDelete",		&recv_CharDelete ),
		add_opcode_entry( XG_CharLogin,			"CharLogin",		&recv_CharLogin ),
		add_opcode_entry( XG_CharList,			"CharList",			&recv_CharList ),
		add_opcode_entry( XG_CharLogout,		"CharLogout",		&recv_CharLogout ),
		add_opcode_entry( XG_ProcGet,			"ProcGet",			&recv_ProcGet ),
		add_opcode_entry( XG_PlayerNumGet,		"PlayerNumGet",		&recv_PlayerNumGet ),
		add_opcode_entry( XG_Echo,				"Echo",				&recv_Echo ),
		add_opcode_entry( XG_Shutdown,			"Shutdown",			&recv_Shutdown ),
		add_opcode_entry( XG_FC,				"FC",				&recv_FC ),
		add_opcode_entry( XG_CH,				"CH",				&recv_CH ),
		add_opcode_entry( XG_CharLoginGate,		"CharLoginGate",	&recv_CharLoginGate ),
		add_opcode_entry( XG_PVUP,				"PVUP",				&recv_PVUP ),
		add_opcode_entry( XG_StallStart,		"StallStart",		&recv_StallStart ),
		add_opcode_entry( XG_StallEnd,			"StallEnd",			&recv_StallEnd ),
		add_opcode_entry( XG_StallBrowse,		"StallBrowse",		&recv_StallBrowse ),
		add_opcode_entry( XG_StallBuy,			"StallBuy",			&recv_StallBuy )
	};

#define add_opcode_obfuscation_entry(opcode, opcode_plain, opcode_recv_obfuscated, opcode_send_obfuscated) \
	{ opcode, opcode_plain, sizeof(opcode_plain) / sizeof(char) - 1, opcode_recv_obfuscated, sizeof(opcode_recv_obfuscated) / sizeof(char) - 1, opcode_send_obfuscated, sizeof(opcode_send_obfuscated) / sizeof(char) - 1 }

	opcode_obfuscation_entry xg_opcodes[XG_OPCODE_COUNT] =
	{
		add_opcode_obfuscation_entry( XG_W,				"W",				"zA",				"" ),
		add_opcode_obfuscation_entry( XG_w,             "w",				"sz",				"" ),
		add_opcode_obfuscation_entry( XG_EV,            "EV",				"pg",				"EZ" ),
		add_opcode_obfuscation_entry( XG_M,             "M",				"UUN",				"aBS" ),
		add_opcode_obfuscation_entry( XG_EN,            "EN",				"KeOX",				"dAgg" ),
		add_opcode_obfuscation_entry( XG_DU,            "DU",				"ik",				"" ),
		add_opcode_obfuscation_entry( XG_EO,            "EO",				"eL",				"" ),
		add_opcode_obfuscation_entry( XG_BU,            "BU",				"NgMy",				"" ),
		add_opcode_obfuscation_entry( XG_JB,            "JB",				"cEj",				"" ),
		add_opcode_obfuscation_entry( XG_LB,            "LB",				"IMv",				"" ),
		add_opcode_obfuscation_entry( XG_B,             "B",				"Rg",				"LYIm" ),
		add_opcode_obfuscation_entry( XG_ID,            "ID",				"Ak",				"" ),
		add_opcode_obfuscation_entry( XG_IDF,           "IDF",				"iVfo",				"" ),
		add_opcode_obfuscation_entry( XG_PI,            "PI",				"fIM",				"" ),
		add_opcode_obfuscation_entry( XG_DI,            "DI",				"QpfE",				"" ),
		add_opcode_obfuscation_entry( XG_DG,            "DG",				"woz",				"" ),
		add_opcode_obfuscation_entry( XG_DP,            "DP",				"FvWm",				"" ),
		add_opcode_obfuscation_entry( XG_MI,            "MI",				"yi",				"" ),
		add_opcode_obfuscation_entry( XG_IR,            "IR",				"fS",				"xmgQ" ),
		add_opcode_obfuscation_entry( XG_MSG,           "MSG",				"CV",				"Mhbt" ),
		add_opcode_obfuscation_entry( XG_ALI,           "ALI",				"kza",				"dM" ),
		add_opcode_obfuscation_entry( XG_ALN,           "ALN",				"LaR",				"Dn" ),
		add_opcode_obfuscation_entry( XG_PMSG,          "PMSG",				"rMsv",				"" ),
		add_opcode_obfuscation_entry( XG_DAB,           "DAB",				"FPkR",				"" ),
		add_opcode_obfuscation_entry( XG_AAB,           "AAB",				"vcAz",				"" ),
		add_opcode_obfuscation_entry( XG_GI,            "GI",				"Ufm",				"aDn" ),
		add_opcode_obfuscation_entry( XG_GT,            "GT",				"GY",				"znv" ),
		add_opcode_obfuscation_entry( XG_GMR,           "GMR",				"CZ",				"" ),
		add_opcode_obfuscation_entry( XG_BGT,           "BGT",				"uHb",				"" ),
		add_opcode_obfuscation_entry( XG_AGM,           "AGM",				"npzi",				"" ),
		add_opcode_obfuscation_entry( XG_LG,            "LG",				"XIsR",				"" ),
		add_opcode_obfuscation_entry( XG_RGM,           "RGM",				"dbQC",				"oRut" ),
		add_opcode_obfuscation_entry( XG_GML,           "GML",				"uId",				"Rll" ),
		add_opcode_obfuscation_entry( XG_PGML,          "PGML",				"PfqV",				"" ),
		add_opcode_obfuscation_entry( XG_GD,            "GD",				"nkda",				"frQp" ),
		add_opcode_obfuscation_entry( XG_PRV,           "PRV",				"HoW",				"HNP" ),
		add_opcode_obfuscation_entry( XG_PRS,           "PRS",				"Ot",				"" ),
		add_opcode_obfuscation_entry( XG_PRD,           "PRD",				"TLKH",				"DpSY" ),
		add_opcode_obfuscation_entry( XG_PRE,           "PRE",				"wD",				"cNB" ),
		add_opcode_obfuscation_entry( XG_PRM,           "PRM",				"GH",				"ow" ),
		add_opcode_obfuscation_entry( XG_PRO,           "PRO",				"lz",				"" ),
		add_opcode_obfuscation_entry( XG_L,             "L",				"nB",				"" ),
		add_opcode_obfuscation_entry( XG_TK,            "TK",				"uSr",				"zdr" ),
		add_opcode_obfuscation_entry( XG_FS,            "FS",				"dcMv",				"SkBD" ),
		add_opcode_obfuscation_entry( XG_HL,            "HL",				"FVm",				"sZWu" ),
		add_opcode_obfuscation_entry( XG_PR,            "PR",				"zn",				"bNY" ),
		add_opcode_obfuscation_entry( XG_KS,            "KS",				"LKQy",				"" ),
		add_opcode_obfuscation_entry( XG_MP,            "MP",				"Ukuy",				"" ),
		add_opcode_obfuscation_entry( XG_GFL,           "GFL",				"iET",				"ysVc" ),
		add_opcode_obfuscation_entry( XG_GPD,           "GPD",				"McVa",				"ND" ),
		add_opcode_obfuscation_entry( XG_GFLI,          "GFLI",				"as",				"LY" ),
		add_opcode_obfuscation_entry( XG_GPDI,          "GPDI",				"mjCv",				"nFV" ),
		add_opcode_obfuscation_entry( XG_IH,            "IH",				"BG",				"" ),
		add_opcode_obfuscation_entry( XG_AC,            "AC",				"MIS",				"" ),
		add_opcode_obfuscation_entry( XG_ACS,           "ACS",				"klF",				"" ),
		add_opcode_obfuscation_entry( XG_MU,            "MU",				"RXr",				"" ),
		add_opcode_obfuscation_entry( XG_TU,            "TU",				"sM",				"JgO" ),
		add_opcode_obfuscation_entry( XG_TRPL,          "TRPL",				"ixH",				"Cxo" ),
		add_opcode_obfuscation_entry( XG_TRS,           "TRS",				"QeuC",				"ms" ),
		add_opcode_obfuscation_entry( XG_TROP,          "TROP",				"aiAb",				"NdM" ),
		add_opcode_obfuscation_entry( XG_TRCL,          "TRCL",				"suI",				"xEg" ),
		add_opcode_obfuscation_entry( XG_TROC,          "TROC",				"OU",				"We" ),
		add_opcode_obfuscation_entry( XG_PS,            "PS",				"Flvp",				"BHe" ),
		add_opcode_obfuscation_entry( XG_ST,            "ST",				"zGp",				"" ),
		add_opcode_obfuscation_entry( XG_DT,            "DT",				"tj",				"" ),
		add_opcode_obfuscation_entry( XG_FT,            "FT",				"ACwB",				"" ),
		add_opcode_obfuscation_entry( XG_LVUP,          "LVUP",				"IHw",				"kg" ),
		add_opcode_obfuscation_entry( XG_PLVUP,         "PLVUP",			"kjSK",				"sA" ),
		add_opcode_obfuscation_entry( XG_SKSW,          "SKSW",				"Noeb",				"" ),
		add_opcode_obfuscation_entry( XG_PSSW,          "PSSW",				"HJQp",				"" ),
		add_opcode_obfuscation_entry( XG_POS,           "POS",				"ih",				"Ov" ),
		add_opcode_obfuscation_entry( XG_KN,            "KN",				"ycX",				"" ),
		add_opcode_obfuscation_entry( XG_WN,            "WN",				"xD",				"BZ" ),
		add_opcode_obfuscation_entry( XG_SP,            "SP",				"cZt",				"" ),
		add_opcode_obfuscation_entry( XG_ClientLogin,   "ClientLogin",		"JFVf",				"whV" ),
		add_opcode_obfuscation_entry( XG_CreateNewChar, "CreateNewChar",	"aSEl",				"DZ" ),
		add_opcode_obfuscation_entry( XG_CharDelete,    "CharDelete",		"Godf",				"oy" ),
		add_opcode_obfuscation_entry( XG_CharLogin,     "CharLogin",		"emRV",				"hPUd" ),
		add_opcode_obfuscation_entry( XG_CharList,      "CharList",			"Gp",				"Daj" ),
		add_opcode_obfuscation_entry( XG_CharLogout,    "CharLogout",		"WA",				"HMNy" ),
		add_opcode_obfuscation_entry( XG_ProcGet,       "ProcGet",			"yp",				"cJzI" ),
		add_opcode_obfuscation_entry( XG_PlayerNumGet,  "PlayerNumGet",		"rvb",				"Bi" ),
		add_opcode_obfuscation_entry( XG_Echo,          "Echo",				"Or",				"LOh" ),
		add_opcode_obfuscation_entry( XG_Shutdown,      "Shutdown",			"IcE",				"" ),
		add_opcode_obfuscation_entry( XG_FC,            "FC",				"DE",				"" ),
		add_opcode_obfuscation_entry( XG_CH,            "CH",				"PGXE",				"" ),
		add_opcode_obfuscation_entry( XG_CharLoginGate, "CharLoginGate",	"lO",				"" ),
		add_opcode_obfuscation_entry( XG_PVUP,          "PVUP",				"Xoee",				"ed" ),
		add_opcode_obfuscation_entry( XG_StallStart,    "StallStart",		"ylGN",				"Tmrx" ),
		add_opcode_obfuscation_entry( XG_StallEnd,      "StallEnd",			"JMh",				"KXhH" ),
		add_opcode_obfuscation_entry( XG_StallBrowse,   "StallBrowse",		"ElVN",				"ft" ),
		add_opcode_obfuscation_entry( XG_StallBuy,      "StallBuy",			"wAo",				"Yx" ),
		add_opcode_obfuscation_entry( XG_MAC,           "MAC",				"",					"tq" ),
		add_opcode_obfuscation_entry( XG_PV,            "PV",				"",					"PfM" ),
		add_opcode_obfuscation_entry( XG_CC,            "CC",				"",					"GmNM" ),
		add_opcode_obfuscation_entry( XG_ES,            "ES",				"",					"ftaN" ),
		add_opcode_obfuscation_entry( XG_PLAYSE,        "PLAYSE",			"",					"murD" ),
		add_opcode_obfuscation_entry( XG_SH,            "SH",				"",					"PVfz" ),
		add_opcode_obfuscation_entry( XG_PC,            "PC",				"",					"bJg" ),
		add_opcode_obfuscation_entry( XG_BGMW,          "BGMW",				"",					"wiB" ),
		add_opcode_obfuscation_entry( XG_SE,            "SE",				"",					"TFCu" ),
		add_opcode_obfuscation_entry( XG_TRLG,          "TRLG",				"",					"hJq" ),
		add_opcode_obfuscation_entry( XG_GC,            "GC",				"",					"GC" ),
		add_opcode_obfuscation_entry( XG_ALO,           "ALO",				"",					"IYo" ),
		add_opcode_obfuscation_entry( XG_BT,            "BT",				"",					"MTg" ),
		add_opcode_obfuscation_entry( XG_SI,            "SI",				"",					"vWpn" ),
		add_opcode_obfuscation_entry( XG_LI,            "LI",				"",					"PudO" ),
		add_opcode_obfuscation_entry( XG_EP,            "EP",				"",					"OaHi" ),
		add_opcode_obfuscation_entry( XG_MC,            "MC",				"",					"jmb" ),
		add_opcode_obfuscation_entry( XG_XYD,           "XYD",				"",					"QBt" ),
		add_opcode_obfuscation_entry( XG_IP,            "IP",				"",					"lSZ" ),
		add_opcode_obfuscation_entry( XG_MN,            "MN",				"",					"TeD" ),
		add_opcode_obfuscation_entry( XG_EF,            "EF",				"",					"zzd" ),
		add_opcode_obfuscation_entry( XG_TRLPS,         "TRLPS",			"",					"Yq" ),
		add_opcode_obfuscation_entry( XG_TRLP,          "TRLP",				"",					"RwK" ),
		add_opcode_obfuscation_entry( XG_TRLI,          "TRLI",				"",					"nUbJ" ),
		add_opcode_obfuscation_entry( XG_S,             "S",				"",					"Yo" ),
		add_opcode_obfuscation_entry( XG_PT,            "PT",				"",					"OBee" ),
		add_opcode_obfuscation_entry( XG_CT,            "CT",				"",					"Rspn" ),
		add_opcode_obfuscation_entry( XG_CS,            "CS",				"",					"cNE" ),
		add_opcode_obfuscation_entry( XG_CJ,            "CJ",				"",					"jv" ),
		add_opcode_obfuscation_entry( XG_CD,            "CD",				"",					"uSB" ),
		add_opcode_obfuscation_entry( XG_CA,            "CA",				"",					"NI" ),
		add_opcode_obfuscation_entry( XG_TITLE,         "TITLE",			"",					"Sc" ),
		add_opcode_obfuscation_entry( XG_CN,            "CN",				"",					"JJQj" ),
		add_opcode_obfuscation_entry( XG_C,             "C",				"",					"nsY" ),
		add_opcode_obfuscation_entry( XG_PP,            "PP",				"",					"TNT" ),
		add_opcode_obfuscation_entry( XG_KP2,           "KP2",				"",					"AU" ),
		add_opcode_obfuscation_entry( XG_KP,            "KP",				"",					"IsSy" ),
		add_opcode_obfuscation_entry( XG_CP2,           "CP2",				"",					"UU" ),
		add_opcode_obfuscation_entry( XG_CP,            "CP",				"",					"aPc" ),
		add_opcode_obfuscation_entry( XG_STK,           "STK",				"",					"Zd" ),
		add_opcode_obfuscation_entry( XG_PRAD,          "PRAD",				"",					"KyN" ),
		add_opcode_obfuscation_entry( XG_PRW,           "PRW",				"",					"DyAG" ),
		add_opcode_obfuscation_entry( XG_PRA,           "PRA",				"",					"IUn" ),
		add_opcode_obfuscation_entry( XG_PRL,           "PRL",				"",					"eZT" ),
		add_opcode_obfuscation_entry( XG_GMI,           "GMI",				"",					"Kka" ),
		add_opcode_obfuscation_entry( XG_GM,            "GM",				"",					"yuD" ),
		add_opcode_obfuscation_entry( XG_ABI,           "ABI",				"",					"vI" ),
		add_opcode_obfuscation_entry( XG_ABG,           "ABG",				"",					"PhdQ" ),
		add_opcode_obfuscation_entry( XG_AB,            "AB",				"",					"gnxd" ),
		add_opcode_obfuscation_entry( XG_PME,           "PME",				"",					"lDuJ" ),
		add_opcode_obfuscation_entry( XG_AL,            "AL",				"",					"Ilz" ),
		add_opcode_obfuscation_entry( XG_I,             "I",				"",					"rU" ),
		add_opcode_obfuscation_entry( XG_IA,            "IA",				"",					"ELoy" ),
		add_opcode_obfuscation_entry( XG_RD,            "RD",				"",					"hB" ),
		add_opcode_obfuscation_entry( XG_RS,            "RS",				"",					"Gn" ),
	};

	void xg_recv(xg_session* session, core::packet&& cpacket)
	{
		xg_packet packet(std::move(cpacket));

		for (uint32 i = 0; i < XG_OPCODE_COUNT; i++)
		{
			if (xg_opcodes[i].recv_op_size != 0 &&
				strncmp( (char*)packet.data, xg_opcodes[i].recv_op, xg_opcodes[i].recv_op_size) == 0)
			{
				xg_log( "recv[%d]:\"%s(%s) %.*s\"\n", cpacket.size, xg_opcodes[i].plain_op, xg_opcodes[i].recv_op, cpacket.size - xg_opcodes[i].recv_op_size - 1, cpacket.data + xg_opcodes[i].recv_op_size + 1 );


				auto handler = xg_recv_opcodes[i].handler;
				( *handler )( session, &packet );
				return;
			}
		}
		printf("Unhandled packet:%s\n", packet.data);
	}

	crossgate::xg_packet xg_begin_packet( xg_session* session, xg_opcode op )
	{
		xg_packet p{ session->scratch_buffer.data(), 0, false };
		memcpy( p.data, xg_opcodes[op].send_op, xg_opcodes[op].send_op_size );
		p.data[xg_opcodes[op].send_op_size] = ' ';
		p.size += xg_opcodes[op].send_op_size + 1;
		return p;
	}

	bool xg_send( xg_session* session, uint8* packet_data, uint32 packet_size, bool raw /*= false */ )
	{
		if (!session->wqueue.enqueue( packet_data, packet_size, raw ))
		{
			assert( false );
			printf( "enqueue failed.\n" );
			return false;
		}

		session->signal.fire();

		return true;
	}

	bool xg_send( xg_session* session, xg_packet packet, bool raw /*= false */ )
	{
		for (uint32 i = 0; i < XG_OPCODE_COUNT; i++)
		{
			if (xg_opcodes[i].send_op_size != 0 &&
				strncmp( (char*)packet.data, xg_opcodes[i].send_op, xg_opcodes[i].send_op_size ) == 0)
			{
				xg_log( "send[%d]:\"%s(%s) %.*s\"\n", packet.size, xg_opcodes[i].plain_op, xg_opcodes[i].send_op, packet.size - xg_opcodes[i].send_op_size - 1, packet.data + xg_opcodes[i].send_op_size + 1 );
				break;
			}
		}

		packet.end();

		if (!session->wqueue.enqueue( packet.data, packet.size, raw ))
		{
			assert( false );
			printf( "enqueue failed.\n" );
			return false;
		}

		session->signal.fire();

		return true;
	}

}

