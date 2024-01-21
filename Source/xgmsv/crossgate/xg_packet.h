#pragma once

#include "core/shared.h"
#include "core/network/packet_queue.h"

namespace crossgate
{
	struct xg_session;

	struct xg_packet : public core::packet
	{
		xg_packet(core::packet&& packet) :
			core::packet{ packet }
		{}

		xg_packet(uint8* data, uint64_t size, bool raw = false) :
			core::packet{ .data = data, .size = size, .raw = raw }
		{}

        void r(const char** str);
        void r(int32* i32);
        void r(int64* i64);
        void r(uint32* u32);
        void r(uint64* u64);

		void w( const char* str );
		void w( int32 i32 );
		void w( int64 i64 );
		void w( uint32 u32 );
		void w( uint64 u64 );
		void end();
	};

	using opcode_handler = void (*)( xg_session* session, xg_packet* packet );
	struct opcode_entry
	{
		enum xg_opcode opcode;
		const char* opstring;
		uint32 opstring_size;
		opcode_handler handler;
	};

	struct opcode_obfuscation_entry
	{
		enum xg_opcode opcode;
		const char* plain_op;
		uint32 plain_op_size;
		const char* recv_op;
		uint32 recv_op_size;
		const char* send_op;
		uint32 send_op_size;
	};

	enum xg_opcode
	{
		XG_W,
		XG_w,
		XG_EV,
		XG_M,
		XG_EN,
		XG_DU,
		XG_EO,
		XG_BU,
		XG_JB,
		XG_LB,
		XG_B,
		XG_ID,
		XG_IDF,
		XG_PI,
		XG_DI,
		XG_DG,
		XG_DP,
		XG_MI,
		XG_IR,
		XG_MSG,
		XG_ALI,
		XG_ALN,
		XG_PMSG,
		XG_DAB,
		XG_AAB,
		XG_GI,
		XG_GT,
		XG_GMR,
		XG_BGT,
		XG_AGM,
		XG_LG,
		XG_RGM,
		XG_GML,
		XG_PGML,
		XG_GD,
		XG_PRV,
		XG_PRS,
		XG_PRD,
		XG_PRE,
		XG_PRM,
		XG_PRO,
		XG_L,
		XG_TK,
		XG_FS,
		XG_HL,
		XG_PR,
		XG_KS,
		XG_MP,
		XG_GFL,
		XG_GPD,
		XG_GFLI,
		XG_GPDI,
		XG_IH,
		XG_AC,
		XG_ACS,
		XG_MU,
		XG_TU,
		XG_TRPL,
		XG_TRS,
		XG_TROP,
		XG_TRCL,
		XG_TROC,
		XG_PS,
		XG_ST,
		XG_DT,
		XG_FT,
		XG_LVUP,
		XG_PLVUP,
		XG_SKSW,
		XG_PSSW,
		XG_POS,
		XG_KN,
		XG_WN,
		XG_SP,
		XG_ClientLogin,
		XG_CreateNewChar,
		XG_CharDelete,
		XG_CharLogin,
		XG_CharList,
		XG_CharLogout,
		XG_ProcGet,
		XG_PlayerNumGet,
		XG_Echo,
		XG_Shutdown,
		XG_FC,
		XG_CH,
		XG_CharLoginGate,
		XG_PVUP,
		XG_StallStart,
		XG_StallEnd,
		XG_StallBrowse,
		XG_StallBuy,
		XG_MAC,
		XG_PV,
		XG_CC,
		XG_ES,
		XG_PLAYSE,
		XG_SH,
		XG_PC,
		XG_BGMW,
		XG_SE,
		XG_TRLG,
		XG_GC,
		XG_ALO,
		XG_BT,
		XG_SI,
		XG_LI,
		XG_EP,
		XG_MC,
		XG_XYD,
		XG_IP,
		XG_MN,
		XG_EF,
		XG_TRLPS,
		XG_TRLP,
		XG_TRLI,
		XG_S,
		XG_PT,
		XG_CT,
		XG_CS,
		XG_CJ,
		XG_CD,
		XG_CA,
		XG_TITLE,
		XG_CN,
		XG_C,
		XG_PP,
		XG_KP2,
		XG_KP,
		XG_CP2,
		XG_CP,
		XG_STK,
		XG_PRAD,
		XG_PRW,
		XG_PRA,
		XG_PRL,
		XG_GMI,
		XG_GM,
		XG_ABI,
		XG_ABG,
		XG_AB,
		XG_PME,
		XG_AL,
		XG_I,
		XG_IA,
		XG_RD,
		XG_RS,

		XG_OPCODE_COUNT = 147
	};

	extern opcode_entry xg_recv_opcodes[XG_OPCODE_COUNT];
	extern opcode_obfuscation_entry xg_opcodes[XG_OPCODE_COUNT];

	template<typename T, size_t N>
	int mystrncmp(const T* a, const T(&b)[N])
	{
		return _tcsnccmp(a, b, N - 1);
	}

	void xg_recv(xg_session* session, core::packet&& packet);


	xg_packet xg_begin_packet( xg_session* session, xg_opcode op );

	bool xg_send( xg_session* session, uint8* packet_data, uint32 packet_size, bool raw = false );
	bool xg_send( xg_session* session, xg_packet packet, bool raw = false );
}
