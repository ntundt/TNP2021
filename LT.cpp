#pragma once

#include "stdafx.h"

#include "LT.h"

namespace LT {

	LexTable Create(int size)
	{
		LexTable* table = new LexTable;
		table->maxSize = size;
		table->size = 0;
		table->table = new Entry[size];

		return *table;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		lextable.size++;
		if (lextable.maxSize < lextable.size)
		{
			throw "Too much lexems in one file;";
		}
		lextable.table[lextable.size - 1] = entry;
	}

	void Replace(LexTable& lextable, Entry entry, int n)
	{
		lextable.table[n] = entry;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		free(lextable.table);
		free(&lextable);
	}

	std::string ToString(LexTable& lextable)
	{
		int lineIndex = -1;
		std::string str;
		std::stringstream buffer(str);
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].stringNumber != lineIndex)
			{
				lineIndex = lextable.table[i].stringNumber;
				buffer << '\n' << std::setw(4) << std::left << lineIndex;
			}
			buffer << lextable.table[i].lexemeType;
		}
		return buffer.str();
	}

	void RemoveEmptyEntries(LexTable& lextable)
	{
		int insertIndex = 0;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexemeType != '\0')
			{
				lextable.table[insertIndex++] = lextable.table[i];
			}
		}
		lextable.size = insertIndex;
	}

	char* GetLine(LexTable& lexTable, int index)
	{
		char* result = new char[256];
		result[0] = '\0';
		int line = lexTable.table[index].stringNumber;
		while (line == lexTable.table[index].stringNumber)
		{
			if (lexTable.table[index].originalLexeme != nullptr)
			{
				strncat_s(result, 256, lexTable.table[index].originalLexeme, 32);
				strncat_s(result, 256, " ", 2);
			}
			index++;
		}
		return result;
	}
	
	LT::Entry::Entry(char lexeme, int stringNumber, int stringPosition)
	{
		this->lexemeType = lexeme;
		this->stringNumber = stringNumber;
		this->stringPosition = stringPosition;
	}

	LT::Entry::Entry() { }


}