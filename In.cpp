#pragma once

#include "stdafx.h"

#include "In.h"
#include "Error.h"

namespace In {

	IN getin(wchar_t infile[])
	{
		IN* result = new IN;

		result->ignor = 0;
		result->lines = 0;
		result->size = 0;

		int column = 0;
		std::ifstream s(infile);
		int fsize = 0;
		if (!s.is_open() or s.fail())
		{
			throw ERROR_THROW(110);
		}
		while (!s.eof())
		{
			s.get();
			fsize++;
		}
		fsize++;
		result->text = new unsigned char[fsize];
		s.close();

		std::ifstream input(infile);
		unsigned char b;
		int i = 0;
		while (!input.eof())
		{
			b = input.get();
			if (result->code[b] == IN::T)
			{
				result->text[i] = b;
				result->size++;
				column++;
				if (b == IN_CODE_ENDL)
				{
					result->lines++;
					column = 0;
				}
				i++;
			}
			else if (result->code[b] == IN::F)
			{
				throw ERROR_THROW_IN(111, result->lines, column);
			}
			else if (result->code[b] == IN::I)
			{
				column++;
			}
			else if (result->code[b] >= 0 and result->code[b] <= 255)
			{
				result->text[i] = result->code[b];
				result->size++;
				column++;
				i++;
			}
		}
		result->text[i - 1] = '\0';

		return *result;
	}

}