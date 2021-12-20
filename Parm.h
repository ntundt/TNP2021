#pragma once

#include "stdafx.h"

#define PARM_IN
#define PARM_OUT
#define PARM_LOG
#define PARM_MAX_SIZE 300
#define PARM_OUT_DEFAULT_EXT L".out"
#define PARM_LOG_DEFAULT_EXT L".log"

namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE] = L"";
		wchar_t out[PARM_MAX_SIZE] = L"";
		wchar_t log[PARM_MAX_SIZE] = L"";
		bool output = true;
		bool compileAsm = false;
	};

	PARM getparm(int argc, _TCHAR* argv[]);
}