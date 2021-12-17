#pragma once

#include "stdafx.h"

#include "LT.h"
#include "IT.h"
#include "PolishNotation.h"
#include "Error.h"

namespace Semantics
{
	IT::IdDatatype GetExpressionDatatype(LT::LexTable& lexemeTable, IT::IdTable& idTable, int position)
	{
		std::list<LT::Entry>* expressionP = PolishNotation::PolishNotation(position, lexemeTable, idTable, true);
		std::stack<IT::IdDatatype> calculationStack;
		while (!expressionP->empty())
		{
			if (expressionP->back().originalLexeme == nullptr)
			{
				expressionP->pop_back();
				continue;
			}
			if (expressionP->back().lexemeType == LEX_IDENTIFIER || expressionP->back().lexemeType == LEX_LITERAL)
			{
				IT::Entry entry = IT::GetEntry(idTable, expressionP->back().identifierTableIndex);
				if (entry.idtype == IT::IdType::function)
				{
					for (int i = entry.parametersCount - 1; i >= 0; i--)
					{
						if (calculationStack.top() == entry.parameters[i])
						{
							calculationStack.pop();
						}
						else
						{
							throw ERROR_THROW_IN(201, expressionP->back().stringNumber, expressionP->back().stringPosition); // Тип фактического параметра не совпадает с типом формального 
						}
					}
				}
				calculationStack.push(entry.iddatatype);
			}
			else if (strcmp(expressionP->back().originalLexeme, "+") == 0)
			{
				IT::IdDatatype datatype1 = calculationStack.top();
				calculationStack.pop();
				IT::IdDatatype datatype2 = calculationStack.top();
				calculationStack.pop();
				if (datatype1 == IT::IdDatatype::integer && datatype2 == IT::IdDatatype::integer)
				{
					calculationStack.push(IT::IdDatatype::integer);
				}
				else if (datatype1 == IT::IdDatatype::string && datatype2 == IT::IdDatatype::string)
				{
					calculationStack.push(IT::IdDatatype::string);
				}
				else
				{
					throw ERROR_THROW_IN(200, expressionP->back().stringNumber, expressionP->back().stringPosition); // Эту операцию нельзя применить к операндам этих типов
				}
			}
			else if (strcmp(expressionP->back().originalLexeme, "-") == 0
				|| strcmp(expressionP->back().originalLexeme, "/") == 0
				|| strcmp(expressionP->back().originalLexeme, "*") == 0
				|| strcmp(expressionP->back().originalLexeme, "%") == 0
				|| strcmp(expressionP->back().originalLexeme, "<=") == 0
				|| strcmp(expressionP->back().originalLexeme, ">=") == 0
				|| strcmp(expressionP->back().originalLexeme, "<") == 0
				|| strcmp(expressionP->back().originalLexeme, ">") == 0)
			{
				IT::IdDatatype datatype1 = calculationStack.top();
				calculationStack.pop();
				IT::IdDatatype datatype2 = calculationStack.top();
				calculationStack.pop();
				if (datatype1 == IT::IdDatatype::integer && datatype2 == IT::IdDatatype::integer)
				{
					calculationStack.push(IT::IdDatatype::integer);
				}
				else
				{
					throw ERROR_THROW_IN(200, expressionP->back().stringNumber, expressionP->back().stringPosition); // Эту операцию нельзя применить к операндам этих типов
				}
			}
			else if (strcmp(expressionP->back().originalLexeme, "==") == 0
				|| strcmp(expressionP->back().originalLexeme, "!=") == 0)
			{
				IT::IdDatatype datatype1 = calculationStack.top();
				calculationStack.pop();
				IT::IdDatatype datatype2 = calculationStack.top();
				calculationStack.pop();
				if (datatype1 == IT::IdDatatype::integer && datatype2 == IT::IdDatatype::integer)
				{
					calculationStack.push(IT::IdDatatype::integer);
				}
				else if (datatype1 == IT::IdDatatype::string && datatype2 == IT::IdDatatype::string)
				{
					calculationStack.push(IT::IdDatatype::integer);
				}
				else
				{
					throw ERROR_THROW_IN(200, expressionP->back().stringNumber, expressionP->back().stringPosition); // Эту операцию нельзя применить к операндам этих типов
				}
			}
			expressionP->pop_back();
		}
		if (calculationStack.size() != 1)
		{
			return IT::IdDatatype::undefined;
		}
		return calculationStack.top();
	}

	bool VariableIsDefinedBeforeUsage(LT::LexTable&)
	{
		return true;
	}

	bool ReturnDatatypeMatch(LT::LexTable& lexemeTable, IT::IdTable& identifierTable)
	{
		IT::Entry function;
		int nested = 0;
		for (int i = 0; i < lexemeTable.size; i++)
		{
			if (lexemeTable.table[i].lexemeType == LEX_IDENTIFIER)
			{
				if (identifierTable.table[lexemeTable.table[i].identifierTableIndex].idtype == IT::IdType::function && nested == 0)
				{
					function = identifierTable.table[lexemeTable.table[i].identifierTableIndex];
					i += function.parametersCount * 3 + 1;
				}
			}
			else if (nested > 0 && lexemeTable.table[i].lexemeType == LEX_RETURN)
			{
				i += 2;
				if (function.iddatatype == IT::IdDatatype::procedure && lexemeTable.table[i].lexemeType != LEX_RIGHT_BRACKET)
				{
					throw ERROR_THROW_IN(205, lexemeTable.table[i].stringNumber, lexemeTable.table[i].stringPosition); // Процедура не возвращает значения.
				}
				if (function.iddatatype != GetExpressionDatatype(lexemeTable, identifierTable, i))
				{
					throw ERROR_THROW_IN(206, lexemeTable.table[i].stringNumber, lexemeTable.table[i].stringPosition); // Тип операнда оператора return не совпадает с указанным в сигнатуре функции.
				}
			}
			else if (lexemeTable.table[i].lexemeType == LEX_LEFT_CURLY_BRACKET)
			{
				nested++;
			}
			else if (lexemeTable.table[i].lexemeType == LEX_RIGHT_CURLY_BRACKET)
			{
				nested--;
			}
		}
		return true;
	}

	bool NoProcedureVariables(LT::LexTable& lexemeTable, IT::IdTable& identifierTable)
	{
		for (int i = 0; i < lexemeTable.size; i++)
		{
			if (lexemeTable.table[i].lexemeType == LEX_DATATYPE)
			{
				if (lexemeTable.table[i + 1].lexemeType != LEX_FUNCTION && strcmp(lexemeTable.table[i].originalLexeme, "proc") == 0)
				{
					throw ERROR_THROW_IN(207, lexemeTable.table[i].stringNumber, lexemeTable.table[i].stringPosition); // Переменная не может быть объявлена как proc
				}
			}
		}
		return true;
	}

	bool IntegerPassedToIf(LT::LexTable& lexemeTable, IT::IdTable& identifierTable)
	{
		for (int i = 0; i < lexemeTable.size; i++)
		{
			if (lexemeTable.table[i].lexemeType == LEX_IF || lexemeTable.table[i].lexemeType == LEX_ELSEIF)
			{
				if (GetExpressionDatatype(lexemeTable, identifierTable, i + 2) != IT::IdDatatype::integer)
				{
					throw ERROR_THROW_IN(208, lexemeTable.table[i + 2].stringNumber, lexemeTable.table[i + 2].stringPosition); // if принимает только целочисленный параметр.
				}
			}
		}
		return true;
	}

	bool StringPassedToOutput(LT::LexTable& lexemeTable, IT::IdTable& identifierTable)
	{
		for (int i = 0; i < lexemeTable.size; i++)
		{
			if (lexemeTable.table[i].lexemeType == LEX_PRINT)
			{
				if (GetExpressionDatatype(lexemeTable, identifierTable, i + 2) != IT::IdDatatype::string)
				{
					throw ERROR_THROW_IN(209, lexemeTable.table[i + 2].stringNumber, lexemeTable.table[i + 2].stringPosition);
					// output принимает только строковый параметр.
				}
			}
		}
	}

	bool FormalParametersMatch(LT::LexTable& lexemeTable, IT::IdTable& identifierTable)
	{
		int nested = 0;
		for (int i = 0; i < lexemeTable.size; i++)
		{
			if (lexemeTable.table[i].lexemeType == LEX_LEFT_CURLY_BRACKET)
			{
				nested++;
			}
			else if (lexemeTable.table[i].lexemeType == LEX_RIGHT_CURLY_BRACKET)
			{
				nested--;
			}
			else if (nested > 0)
			{
				if (lexemeTable.table[i].lexemeType == LEX_IDENTIFIER);
				{
					GetExpressionDatatype(lexemeTable, identifierTable, i);
					while (lexemeTable.table[i].lexemeType != LEX_SEMICOLON) i++;
				}
			}
		}
		return true;
	}

	bool BlockHasReturn(LT::LexTable& lexemeTable, IT::IdTable& identifierTable, int& index)
	{
		int nested = 0;
		bool everyNestedHas = true;
		for (int i = index; lexemeTable.table[i].lexemeType != LEX_RIGHT_CURLY_BRACKET && nested == 1; i++)
		{
			if (lexemeTable.table[i].lexemeType != LEX_LEFT_CURLY_BRACKET)
			{
				if (!BlockHasReturn(lexemeTable, identifierTable, i))
				{
					everyNestedHas = false;
					i++;
				}
			}
			if (lexemeTable.table[i].lexemeType == LEX_RETURN)
			{
				return true;
			}
		}
		return everyNestedHas;
	}

	bool FunctionHasReturn(LT::LexTable& lexemeTable, IT::IdTable& identifierTable)
	{
		int nested = 0;
		LT::Entry function;
		for (int i = 0; i < lexemeTable.size; i++)
		{
			if (lexemeTable.table[i].lexemeType == LEX_IDENTIFIER && identifierTable.table[lexemeTable.table[i].identifierTableIndex].idtype == IT::IdType::function)
			{
				function = lexemeTable.table[i];
			}
			if (lexemeTable.table[i].lexemeType == LEX_LEFT_CURLY_BRACKET)
			{
				if (!BlockHasReturn(lexemeTable, identifierTable, i))
				{
					throw ERROR_THROW_IN(211, function.stringNumber, function.stringPosition);
				}
			}
		}
		return true;
	}

	void RunAllChecks(LT::LexTable& lexemeTable, IT::IdTable& identifierTable)
	{
		ReturnDatatypeMatch(lexemeTable, identifierTable);
		NoProcedureVariables(lexemeTable, identifierTable);
		IntegerPassedToIf(lexemeTable, identifierTable);
		StringPassedToOutput(lexemeTable, identifierTable);
		FormalParametersMatch(lexemeTable, identifierTable);
		FunctionHasReturn(lexemeTable, identifierTable);
	}
}