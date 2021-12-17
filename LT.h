#pragma once

#include <string>

#define LEXEM_FIXSIZE 1
#define LEXEM_MAXSIZE 32

#define LT_MAXSIZE 4096
#define LT_TI_NULLIDX 0xFFFFFFFF

#define LEX_DATATYPE 't'
#define LEX_LITERAL 'l'
#define LEX_FUNCTION 'f'
#define LEX_OPERATOR 'o'
#define LEX_IDENTIFIER 'i'
#define LEX_SEMICOLON ';'
#define LEX_COMMA ','
#define LEX_IF '7'
#define LEX_ELSE 'e'
#define LEX_ELSEIF '8'
#define LEX_CREATE 'c'
#define LEX_EQUALS '='
#define LEX_DESTROY 'd'
#define LEX_RETURN 'r'
#define LEX_PRINT 'p'
#define LEX_LEFT_BRACKET '('
#define LEX_RIGHT_BRACKET ')'
#define LEX_LEFT_CURLY_BRACKET '{'
#define LEX_RIGHT_CURLY_BRACKET '}'
#define LEX_LEFT_SQUARE_BRACKET '['
#define LEX_RIGHT_SQUARE_BRACKET ']'
#define LEX_EMPTY '\0'
#define LEX_PUSH_MARK 'P'

namespace LT
{
	typedef char lextype_t;

	struct Entry
	{
		lextype_t lexemeType = LEX_EMPTY;
		int stringNumber = 0;
		int stringPosition = 0;
		int identifierTableIndex = LT_TI_NULLIDX;
		char* originalLexeme = nullptr;
		Entry(char lexem, int stringNumber, int stringPosition);
		Entry();
	};

	struct LexTable
	{
		int maxSize;
		int size;
		Entry* table;
	};

	LexTable Create(int size);

	void Add(LexTable& lextable, Entry entry);

	void Replace(LexTable& lextable, Entry entry, int n);

	Entry GetEntry(LexTable& lextable, int n);

	void RemoveEmptyEntries(LexTable& lextable);

	char* GetLine(LexTable& lexTable, int index);

	void Delete(LexTable& lextable);

	std::string ToString(LexTable& lextable);

}