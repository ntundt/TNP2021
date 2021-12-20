#pragma once

#include "stdafx.h"

#include "Parm.h"
#include "Error.h"

namespace Parm {

	PARM getparm(int argc, _TCHAR* argv[])
	{
		PARM* result = new PARM;

		for (int i = 0; i < argc; i++)
		{
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
			{
				throw ERROR_THROW(104);
			}

			if (wcscmp(argv[i], L"--in") == 0 || wcscmp(argv[i], L"-i") == 0)
			{
				wcscpy_s(result->in, argv[i + 1]);
			}
			else if (wcscmp(argv[i], L"--out") == 0 || wcscmp(argv[i], L"-o") == 0)
			{
				wcscpy_s(result->out, argv[i + 1]);
			}
			else if (wcscmp(argv[i], L"--log") == 0 || wcscmp(argv[i], L"-l") == 0)
			{
				wcscpy_s(result->log, argv[i + 1]);
			}
			else if (wcscmp(argv[i], L"--nooutput") == 0)
			{
				result->output = false;
			}
			else if (wcscmp(argv[i], L"--asmcompile") == 0)
			{
				result->compileAsm = true;
			}
		}

		if (wcscmp(result->in, L"") == 0)
		{
			throw ERROR_THROW(100);
		}
		if (wcscmp(result->out, L"") == 0)
		{
			wcscpy_s(result->out, result->in);
			wcsncat_s(result->out, L".out", PARM_MAX_SIZE);
		}
		if (wcscmp(result->log, L"") == 0)
		{
			wcscpy_s(result->log, result->in);
			wcsncat_s(result->log, L".log", PARM_MAX_SIZE);
		}
		return *result;
	}

}