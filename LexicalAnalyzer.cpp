#pragma once

#include "stdafx.h"

#include "IT.h"
#include "LT.h"
#include "FST.h"
#include "Error.h"
#include "LexicalAnalyzer.h"

#include "Specification.h"

namespace LexicalAnalyzer 
{

	bool isDatatypeDeclaration(const char* lexeme, IT::IdDatatype& datatype)
	{
		if (strcmp(lexeme, "integer") == 0)
		{
			datatype = IT::IdDatatype::integer;
			return true;
		}
		else if (strcmp(lexeme, "string") == 0)
		{
			datatype = IT::IdDatatype::string;
			return true;
		}
		else if (strcmp(lexeme, "proc") == 0)
		{
			datatype = IT::IdDatatype::procedure;
			return true;
		}
		return false;
	}

	bool isLiteral(const char* lexeme, IT::IdDatatype& datatype, void*& value)
	{
		if (std::regex_match(lexeme, *Specification::string_literal_))
		{
			datatype = IT::IdDatatype::string;
			return true;
		}
		else if (std::regex_match(lexeme, *Specification::decimal_integer_literal_))
		{
			datatype = IT::IdDatatype::integer;
			value = (void*)(new int(std::stoi(lexeme, 0, 10)));
			return true;
		}
		else if (std::regex_match(lexeme, *Specification::octal_integer_literal_))
		{
			datatype = IT::IdDatatype::integer;
			value = (void*)(new int(std::stoi(lexeme, 0, 8)));
			return true;
		}
		return false;
	}

	bool isOperator(const char* lexeme)
	{
		return std::regex_match(lexeme, *Specification::operator_);
	}

	bool isIdentifier(const char* lexeme)
	{
		return std::regex_match(lexeme, *Specification::identifier_);
	}

	LT::lextype_t getLexemeType(const char* lexeme, IT::IdDatatype& datatype, void*& value)
	{
		if (isDatatypeDeclaration(lexeme, datatype))
		{
			return LEX_DATATYPE;
		}
		else if (isLiteral(lexeme, datatype, value))
		{
			return LEX_LITERAL;
		}
		else if (strcmp(lexeme, "function") == 0)
		{
			return LEX_FUNCTION;
		}
		else if (isOperator(lexeme))
		{
			return LEX_OPERATOR;
		}
		else if (strcmp(lexeme, ";") == 0)
		{
			return LEX_SEMICOLON;
		}
		else if (strcmp(lexeme, ",") == 0)
		{
			return LEX_COMMA;
		}
		else if (strcmp(lexeme, "(") == 0)
		{
			return LEX_LEFT_BRACKET;
		}
		else if (strcmp(lexeme, ")") == 0)
		{
			return LEX_RIGHT_BRACKET;
		}
		else if (strcmp(lexeme, "{") == 0)
		{
			return LEX_LEFT_CURLY_BRACKET;
		}
		else if (strcmp(lexeme, "}") == 0)
		{
			return LEX_RIGHT_CURLY_BRACKET;
		}
		else if (strcmp(lexeme, "[") == 0)
		{
			return LEX_LEFT_SQUARE_BRACKET;
		}
		else if (strcmp(lexeme, "]") == 0)
		{
			return LEX_RIGHT_SQUARE_BRACKET;
		}
		else if (strcmp(lexeme, "if") == 0)
		{
			return LEX_IF;
		}
		else if (strcmp(lexeme, "else") == 0)
		{
			return LEX_ELSE;
		}
		else if (strcmp(lexeme, "elseif") == 0)
		{
			return LEX_ELSEIF;
		}
		else if (strcmp(lexeme, "create") == 0)
		{
			return LEX_CREATE;
		}
		else if (strcmp(lexeme, "destroy") == 0)
		{
			return LEX_DESTROY;
		}
		else if (strcmp(lexeme, "output") == 0)
		{
			return LEX_PRINT;
		}
		else if (strcmp(lexeme, "if") == 0)
		{
			return LEX_IF;
		}
		else if (strcmp(lexeme, "elseif") == 0)
		{
			return LEX_ELSEIF;
		}
		else if (strcmp(lexeme, "else") == 0)
		{
			return LEX_ELSE;
		}
		else if (strcmp(lexeme, "return") == 0)
		{
			return LEX_RETURN;
		}
		else if (strcmp(lexeme, "=") == 0)
		{
			return LEX_EQUALS;
		}
		else if (isIdentifier(lexeme))
		{
			return LEX_IDENTIFIER;
		}
		return LEX_EMPTY;
	}

	void getRecognizerPos(std::string prefix, int& row, int& column)
	{
		for (std::string::iterator it = prefix.begin(); it < prefix.end(); it++)
		{
			if (*it == '\n')
			{
				row++;
				column = 0;
			}
			else
			{
				column++;
			}
		}
	}

	int fillFunctionParameters(LT::LexTable& lexTable, int startIndex, std::string path, IT::IdTable& idTable, int functionIdTableIndex)
	{
		IT::IdDatatype datatype = IT::IdDatatype::undefined;
		IT::Entry* idTableEntry = new IT::Entry;
		idTableEntry->path = path;

		int i = startIndex;
		while (LT::GetEntry(lexTable, i).lexemeType != LEX_RIGHT_BRACKET)
		{
			LT::Entry* entry = &lexTable.table[i];
			if (entry->lexemeType == LEX_COMMA)
			{
				datatype = IT::IdDatatype::undefined;
			} 
			else if (entry->lexemeType == LEX_DATATYPE)
			{
				isDatatypeDeclaration(entry->originalLexeme, datatype);
			}
			else if (entry->lexemeType == LEX_IDENTIFIER)
			{
				idTableEntry->iddatatype = datatype;
				idTableEntry->idtype = IT::IdType::functionParameter;
				strcpy_s(idTableEntry->name, 32, entry->originalLexeme);
				idTableEntry->lexTableFirstEntry = i;
				entry->identifierTableIndex = IT::Add(idTable, *idTableEntry);
				
				if (idTable.table[functionIdTableIndex].parametersCount + 1 > 16)
				{
					LT::Entry entry = LT::GetEntry(lexTable, i);
					
					ERROR_THROW_IN(202, entry.stringNumber, entry.stringPosition);
				}

				idTable.table[functionIdTableIndex].parameters[idTable.table[functionIdTableIndex].parametersCount++] = datatype;
			}
			i++;
		}
		return i;
	}

	int fillIdTable(LT::LexTable& lexTable, int startIndex, IT::IdTable& idTable)
	{
		static std::string codeBlocks = "_";
		static uint32_t ifCounter = 0;
		static uint32_t whileCounter = 0;
		
		bool metIfKeyword = false;

		bool metWhileKeyword = false;
		bool metFunctionKeyword = false;
		char functionName[ID_NAME_MAXSIZE] = "";
		bool metBracket = false;

		bool metCreate = false;

		IT::IdDatatype datatype = IT::IdDatatype::undefined;

		int i = startIndex;
		while (LT::GetEntry(lexTable, i).lexemeType != LEX_RIGHT_CURLY_BRACKET && i < lexTable.size)
		{
			LT::Entry* entry = &lexTable.table[i];
			if (entry->lexemeType == LEX_IDENTIFIER)
			{
				if (strlen(entry->originalLexeme) > 31)
				{
					throw ERROR_THROW_IN(203, entry->stringNumber, entry->stringPosition); // Идентификатор не может быть длинее 31 сиимвола
				}
			}

			if (entry->lexemeType == LEX_LEFT_CURLY_BRACKET)
			{
				int previousLength = codeBlocks.length();
				if (metFunctionKeyword)
				{
					codeBlocks.append(std::string("@_") + std::string(functionName));
				}
				else if (metIfKeyword)
				{
					codeBlocks.append(std::string("@if") + std::to_string(ifCounter++));
				}
				else if (metWhileKeyword)
				{
					codeBlocks.append(std::string("@while") + std::to_string(whileCounter++));
				}
				i = fillIdTable(lexTable, i + 1, idTable);
				codeBlocks.resize(previousLength);
			} 
			else if (entry->lexemeType == LEX_DATATYPE)
			{
				isDatatypeDeclaration(entry->originalLexeme, datatype);
			}
			else if (entry->lexemeType == LEX_FUNCTION)
			{
				metFunctionKeyword = true;
			}
			else if (strcmp(entry->originalLexeme, "if") == 0)
			{
				metIfKeyword = true;
			}
			else if (strcmp(entry->originalLexeme, "while") == 0) 
			{
				metWhileKeyword = true;
			}
			else if (entry->lexemeType == LEX_CREATE)
			{
				metCreate = true;
			}
			else if (entry->lexemeType == LEX_SEMICOLON)
			{
				metWhileKeyword = false;
				metIfKeyword = false;
				metFunctionKeyword = false;
				metCreate = false;
				metBracket = false;
			}
			else if (entry->lexemeType == LEX_LEFT_BRACKET && metFunctionKeyword && metCreate)
			{
				IT::Entry* idTableEntry = new IT::Entry;
				idTableEntry->iddatatype = datatype;
				idTableEntry->idtype = IT::IdType::function;
				idTableEntry->lexTableFirstEntry = i;
				idTableEntry->path = codeBlocks.c_str();
				strcpy_s(idTableEntry->name, 32, functionName);
				(entry - 1)->identifierTableIndex = IT::Add(idTable, *idTableEntry);

				int previousLength = codeBlocks.length();
				codeBlocks.append(std::string("@_") + std::string(functionName));
				i = fillFunctionParameters(lexTable, i + 1, codeBlocks, idTable, (entry - 1)->identifierTableIndex);
				codeBlocks.resize(previousLength);
			}
			else if (entry->lexemeType == LEX_LEFT_BRACKET && metFunctionKeyword && !metCreate)
			{
				throw ERROR_THROW_IN(204, entry->stringNumber, entry->stringPosition); // ERR: Идентификатор не объявлен
			}
			else if (entry->lexemeType == LEX_IDENTIFIER && !metFunctionKeyword && metCreate)
			{
				int id = IT::IsIdName(idTable, entry->originalLexeme, codeBlocks);
				if (id == TI_NULLIDX)
				{
					IT::Entry* idTableEntry = new IT::Entry;
					idTableEntry->iddatatype = datatype;
					idTableEntry->idtype = IT::IdType::variable;
					idTableEntry->path = codeBlocks.c_str();
					idTableEntry->lexTableFirstEntry = i;
					strcpy_s(idTableEntry->name, 32, entry->originalLexeme);
					entry->identifierTableIndex = IT::Add(idTable, *idTableEntry);
				}
				else
				{
					entry->identifierTableIndex = id;
				}
			}
			else if (entry->lexemeType == LEX_IDENTIFIER && !metFunctionKeyword && !metCreate && IT::IsIdName(idTable, entry->originalLexeme, codeBlocks) == LT_TI_NULLIDX)
			{
				throw ERROR_THROW_IN(204, entry->stringNumber, entry->stringPosition); // ERR: Идентификатор не объявлен
			}
			else if (entry->lexemeType == LEX_IDENTIFIER && !metFunctionKeyword && !metCreate)
			{
				entry->identifierTableIndex = IT::IsIdName(idTable, entry->originalLexeme, codeBlocks);	
			}
			else if (entry->lexemeType == LEX_IDENTIFIER && metFunctionKeyword && metCreate)
			{
				strcpy_s(functionName, 32, entry->originalLexeme);
			}

			i++;
		}
		return i;
	}

	void TNP2021(const char* program, LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		std::string prg(program);
		std::smatch match;

		LT::lextype_t type;
		IT::IdDatatype datatype;
		void* value;

		static uint32_t literalCounter;

		int row = 1;
		int column = 1;

		int lexTableIndex = 0;

		while (std::regex_search(prg, match, Specification::lexeme))
		{
			lexTableIndex++;

			datatype = IT::IdDatatype::undefined;
			value = nullptr;

			type = getLexemeType(match[0].str().c_str(), datatype, value);
			getRecognizerPos(match.prefix().str(), row, column);
			LT::Entry* lexTableEntry = new LT::Entry(type, row, column);

			char* text = new char[match[0].str().length() + 1];
			strcpy_s(text, match[0].str().length() + 1, match[0].str().c_str());
			lexTableEntry->originalLexeme = text;

			if (type == LEX_LITERAL)
			{
				IT::Entry* idTableEntry = new IT::Entry;
				idTableEntry->iddatatype = datatype;
				idTableEntry->idtype = IT::IdType::literal_;
				if (datatype == IT::IdDatatype::string)
				{
					int index = IT::StringLiteralPresent(idTable, match[1].str().c_str());
					if (index == -1)
					{
						strcpy_s(idTableEntry->name, 32, (std::string("literal") + std::to_string(literalCounter++)).c_str());
						idTableEntry->lexTableFirstEntry = lexTableIndex;
						std::string val = match[1].str();
						idTableEntry->value = (uint32_t)new char[val.length() + 1];
						strcpy_s((char*)idTableEntry->value, val.length() + 1, val.c_str());
						lexTableEntry->identifierTableIndex = IT::Add(idTable, *idTableEntry);
					}
					else
					{
						lexTableEntry->identifierTableIndex = index;
					}
				}
				else if (datatype == IT::IdDatatype::integer)
				{
					strcpy_s(idTableEntry->name, 32, (std::string("literal") + std::to_string(literalCounter++)).c_str());
					idTableEntry->lexTableFirstEntry = lexTableIndex;
					idTableEntry->value = *(uint32_t*)value;
					lexTableEntry->identifierTableIndex = IT::Add(idTable, *idTableEntry);
				}
				delete idTableEntry;
			}
			else if (type == LEX_DATATYPE)
			{
				lexTableEntry->lexemeType = LEX_DATATYPE;
			}
			else if (type == LEX_EMPTY)
			{
				throw ERROR_THROW_IN(205, row, column);
			}

			LT::Add(lexTable, *lexTableEntry);
			delete lexTableEntry;
			prg = match.suffix();
		}

		fillIdTable(lexTable, 0, idTable);
	}

}