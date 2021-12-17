#pragma once

#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include "Log.h"
#include "Parm.h"
#include "Error.h"
#include "In.h"

namespace Log {

	LOG getlog(wchar_t logfile[])
	{
		LOG* result = new LOG;
		wcscpy_s(result->logfile, logfile);
		std::ofstream* log = new std::ofstream;
		log->open(logfile);
		if (log->fail() or not log->is_open() or log->bad())
		{
			throw ERROR_THROW(112);
		}
		result->stream = log;
		return *result;
	}

	void WriteLine(LOG log, const char* c, ...)
	{
		const char** ptr = &c;
		while (strcmp(*ptr, "") != 0)
		{
			*(log.stream) << *ptr;
			ptr++;
		}
		*(log.stream) << '\n';
	}

	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		const wchar_t** ptr = &c;
		while (wcscmp(*ptr, L"") != 0)
		{
			log.stream->write((char*)*ptr, 10);
			ptr++;
		}
		*(log.stream) << '\n';
	}

	void Write(LOG log, const char* c, ...)
	{
		const char** ptr = &c;
		while (strcmp(*ptr, "") != 0)
		{
			*(log.stream) << *ptr;
			ptr++;
		}
	}

	void WriteSingle(LOG log, const char* c)
	{
		*(log.stream) << c;
	}

	void WriteLog(LOG log)
	{
		time_t n = time(0);
		const time_t* now = &n;
		tm* ltm = localtime(now);
		*(log.stream) << "---- Протокол работы компилятора TNP-2021 ---- \nДата: "
			<< (ltm->tm_mday < 10 ? "0" : "") << ltm->tm_mday << '.'
			<< (ltm->tm_mon + 1 < 10 ? "0" : "") << ltm->tm_mon + 1 << '.' << ltm->tm_year + 1900 << ' '
			<< (ltm->tm_hour < 10 ? "0" : "") << ltm->tm_hour << ':'
			<< (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min << ':'
			<< (ltm->tm_sec < 10 ? "0" : "") << ltm->tm_sec << " ----\n";
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		char buffer[PARM_MAX_SIZE];
		*(log.stream) << "---- Параметры ----\n";
		wcstombs(buffer, parm.in, PARM_MAX_SIZE);
		*(log.stream) << "--in       " << buffer << "\n";
		wcstombs(buffer, parm.out, PARM_MAX_SIZE);
		*(log.stream) << "--out      " << buffer << "\n";
		wcstombs(buffer, parm.log, PARM_MAX_SIZE);
		*(log.stream) << "--log      " << buffer << "\n";
		wcstombs(buffer, parm.output ? L"false" : L"true", 10);
		*(log.stream) << "--nooutput " << buffer << "\n";
	}

	void WriteIn(LOG log, In::IN in)
	{
		*(log.stream) << "---- Исходные данные ----\n"
			<< "Количество символов : " << in.size << "\n"
			<< "Проигнорировано     : " << in.ignor << "\n"
			<< "Количество строк    : " << in.lines << "\n";
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream->is_open())
		{
			*(log.stream) << "Ошибка " << error.id << ": "
				<< error.message << ", строка " << error.inext.line << ", столбец " << error.inext.col << "\n";
		}
		else
		{
			std::wcout << "Ошибка " << error.id << ": "
				<< error.message << ", строка " << error.inext.line << ", столбец " << error.inext.col << "\n";
		}
	}

	void Close(LOG log)
	{
		log.stream->close();
	}

}