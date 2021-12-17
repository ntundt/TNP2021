#pragma once

#include "stdafx.h"

#include "IT.h"
#include "LT.h"
#include "FST.h"
#include "LexicalAnalyzer.h"
#include "Error.h"

namespace PolishNotation 
{
	struct ConstCharPtrComparer
	{
		bool operator()(char const* a, char const* b) const
		{
			return std::strcmp(a, b) < 0;
		}
	};

	std::map<const char*, int, ConstCharPtrComparer> priorities{
		{ "=", 0 },
		{ "output", 0 },
		{ "input", 0 },
		{ "return", 0 },
		{ "and", 1 },
		{ "or", 1 },
		{ "xor", 1 },
		{ "==", 2 },
		{ "<=", 2 },
		{ ">=", 2 },
		{ "!=", 2 },
		{ "<", 2 },
		{ ">", 2 },
		{ "<<", 3 },
		{ ">>", 3 },
		{ "+", 4 },
		{ "-", 4 },
		{ "*", 5 },
		{ "/", 5 },
		{ "%", 5 },
		{ "_function_call", 6 },
	};

	const char* prefixOperators[] = {
		"output", "input", "return"
	};

	bool isPrefixOperator(const char* originalLexeme)
	{
		for (int i = 0; i < 2; i++)
		{
			if (strcmp(originalLexeme, prefixOperators[i]) == 0)
			{
				return true;
			}
		}
		return false;
	}

	const char* binaryOperators[] = {
		"=", "+", "-", "*", "/", "%",
		"==", "!=", ">=", "<=", ">", "<",
		"<<", ">>",
		"and", "or", "xor"
	};

	bool isBinaryOperator(const char* originalLexeme)
	{
		for (int i = 0; i < 15; i++)
		{
			if (strcmp(originalLexeme, binaryOperators[i]) == 0)
			{
				return true;
			}
		}
		return false;
	}

	void setLexem(LT::LexTable& lextable, char lexem, int n)
	{
		lextable.table[n].lexemeType = lexem;
	}

	std::list<LT::Entry>* PolishNotation(int& lextableScanPos, LT::LexTable& lextable, IT::IdTable& idtable, bool terminateOnComma = false)
	{
		std::stack<LT::Entry> stack;
		std::list<LT::Entry>* list = new std::list<LT::Entry>;

		while (lextable.table[lextableScanPos].lexemeType != LEX_SEMICOLON)
		{
			if (terminateOnComma)
			{
				if (lextable.table[lextableScanPos].lexemeType == LEX_COMMA || lextable.table[lextableScanPos].lexemeType == LEX_RIGHT_BRACKET)
				{
					while (!stack.empty())
					{
						list->push_front(stack.top());
						stack.pop();
					}
					return list;
				}
			}

			if (lextable.table[lextableScanPos].lexemeType == LEX_IDENTIFIER)
			{
				if (IT::GetEntry(idtable, lextable.table[lextableScanPos].identifierTableIndex).idtype == IT::IdType::function)
				{
					stack.push(lextable.table[lextableScanPos]);
				}
				else
				{
					list->push_front(lextable.table[lextableScanPos]);
				}
			}
			else if (lextable.table[lextableScanPos].lexemeType == LEX_LITERAL)
			{
				list->push_front(lextable.table[lextableScanPos]);
			}
			else if (isPrefixOperator(lextable.table[lextableScanPos].originalLexeme))
			{
				stack.push(lextable.table[lextableScanPos]);
			}
			else if (lextable.table[lextableScanPos].lexemeType == LEX_LEFT_BRACKET)
			{
				std::stack<std::list<LT::Entry>*>* pars = new std::stack<std::list<LT::Entry>*>;

				bool functionCall = false;
				IT::Entry function;
				LT::Entry functionLexeme;
				if (lextable.table[lextableScanPos - 1].lexemeType == LEX_IDENTIFIER)
				{
					functionCall = idtable.table[lextable.table[lextableScanPos - 1].identifierTableIndex].idtype == IT::IdType::function && idtable.table[lextable.table[lextableScanPos - 1].identifierTableIndex].parametersCount > 0;
					function = idtable.table[lextable.table[lextableScanPos - 1].identifierTableIndex];
					functionLexeme = lextable.table[lextableScanPos - 1];
				}

				while (lextable.table[lextableScanPos].lexemeType != LEX_RIGHT_BRACKET)
				{
					pars->push(PolishNotation(++lextableScanPos, lextable, idtable, true));
				}

				if (functionCall && pars->size() != function.parametersCount)
				{
					throw ERROR_THROW_IN(201, functionLexeme.stringNumber, functionLexeme.stringPosition);
				}

				while (!pars->empty())
				{
					while (!pars->top()->empty())
					{
						list->push_front(pars->top()->back());
						pars->top()->pop_back();
					}
					if (functionCall)
					{
						if (list->size() > 0)
						{
							list->push_front(*new LT::Entry(LEX_PUSH_MARK, list->front().stringNumber, 0));
						}
					}
					pars->pop();
				}
				delete pars;
				
				if (lextable.table[lextableScanPos].lexemeType == LEX_SEMICOLON)
				stack.push(lextable.table[lextableScanPos]);
			}
			else if (lextable.table[lextableScanPos].lexemeType == LEX_COMMA)
			{
				while (stack.top().lexemeType != LEX_LEFT_BRACKET)
				{
					list->push_front(stack.top());
					stack.pop();
				}
			}
			else if (isBinaryOperator(lextable.table[lextableScanPos].originalLexeme))
			{
				while (
					!stack.empty() 
					&& stack.top().lexemeType != LEX_LEFT_BRACKET 
					&& (
						isPrefixOperator(stack.top().originalLexeme) 
						|| (idtable.table[stack.top().identifierTableIndex].idtype == IT::IdType::function ? 
							priorities["_function_call"]
							: priorities[stack.top().originalLexeme] >= priorities[lextable.table[lextableScanPos].originalLexeme])
						)
					)
				{
					list->push_front(stack.top());
					stack.pop();
				}
				stack.push(lextable.table[lextableScanPos]);
			}
			else if (lextable.table[lextableScanPos].lexemeType == LEX_RIGHT_BRACKET)
			{
				if (stack.empty())
				{
					return list;
				}
				while (stack.top().lexemeType != LEX_LEFT_BRACKET)
				{
					list->push_front(stack.top());
					stack.pop();
					if (stack.empty())
					{
						return list;
					}
				}
				stack.pop();
				if (stack.top().lexemeType == LEX_IDENTIFIER)
				{
					if (idtable.table[stack.top().identifierTableIndex].idtype == IT::IdType::function)
					{
						list->push_front(stack.top());
						stack.pop();
					}
				}
			}

			lextableScanPos++;
		}

		while (!stack.empty())
		{
			list->push_front(stack.top());
			stack.pop();
		}

		//LT::Entry null = *new LT::Entry;
		//null.lexem[0] = '@';
		
		/*while (LT::GetEntry(lextable, lextableInsertPos).lexemeType != LEX_SEMICOLON)
		{
			setLexem(lextable, '\0', lextableInsertPos);
			lextableInsertPos++;
			//LT::Replace(lextable, null, lextableInsertPos++);
		}*/

		return list;
	}

}