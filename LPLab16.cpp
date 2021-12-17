#pragma once
#include "stdafx.h"

#include "LT.h"
#include "Log.h"
#include "LexicalAnalyzer.h"
#include "MFST.h"
#include "MFST_trace.h"
#include "PolishNotation.h"
#include "Semantics.h"


#include "CodeGenerator.h"

#include <chrono>

const int stdlibFunctionCount = 6;
IT::Entry stdlib[] = {
	{ 0, "toInteger", "", 1, { IT::IdDatatype::string }, IT::IdDatatype::integer, IT::IdType::function, 0, true },
	{ 0, "toString", "", 1, { IT::IdDatatype::integer }, IT::IdDatatype::string, IT::IdType::function, 0, true },
	{ 0, "inputInteger", "", 0, { }, IT::IdDatatype::integer, IT::IdType::function, 0, true },
	{ 0, "inputString", "", 0, { }, IT::IdDatatype::string, IT::IdType::function, 0, true },
	{ 0, "shiftRight", "", 2, { IT::IdDatatype::integer, IT::IdDatatype::integer }, IT::IdDatatype::integer, IT::IdType::function, 0, true },
	{ 0, "shiftLeft", "", 2, { IT::IdDatatype::integer, IT::IdDatatype::integer }, IT::IdDatatype::integer, IT::IdType::function, 0, true },
};

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	Log::LOG log = Log::INITLOG;
	output = true;

	auto start = std::chrono::high_resolution_clock::now();

	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		std::wcout << parm.in << std::endl
			<< parm.out << std::endl
			<< parm.log << std::endl;
		
		output = parm.output;

		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);

		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);

		LT::LexTable lexemeTable = LT::Create(4096);
		IT::IdTable identifierTable = IT::Create(4096);

		for (int i = 0; i < stdlibFunctionCount; i++)
		{
			IT::Add(identifierTable, stdlib[i]);
		}

		std::cout << "Лексический анализ..." << std::endl;
		LexicalAnalyzer::TNP2021((char*)in.text, lexemeTable, identifierTable);
		std::cout << "Лексический анализ успешно завершен." << std::endl;

		std::cout << "Синтаксический анализ..." << std::endl;
		MFST_TRACE_START
		MFST::Mfst mfst(lexemeTable, GRB::getGreibach());
		mfst.start();
		mfst.savededucation();
		mfst.printrules();
		std::cout << "Синаксический анализ успешно завершен." << std::endl;

		Semantics::RunAllChecks(lexemeTable, identifierTable);

		int index = 0;
		int nested = 0;
		while (index < lexemeTable.size)
		{
			if (lexemeTable.table[index].lexemeType == LEX_LEFT_CURLY_BRACKET)
			{
				nested++;
			}
			else if (lexemeTable.table[index].lexemeType == LEX_RIGHT_CURLY_BRACKET)
			{
				nested--;
			}
			else if (nested > 0)
			{
				if (lexemeTable.table[index].lexemeType == LEX_IF || lexemeTable.table[index].lexemeType == LEX_ELSEIF || lexemeTable.table[index].lexemeType == LEX_RETURN)
				{
					index = index + 2;
					int start = index;
					auto t = PolishNotation::PolishNotation(index, lexemeTable, identifierTable, true);
					while (!t->empty())
					{
						lexemeTable.table[start++] = t->back();
						t->pop_back();
					}

					int roundNested = 1;
					while (true)
					{
						if (lexemeTable.table[start].lexemeType == LEX_LEFT_BRACKET)
						{
							roundNested++;
						}
						else if (lexemeTable.table[start].lexemeType == LEX_RIGHT_BRACKET)
						{
							roundNested--;
							if (roundNested == 0) break;
						}
						LT::Entry* entry = new LT::Entry();
						entry->lexemeType = '\0';
						entry->stringNumber = lexemeTable.table[start].stringNumber;
						entry->stringPosition = lexemeTable.table[start].stringPosition;
						entry->identifierTableIndex = -1;
						entry->originalLexeme = nullptr;
						lexemeTable.table[start] = *entry;
						start++;
					}
				}
				else if (lexemeTable.table[index].lexemeType == LEX_IDENTIFIER || lexemeTable.table[index].lexemeType == LEX_PRINT)
				{
					int start = index;
					auto t = PolishNotation::PolishNotation(index, lexemeTable, identifierTable);
					while (!t->empty())
					{
						lexemeTable.table[start++] = t->back();
						t->pop_back();
					}
					while (lexemeTable.table[start].lexemeType != LEX_SEMICOLON)
					{
						LT::Entry* entry = new LT::Entry();
						entry->lexemeType = '\0';
						entry->stringNumber = lexemeTable.table[start].stringNumber;
						entry->stringPosition = lexemeTable.table[start].stringPosition;
						entry->identifierTableIndex = -1;
						entry->originalLexeme = nullptr;
						lexemeTable.table[start] = *entry;
						start++;
					}
				}
			}
			index++;
		}

		LT::RemoveEmptyEntries(lexemeTable);

		std::cout << "Генерация кода..." << std::endl;
		std::string program = CodeGenerator::GenerateCode(lexemeTable, identifierTable);
		std::cout << "Генерация кода успешно завершена." << std::endl;

		std::string lextable = LT::ToString(lexemeTable);
		std::cout << "Таблица лексем:" << lextable;
		std::cout << std::endl << std::endl;
		Log::WriteLine(log, "---- Таблица лексем:\n", lextable.c_str(), "\n", "");

		std::string idtable = IT::ToString(identifierTable);
		std::cout << "Таблица идентификаторов: " << std::endl;
		Log::WriteLine(log, "---- Таблица идентификаторов:\n", idtable.c_str(), "\n", "");

		Log::Close(log);

		std::ofstream out(parm.out);
		out << program;
		out.close();

		system("ml.exe /MANIFEST /LTCG:incremental /NXCOMPAT /DYNAMICBASE \"kernel32.lib\" \"user32.lib\" \"gdi32.lib\" \"winspool.lib\" \"comdlg32.lib\" \"advapi32.lib\" \"shell32.lib\" \"ole32.lib\" \"oleaut32.lib\" \"uuid.lib\" \"odbc32.lib\" \"odbccp32.lib\" /DEBUG /MACHINE:X86 /OPT:REF /SAFESEH:NO /INCREMENTAL:NO /SUBSYSTEM:CONSOLE /OPT:ICF /ERRORREPORT:PROMPT /NOLOGO /TLBID:1 test.tnp2021.out -link /subsystem:console");
	}
	catch (Error::ERROR e)
	{
		if (e.id == 100 || e.id == 110)
		{
			std::cout << "Ошибка " << e.id << ": " << e.message << '\n';
		}
		else
		{
			std::cout << "Ошибка " << e.id << ": " << e.message << '\n';
			Log::WriteError(log, e);
			Log::Close(log);
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	std::cout << "Время работы компилятора: " << (float)duration.count() / 1000 << " с" << std::endl;

	return 0;

}