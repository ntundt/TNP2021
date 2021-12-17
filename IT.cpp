#pragma once

#include "stdafx.h"

#include "IT.h"

namespace IT {

	IdTable Create(int size)
	{
		IdTable* table = new IdTable;
		table->maxsize = size;
		table->size = 0;
		table->table = new Entry[size];

		return *table;
	}

	int Add(IdTable& idtable, Entry entry)
	{
		idtable.table[idtable.size] = entry;
		idtable.size++;
		return idtable.size - 1;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int IsIdName(IdTable& idtable, const char id[ID_NAME_MAXSIZE], std::string path)
	{
		int returnIndex = TI_NULLIDX;
		int maxOctotorpCount = -1;
		path = path + "@";
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].name, id) == 0)
			{
				if (idtable.table[i].external)
				{
					maxOctotorpCount = 0; 
					returnIndex = i;
				}
				if (path.rfind(idtable.table[i].path + "@", 0) == 0)
				{
					int octotorpCount = std::count(idtable.table[i].path.begin(), idtable.table[i].path.end(), '@');
					if (octotorpCount > maxOctotorpCount)
					{
						maxOctotorpCount = octotorpCount;
						returnIndex = i;
					}
				}
			}
		}
		return returnIndex;
	}

	int IsFunctionIdName(IdTable& idtable, const char id[ID_NAME_MAXSIZE], const IdDatatype parameters[TI_FUNCTION_MAX_PARAMETERS], short parametersCount, std::string path)
	{
		int returnIndex = TI_NULLIDX;
		int maxOctotorpCount = -1;
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].name, id) == 0 && idtable.table[i].idtype == IdType::function)
			{
				if (idtable.table[i].parametersCount != parametersCount)
				{
					continue;
				}
				for (int j = 0; j < parametersCount; j++)
				{
					if (idtable.table[i].parameters[j] != parameters[j])
					{
						continue;
					}
				}
				if (path.rfind(idtable.table[i].path, 0) == 0)
				{
					int octotorpCount = std::count(idtable.table[i].path.begin(), idtable.table[i].path.end(), '@');
					if (octotorpCount > maxOctotorpCount)
					{
						maxOctotorpCount = octotorpCount;
						returnIndex = i;
					}
				}
			}
		}
		return returnIndex;
	}

	int StringLiteralPresent(IdTable& idTable, const char* literal)
	{
		for (int i = 0; i < idTable.size; i++)
		{
			if (idTable.table[i].iddatatype == IT::IdDatatype::string && idTable.table[i].idtype == IT::IdType::literal_)
			{
				if (strcmp((const char*)idTable.table[i].value, literal) == 0)
				{
					return i;
				}
			}
		}
		return TI_NULLIDX;
	}

	std::string ToString(IdTable& idtable)
	{
		std::string str;
		std::stringstream result(str);
		result << std::setw(10) << std::left << "Тип"
			<< std::setw(18) << std::left << "Вид"
			<< std::setw(4) << std::left << "ЛТ№"
			<< std::setw(32) << std::left << "Имя идентификатора"
			<< std::setw(40) << std::left << "Путь"
			<< std::setw(10) << std::left << "Значение"
			<< '\n';

		for (int i = 0; i < idtable.size; i++)
		{
			result << std::setw(10) << std::left;
			if (idtable.table[i].iddatatype == IdDatatype::undefined)
			{
				result << "undefined";
			}
			else if (idtable.table[i].iddatatype == IdDatatype::string)
			{
				result << "string";
			}
			else if (idtable.table[i].iddatatype == IdDatatype::integer)
			{
				result << "integer";
			}
			else if (idtable.table[i].iddatatype == IdDatatype::procedure)
			{
				result << "procedure";
			}

			result << std::setw(18) << std::left;
			if (idtable.table[i].idtype == IdType::function)
			{
				result << "function";
			}
			else if (idtable.table[i].idtype == IdType::functionParameter)
			{
				result << "functionParameter";
			}
			else if (idtable.table[i].idtype == IdType::literal_)
			{
				result << "literal";
			}
			else if (idtable.table[i].idtype == IdType::undefined)
			{
				result << "undefined";
			}
			else if (idtable.table[i].idtype == IdType::variable)
			{
				result << "variable";
			}

			result << std::setw(4) << std::left << idtable.table[i].lexTableFirstEntry;
			result << std::setw(32) << std::left << idtable.table[i].name;
			result << std::setw(40) << std::left << idtable.table[i].path;
			result << std::setw(10) << std::left << idtable.table[i].value;
			result << '\n';
		}
		return result.str();
	}

	const char* GetQualifiedIdentifierName(Entry& entry)
	{
		if (entry.idtype == IdType::reserved || entry.idtype == IdType::literal_)
		{
			return entry.name;
		}
		else if (entry.idtype == IdType::function && entry.external == true)
		{
			return entry.name;
		}
		else
		{
			int namelen = strlen(entry.name) + entry.path.length() + 4;
			char* name = new char[namelen];
			sprintf_s(name, namelen, "%s@_%s", entry.path.c_str(), entry.name);
			return name;
		}
	}

	void Delete(IdTable& idtable)
	{
		free(idtable.table);
		free(&idtable);
	}

}