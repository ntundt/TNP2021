#pragma once

#include "stdafx.h"

#include "Greibach.h"
#include "LT.h"

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

class MFSTSTACK : public std::stack<short>
{
public:
	using std::stack<short>::c;
};

namespace MFST
{
	struct MfstState
	{
		short lenta_position;
		short nrule;
		short nrulechain;
		MFSTSTACK st;
		MfstState();
		MfstState(short pposition, MFSTSTACK pst, short pnrulechain);
		MfstState(short pposition, MFSTSTACK pst, short pnrule, short pnrulechain);
	};

	class MFSTSTATE : public std::stack<MfstState>
	{
	public:
		using std::stack<MfstState>::c;
	};

	struct Mfst
	{
		enum class RC_STEP
		{
			NS_OK,
			NS_NORULE,
			NS_NORULECHAIN,
			NS_ERROR,
			TS_OK,
			TS_NOK,
			LENTA_END,
			SURPRISE
		};

		struct MfstDiagnosis
		{
			short lenta_position;
			RC_STEP rc_step;
			short nrule;
			short nrule_chain;
			MfstDiagnosis();
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);
		} diagnosis[MFST_DIAGN_NUMBER];

		GRBALPHABET* lenta;

		short lenta_position;
		short nrule;
		short nrulechain;
		short lenta_size;
		
		GRB::Greibach grebach;
		LT::LexTable lex;
		MFSTSTACK st;
		MFSTSTATE storestate;
		
		Mfst();
		Mfst(LT::LexTable lextable, GRB::Greibach pgrebach);
		
		char* getCSt(char* buf);
		char* getCLenta(char* buf, short pos, short n = 25);
		char* getDiagnosis(short n, char* buf);
		
		bool savestate();
		bool reststate();
		bool push_chain(GRB::Rule::Chain chain);
		
		RC_STEP step();
		
		bool start();
		bool savediagnosis(RC_STEP pprc_step);
		void printrules();
		
		struct Deducation
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation() { size = 0; nrules = 0; nrulechains = 0; }
		} deducation;
		
		bool savededucation();
	};

}