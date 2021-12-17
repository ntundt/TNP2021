#pragma once

#include <string>

#define ID_NAME_MAXSIZE 32
#define TI_MAXSIZE 4096
#define TI_INT_DEFAULT 0x00000000
#define TI_STR_DEFAULT 0x00
#define TI_NULLIDX 0xffffffff
#define TI_STR_MAXSIZE 256
#define TI_FUNCTION_MAX_PARAMETERS 16

namespace IT
{

	enum class IdDatatype { undefined, integer, string, procedure };

	enum class IdType { undefined, function, variable, literal_, functionParameter, reserved };

	struct Entry
	{
		int lexTableFirstEntry = 0;
		char name[ID_NAME_MAXSIZE];
		std::string path = "";
		short parametersCount = 0;
		IdDatatype parameters[TI_FUNCTION_MAX_PARAMETERS] = {};
		IdDatatype iddatatype = IdDatatype::undefined;
		IdType idtype = IdType::undefined;
		uint32_t value = 0;
		bool external = false;
	};

	struct IdTable
	{
		int maxsize;
		int size;
		Entry* table;
	};

	IdTable Create(int size);

	int Add(IdTable& idtable, Entry entry);

	Entry GetEntry(IdTable& idtable, int n);

	int IsIdName(IdTable& idtable, const char id[ID_NAME_MAXSIZE], std::string path);

	int StringLiteralPresent(IdTable& idtable, const char* literal);

	const char* GetQualifiedIdentifierName(Entry& entry);

	std::string ToString(IdTable& idtable);

	void Delete(IdTable& idtable);

}