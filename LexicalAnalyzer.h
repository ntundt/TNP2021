#pragma once

#include "stdafx.h"

#include "IT.h"
#include "LT.h"
#include "FST.h"

namespace LexicalAnalyzer
{

	enum class State
	{
		Replacing,
		WaitingForDatatype,
		WaitingForIdentifierOrFunction,
		WaitingForIdentifier,
		WaitingForFunctionIdentifier,
		WaitingForParametersList,
		WaitingForParameterDatatype,
		WaitingForParameterIdentifier
	};

	const std::regex lexeme("('.*'|;|\\{|\\}|\\(|\\)|=|,|\\+|\\*|/|-|[a-zA-Z_][a-zA-Z0-9_]*|[0-9]+)");

	static FSM::FiniteStateMachine main_(
		5,
		FSM::State(1, FSM::StateRelation('m', 1)),
		FSM::State(1, FSM::StateRelation('a', 2)),
		FSM::State(1, FSM::StateRelation('i', 3)),
		FSM::State(1, FSM::StateRelation('n', 4)),
		FSM::State()
	);

	FSM::FiniteStateMachine string_(
		6,
		FSM::State(1, FSM::StateRelation('s', 1)),
		FSM::State(1, FSM::StateRelation('t', 2)),
		FSM::State(1, FSM::StateRelation('r', 3)),
		FSM::State(1, FSM::StateRelation('i', 4)),
		FSM::State(1, FSM::StateRelation('n', 5)),
		FSM::State(1, FSM::StateRelation('g', 6)),
		FSM::State()
	);

	static FSM::FiniteStateMachine string_literal_(
		4,
		FSM::State(2, FSM::StateRelation('\'', 1), FSM::StateRelation('\'', 2)),

		FSM::State(314,
			FSM::StateRelation('a', 1), FSM::StateRelation('b', 1), FSM::StateRelation('c', 1), FSM::StateRelation('d', 1),
			FSM::StateRelation('e', 1), FSM::StateRelation('f', 1), FSM::StateRelation('g', 1), FSM::StateRelation('h', 1),
			FSM::StateRelation('i', 1), FSM::StateRelation('j', 1), FSM::StateRelation('k', 1), FSM::StateRelation('l', 1),
			FSM::StateRelation('m', 1), FSM::StateRelation('n', 1), FSM::StateRelation('o', 1), FSM::StateRelation('p', 1),
			FSM::StateRelation('q', 1), FSM::StateRelation('r', 1), FSM::StateRelation('s', 1), FSM::StateRelation('t', 1),
			FSM::StateRelation('u', 1), FSM::StateRelation('v', 1), FSM::StateRelation('w', 1), FSM::StateRelation('x', 1),
			FSM::StateRelation('y', 1), FSM::StateRelation('z', 1),
			FSM::StateRelation('A', 1), FSM::StateRelation('B', 1), FSM::StateRelation('C', 1), FSM::StateRelation('D', 1),
			FSM::StateRelation('E', 1), FSM::StateRelation('F', 1), FSM::StateRelation('G', 1), FSM::StateRelation('H', 1),
			FSM::StateRelation('I', 1), FSM::StateRelation('J', 1), FSM::StateRelation('K', 1), FSM::StateRelation('L', 1),
			FSM::StateRelation('M', 1), FSM::StateRelation('N', 1), FSM::StateRelation('O', 1), FSM::StateRelation('P', 1),
			FSM::StateRelation('Q', 1), FSM::StateRelation('R', 1), FSM::StateRelation('S', 1), FSM::StateRelation('T', 1),
			FSM::StateRelation('U', 1), FSM::StateRelation('V', 1), FSM::StateRelation('W', 1), FSM::StateRelation('X', 1),
			FSM::StateRelation('Y', 1), FSM::StateRelation('Z', 1), FSM::StateRelation('_', 1),

			FSM::StateRelation('à', 1), FSM::StateRelation('á', 1), FSM::StateRelation('â', 1), FSM::StateRelation('ã', 1),
			FSM::StateRelation('ä', 1), FSM::StateRelation('å', 1), FSM::StateRelation('¸', 1), FSM::StateRelation('æ', 1),
			FSM::StateRelation('ç', 1), FSM::StateRelation('è', 1), FSM::StateRelation('é', 1), FSM::StateRelation('ê', 1),
			FSM::StateRelation('ë', 1), FSM::StateRelation('ì', 1), FSM::StateRelation('í', 1), FSM::StateRelation('î', 1),
			FSM::StateRelation('ï', 1), FSM::StateRelation('ð', 1), FSM::StateRelation('ñ', 1), FSM::StateRelation('ò', 1),
			FSM::StateRelation('ó', 1), FSM::StateRelation('ô', 1), FSM::StateRelation('õ', 1), FSM::StateRelation('ö', 1),
			FSM::StateRelation('÷', 1), FSM::StateRelation('ø', 1), FSM::StateRelation('ù', 1), FSM::StateRelation('ú', 1),
			FSM::StateRelation('û', 1), FSM::StateRelation('ü', 1), FSM::StateRelation('ý', 1), FSM::StateRelation('þ', 1),
			FSM::StateRelation('ÿ', 1),
			FSM::StateRelation('À', 1), FSM::StateRelation('Á', 1), FSM::StateRelation('Â', 1), FSM::StateRelation('Ã', 1),
			FSM::StateRelation('Ä', 1), FSM::StateRelation('Å', 1), FSM::StateRelation('¨', 1), FSM::StateRelation('Æ', 1),
			FSM::StateRelation('Ç', 1), FSM::StateRelation('È', 1), FSM::StateRelation('É', 1), FSM::StateRelation('Ê', 1),
			FSM::StateRelation('Ë', 1), FSM::StateRelation('Ì', 1), FSM::StateRelation('Í', 1), FSM::StateRelation('Î', 1),
			FSM::StateRelation('Ï', 1), FSM::StateRelation('Ð', 1), FSM::StateRelation('Ñ', 1), FSM::StateRelation('Ò', 1),
			FSM::StateRelation('Ó', 1), FSM::StateRelation('Ô', 1), FSM::StateRelation('Õ', 1), FSM::StateRelation('Ö', 1),
			FSM::StateRelation('×', 1), FSM::StateRelation('Ø', 1), FSM::StateRelation('Ù', 1), FSM::StateRelation('Ú', 1),
			FSM::StateRelation('Û', 1), FSM::StateRelation('Ü', 1), FSM::StateRelation('Ý', 1), FSM::StateRelation('Þ', 1),
			FSM::StateRelation('ß', 1),

			FSM::StateRelation('0', 1), FSM::StateRelation('1', 1), FSM::StateRelation('2', 1), FSM::StateRelation('3', 1),
			FSM::StateRelation('4', 1), FSM::StateRelation('5', 1), FSM::StateRelation('6', 1), FSM::StateRelation('7', 1),
			FSM::StateRelation('8', 1), FSM::StateRelation('9', 1),
			FSM::StateRelation('.', 1), FSM::StateRelation(',', 1), FSM::StateRelation(':', 1), FSM::StateRelation(';', 1),
			FSM::StateRelation('"', 1), FSM::StateRelation('<', 1), FSM::StateRelation('>', 1), FSM::StateRelation('(', 1),
			FSM::StateRelation(')', 1), FSM::StateRelation('[', 1), FSM::StateRelation(']', 1), FSM::StateRelation('!', 1),
			FSM::StateRelation('?', 1), FSM::StateRelation('@', 1), FSM::StateRelation('{', 1), FSM::StateRelation('}', 1),
			FSM::StateRelation('|', 1), FSM::StateRelation('/', 1), FSM::StateRelation('#', 1), FSM::StateRelation('$', 1),
			FSM::StateRelation('%', 1), FSM::StateRelation('^', 1), FSM::StateRelation('*', 1), FSM::StateRelation('=', 1),
			FSM::StateRelation('+', 1), FSM::StateRelation('-', 1), FSM::StateRelation('`', 1), FSM::StateRelation(' ', 1),

			FSM::StateRelation('a', 2), FSM::StateRelation('b', 2), FSM::StateRelation('c', 2), FSM::StateRelation('d', 2),
			FSM::StateRelation('e', 2), FSM::StateRelation('f', 2), FSM::StateRelation('g', 2), FSM::StateRelation('h', 2),
			FSM::StateRelation('i', 2), FSM::StateRelation('j', 2), FSM::StateRelation('k', 2), FSM::StateRelation('l', 2),
			FSM::StateRelation('m', 2), FSM::StateRelation('n', 2), FSM::StateRelation('o', 2), FSM::StateRelation('p', 2),
			FSM::StateRelation('q', 2), FSM::StateRelation('r', 2), FSM::StateRelation('s', 2), FSM::StateRelation('t', 2),
			FSM::StateRelation('u', 2), FSM::StateRelation('v', 2), FSM::StateRelation('w', 2), FSM::StateRelation('x', 2),
			FSM::StateRelation('y', 2), FSM::StateRelation('z', 2),
			FSM::StateRelation('A', 2), FSM::StateRelation('B', 2), FSM::StateRelation('C', 2), FSM::StateRelation('D', 2),
			FSM::StateRelation('E', 2), FSM::StateRelation('F', 2), FSM::StateRelation('G', 2), FSM::StateRelation('H', 2),
			FSM::StateRelation('I', 2), FSM::StateRelation('J', 2), FSM::StateRelation('K', 2), FSM::StateRelation('L', 2),
			FSM::StateRelation('M', 2), FSM::StateRelation('N', 2), FSM::StateRelation('O', 2), FSM::StateRelation('P', 2),
			FSM::StateRelation('Q', 2), FSM::StateRelation('R', 2), FSM::StateRelation('S', 2), FSM::StateRelation('T', 2),
			FSM::StateRelation('U', 2), FSM::StateRelation('V', 2), FSM::StateRelation('W', 2), FSM::StateRelation('X', 2),
			FSM::StateRelation('Y', 2), FSM::StateRelation('Z', 2), FSM::StateRelation('_', 2),

			FSM::StateRelation('à', 2), FSM::StateRelation('á', 2), FSM::StateRelation('â', 2), FSM::StateRelation('ã', 2),
			FSM::StateRelation('ä', 2), FSM::StateRelation('å', 2), FSM::StateRelation('¸', 2), FSM::StateRelation('æ', 2),
			FSM::StateRelation('ç', 2), FSM::StateRelation('è', 2), FSM::StateRelation('é', 2), FSM::StateRelation('ê', 2),
			FSM::StateRelation('ë', 2), FSM::StateRelation('ì', 2), FSM::StateRelation('í', 2), FSM::StateRelation('î', 2),
			FSM::StateRelation('ï', 2), FSM::StateRelation('ð', 2), FSM::StateRelation('ñ', 2), FSM::StateRelation('ò', 2),
			FSM::StateRelation('ó', 2), FSM::StateRelation('ô', 2), FSM::StateRelation('õ', 2), FSM::StateRelation('ö', 2),
			FSM::StateRelation('÷', 2), FSM::StateRelation('ø', 2), FSM::StateRelation('ù', 2), FSM::StateRelation('ú', 2),
			FSM::StateRelation('û', 2), FSM::StateRelation('ü', 2), FSM::StateRelation('ý', 2), FSM::StateRelation('þ', 2),
			FSM::StateRelation('ÿ', 2),
			FSM::StateRelation('À', 2), FSM::StateRelation('Á', 2), FSM::StateRelation('Â', 2), FSM::StateRelation('Ã', 2),
			FSM::StateRelation('Ä', 2), FSM::StateRelation('Å', 2), FSM::StateRelation('¨', 2), FSM::StateRelation('Æ', 2),
			FSM::StateRelation('Ç', 2), FSM::StateRelation('È', 2), FSM::StateRelation('É', 2), FSM::StateRelation('Ê', 2),
			FSM::StateRelation('Ë', 2), FSM::StateRelation('Ì', 2), FSM::StateRelation('Í', 2), FSM::StateRelation('Î', 2),
			FSM::StateRelation('Ï', 2), FSM::StateRelation('Ð', 2), FSM::StateRelation('Ñ', 2), FSM::StateRelation('Ò', 2),
			FSM::StateRelation('Ó', 2), FSM::StateRelation('Ô', 2), FSM::StateRelation('Õ', 2), FSM::StateRelation('Ö', 2),
			FSM::StateRelation('×', 2), FSM::StateRelation('Ø', 2), FSM::StateRelation('Ù', 2), FSM::StateRelation('Ú', 2),
			FSM::StateRelation('Û', 2), FSM::StateRelation('Ü', 2), FSM::StateRelation('Ý', 2), FSM::StateRelation('Þ', 2),
			FSM::StateRelation('ß', 2),

			FSM::StateRelation('0', 2), FSM::StateRelation('1', 2), FSM::StateRelation('2', 2), FSM::StateRelation('3', 2),
			FSM::StateRelation('4', 2), FSM::StateRelation('5', 2), FSM::StateRelation('6', 2), FSM::StateRelation('7', 2),
			FSM::StateRelation('8', 2), FSM::StateRelation('9', 2),
			FSM::StateRelation('.', 2), FSM::StateRelation(',', 2), FSM::StateRelation(':', 2), FSM::StateRelation(';', 2),
			FSM::StateRelation('"', 2), FSM::StateRelation('<', 2), FSM::StateRelation('>', 2), FSM::StateRelation('(', 2),
			FSM::StateRelation(')', 2), FSM::StateRelation('[', 2), FSM::StateRelation(']', 2), FSM::StateRelation('!', 2),
			FSM::StateRelation('?', 2), FSM::StateRelation('@', 2), FSM::StateRelation('{', 2), FSM::StateRelation('}', 2),
			FSM::StateRelation('|', 2), FSM::StateRelation('/', 2), FSM::StateRelation('#', 2), FSM::StateRelation('$', 2),
			FSM::StateRelation('%', 2), FSM::StateRelation('^', 2), FSM::StateRelation('*', 2), FSM::StateRelation('=', 2),
			FSM::StateRelation('+', 2), FSM::StateRelation('-', 2), FSM::StateRelation('`', 2), FSM::StateRelation(' ', 2)),

		FSM::State(1, FSM::StateRelation('\'', 3)),

		FSM::State()
	);

	static FSM::FiniteStateMachine identifier_(
		3,
		FSM::State(106,
			FSM::StateRelation('a', 1), FSM::StateRelation('b', 1), FSM::StateRelation('c', 1), FSM::StateRelation('d', 1),
			FSM::StateRelation('e', 1), FSM::StateRelation('f', 1), FSM::StateRelation('g', 1), FSM::StateRelation('h', 1),
			FSM::StateRelation('i', 1), FSM::StateRelation('j', 1), FSM::StateRelation('k', 1), FSM::StateRelation('l', 1),
			FSM::StateRelation('m', 1), FSM::StateRelation('n', 1), FSM::StateRelation('o', 1), FSM::StateRelation('p', 1),
			FSM::StateRelation('q', 1), FSM::StateRelation('r', 1), FSM::StateRelation('s', 1), FSM::StateRelation('t', 1),
			FSM::StateRelation('u', 1), FSM::StateRelation('v', 1), FSM::StateRelation('w', 1), FSM::StateRelation('x', 1),
			FSM::StateRelation('y', 1), FSM::StateRelation('z', 1),
			FSM::StateRelation('A', 1), FSM::StateRelation('B', 1), FSM::StateRelation('C', 1), FSM::StateRelation('D', 1),
			FSM::StateRelation('E', 1), FSM::StateRelation('F', 1), FSM::StateRelation('G', 1), FSM::StateRelation('H', 1),
			FSM::StateRelation('I', 1), FSM::StateRelation('J', 1), FSM::StateRelation('K', 1), FSM::StateRelation('L', 1),
			FSM::StateRelation('M', 1), FSM::StateRelation('N', 1), FSM::StateRelation('O', 1), FSM::StateRelation('P', 1),
			FSM::StateRelation('Q', 1), FSM::StateRelation('R', 1), FSM::StateRelation('S', 1), FSM::StateRelation('T', 1),
			FSM::StateRelation('U', 1), FSM::StateRelation('V', 1), FSM::StateRelation('W', 1), FSM::StateRelation('X', 1),
			FSM::StateRelation('Y', 1), FSM::StateRelation('Z', 1), FSM::StateRelation('_', 1),

			FSM::StateRelation('a', 2), FSM::StateRelation('b', 2), FSM::StateRelation('c', 2), FSM::StateRelation('d', 2),
			FSM::StateRelation('e', 2), FSM::StateRelation('f', 2), FSM::StateRelation('g', 2), FSM::StateRelation('h', 2),
			FSM::StateRelation('i', 2), FSM::StateRelation('j', 2), FSM::StateRelation('k', 2), FSM::StateRelation('l', 2),
			FSM::StateRelation('m', 2), FSM::StateRelation('n', 2), FSM::StateRelation('o', 2), FSM::StateRelation('p', 2),
			FSM::StateRelation('q', 2), FSM::StateRelation('r', 2), FSM::StateRelation('s', 2), FSM::StateRelation('t', 2),
			FSM::StateRelation('u', 2), FSM::StateRelation('v', 2), FSM::StateRelation('w', 2), FSM::StateRelation('x', 2),
			FSM::StateRelation('y', 2), FSM::StateRelation('z', 2),
			FSM::StateRelation('A', 2), FSM::StateRelation('B', 2), FSM::StateRelation('C', 2), FSM::StateRelation('D', 2),
			FSM::StateRelation('E', 2), FSM::StateRelation('F', 2), FSM::StateRelation('G', 2), FSM::StateRelation('H', 2),
			FSM::StateRelation('I', 2), FSM::StateRelation('J', 2), FSM::StateRelation('K', 2), FSM::StateRelation('L', 2),
			FSM::StateRelation('M', 2), FSM::StateRelation('N', 2), FSM::StateRelation('O', 2), FSM::StateRelation('P', 2),
			FSM::StateRelation('Q', 2), FSM::StateRelation('R', 2), FSM::StateRelation('S', 2), FSM::StateRelation('T', 2),
			FSM::StateRelation('U', 2), FSM::StateRelation('V', 2), FSM::StateRelation('W', 2), FSM::StateRelation('X', 2),
			FSM::StateRelation('Y', 2), FSM::StateRelation('Z', 2), FSM::StateRelation('_', 2)),

		FSM::State(126,
			FSM::StateRelation('a', 1), FSM::StateRelation('b', 1), FSM::StateRelation('c', 1), FSM::StateRelation('d', 1),
			FSM::StateRelation('e', 1), FSM::StateRelation('f', 1), FSM::StateRelation('g', 1), FSM::StateRelation('h', 1),
			FSM::StateRelation('i', 1), FSM::StateRelation('j', 1), FSM::StateRelation('k', 1), FSM::StateRelation('l', 1),
			FSM::StateRelation('m', 1), FSM::StateRelation('n', 1), FSM::StateRelation('o', 1), FSM::StateRelation('p', 1),
			FSM::StateRelation('q', 1), FSM::StateRelation('r', 1), FSM::StateRelation('s', 1), FSM::StateRelation('t', 1),
			FSM::StateRelation('u', 1), FSM::StateRelation('v', 1), FSM::StateRelation('w', 1), FSM::StateRelation('x', 1),
			FSM::StateRelation('y', 1), FSM::StateRelation('z', 1),
			FSM::StateRelation('A', 1), FSM::StateRelation('B', 1), FSM::StateRelation('C', 1), FSM::StateRelation('D', 1),
			FSM::StateRelation('E', 1), FSM::StateRelation('F', 1), FSM::StateRelation('G', 1), FSM::StateRelation('H', 1),
			FSM::StateRelation('I', 1), FSM::StateRelation('J', 1), FSM::StateRelation('K', 1), FSM::StateRelation('L', 1),
			FSM::StateRelation('M', 1), FSM::StateRelation('N', 1), FSM::StateRelation('O', 1), FSM::StateRelation('P', 1),
			FSM::StateRelation('Q', 1), FSM::StateRelation('R', 1), FSM::StateRelation('S', 1), FSM::StateRelation('T', 1),
			FSM::StateRelation('U', 1), FSM::StateRelation('V', 1), FSM::StateRelation('W', 1), FSM::StateRelation('X', 1),
			FSM::StateRelation('Y', 1), FSM::StateRelation('Z', 1), FSM::StateRelation('_', 1),
			FSM::StateRelation('0', 1), FSM::StateRelation('1', 1), FSM::StateRelation('2', 1), FSM::StateRelation('3', 1),
			FSM::StateRelation('4', 1), FSM::StateRelation('5', 1), FSM::StateRelation('6', 1), FSM::StateRelation('7', 1),
			FSM::StateRelation('8', 1), FSM::StateRelation('9', 1),

			FSM::StateRelation('a', 2), FSM::StateRelation('b', 2), FSM::StateRelation('c', 2), FSM::StateRelation('d', 2),
			FSM::StateRelation('e', 2), FSM::StateRelation('f', 2), FSM::StateRelation('g', 2), FSM::StateRelation('h', 2),
			FSM::StateRelation('i', 2), FSM::StateRelation('j', 2), FSM::StateRelation('k', 2), FSM::StateRelation('l', 2),
			FSM::StateRelation('m', 2), FSM::StateRelation('n', 2), FSM::StateRelation('o', 2), FSM::StateRelation('p', 2),
			FSM::StateRelation('q', 2), FSM::StateRelation('r', 2), FSM::StateRelation('s', 2), FSM::StateRelation('t', 2),
			FSM::StateRelation('u', 2), FSM::StateRelation('v', 2), FSM::StateRelation('w', 2), FSM::StateRelation('x', 2),
			FSM::StateRelation('y', 2), FSM::StateRelation('z', 2),
			FSM::StateRelation('A', 2), FSM::StateRelation('B', 2), FSM::StateRelation('C', 2), FSM::StateRelation('D', 2),
			FSM::StateRelation('E', 2), FSM::StateRelation('F', 2), FSM::StateRelation('G', 2), FSM::StateRelation('H', 2),
			FSM::StateRelation('I', 2), FSM::StateRelation('J', 2), FSM::StateRelation('K', 2), FSM::StateRelation('L', 2),
			FSM::StateRelation('M', 2), FSM::StateRelation('N', 2), FSM::StateRelation('O', 2), FSM::StateRelation('P', 2),
			FSM::StateRelation('Q', 2), FSM::StateRelation('R', 2), FSM::StateRelation('S', 2), FSM::StateRelation('T', 2),
			FSM::StateRelation('U', 2), FSM::StateRelation('V', 2), FSM::StateRelation('W', 2), FSM::StateRelation('X', 2),
			FSM::StateRelation('Y', 2), FSM::StateRelation('Z', 2), FSM::StateRelation('_', 2),
			FSM::StateRelation('0', 2), FSM::StateRelation('1', 2), FSM::StateRelation('2', 2), FSM::StateRelation('3', 2),
			FSM::StateRelation('4', 2), FSM::StateRelation('5', 2), FSM::StateRelation('6', 2), FSM::StateRelation('7', 2),
			FSM::StateRelation('8', 2), FSM::StateRelation('9', 2)),

		FSM::State()
	);

	static FSM::FiniteStateMachine integer_literal_(
		2,
		FSM::State(20,
			FSM::StateRelation('0', 0), FSM::StateRelation('1', 0), FSM::StateRelation('2', 0), FSM::StateRelation('3', 0),
			FSM::StateRelation('4', 0), FSM::StateRelation('5', 0), FSM::StateRelation('6', 0), FSM::StateRelation('7', 0),
			FSM::StateRelation('8', 0), FSM::StateRelation('9', 0),
			FSM::StateRelation('0', 1), FSM::StateRelation('1', 1), FSM::StateRelation('2', 1), FSM::StateRelation('3', 1),
			FSM::StateRelation('4', 1), FSM::StateRelation('5', 1), FSM::StateRelation('6', 1), FSM::StateRelation('7', 1),
			FSM::StateRelation('8', 1), FSM::StateRelation('9', 1)
		),
		FSM::State()
	);

	static FSM::FiniteStateMachine keyword_(
		38,
		FSM::State(7,
			FSM::StateRelation('d', 1), FSM::StateRelation('i', 7), FSM::StateRelation('s', 13), FSM::StateRelation('p', 18),
			FSM::StateRelation('f', 22), FSM::StateRelation('r', 29), FSM::StateRelation('m', 34)
		), // 0
		FSM::State(1, FSM::StateRelation('e', 2)), // 1
		FSM::State(1, FSM::StateRelation('c', 3)), // 2
		FSM::State(1, FSM::StateRelation('l', 4)), // 3
		FSM::State(1, FSM::StateRelation('a', 5)), // 4 
		FSM::State(1, FSM::StateRelation('r', 6)), // 5
		FSM::State(1, FSM::StateRelation('e', 37)), // 6 -> end
		FSM::State(1, FSM::StateRelation('n', 8)), // 7
		FSM::State(1, FSM::StateRelation('t', 9)), // 8
		FSM::State(1, FSM::StateRelation('e', 10)), // 9
		FSM::State(1, FSM::StateRelation('g', 11)), // 10
		FSM::State(1, FSM::StateRelation('e', 12)), // 11
		FSM::State(1, FSM::StateRelation('r', 37)), // 12 -> end
		FSM::State(1, FSM::StateRelation('t', 14)), // 13
		FSM::State(1, FSM::StateRelation('r', 15)), // 14
		FSM::State(1, FSM::StateRelation('i', 16)), // 15
		FSM::State(1, FSM::StateRelation('n', 17)), // 16
		FSM::State(1, FSM::StateRelation('g', 37)), // 17 -> end
		FSM::State(1, FSM::StateRelation('r', 19)), // 18
		FSM::State(1, FSM::StateRelation('i', 20)), // 19
		FSM::State(1, FSM::StateRelation('n', 21)), // 20
		FSM::State(1, FSM::StateRelation('t', 37)), // 21 -> end
		FSM::State(1, FSM::StateRelation('u', 23)), // 22
		FSM::State(1, FSM::StateRelation('n', 24)), // 23
		FSM::State(1, FSM::StateRelation('c', 25)), // 24
		FSM::State(1, FSM::StateRelation('t', 26)), // 25
		FSM::State(1, FSM::StateRelation('i', 27)), // 26
		FSM::State(1, FSM::StateRelation('o', 28)), // 27
		FSM::State(1, FSM::StateRelation('n', 37)), // 28 -> end
		FSM::State(1, FSM::StateRelation('e', 30)), // 29
		FSM::State(1, FSM::StateRelation('t', 31)), // 30
		FSM::State(1, FSM::StateRelation('u', 32)), // 31
		FSM::State(1, FSM::StateRelation('r', 33)), // 32
		FSM::State(1, FSM::StateRelation('n', 37)), // 33 -> end
		FSM::State(1, FSM::StateRelation('a', 35)), // 34
		FSM::State(1, FSM::StateRelation('i', 36)), // 35
		FSM::State(1, FSM::StateRelation('n', 37)), // 36 -> end
		FSM::State() // 37
	);

	static FSM::FiniteStateMachine datatype_(
		13,
		FSM::State(2,
			FSM::StateRelation('i', 1), FSM::StateRelation('s', 7)
		), // 0
		FSM::State(1, FSM::StateRelation('n', 2)), // 1
		FSM::State(1, FSM::StateRelation('t', 3)), // 2
		FSM::State(1, FSM::StateRelation('e', 4)), // 3
		FSM::State(1, FSM::StateRelation('g', 5)), // 4
		FSM::State(1, FSM::StateRelation('e', 6)), // 5
		FSM::State(1, FSM::StateRelation('r', 12)), // 6 -> end
		FSM::State(1, FSM::StateRelation('t', 8)), // 7
		FSM::State(1, FSM::StateRelation('r', 9)), // 8
		FSM::State(1, FSM::StateRelation('i', 10)), // 9
		FSM::State(1, FSM::StateRelation('n', 11)), // 10
		FSM::State(1, FSM::StateRelation('g', 12)), // 11 -> end
		FSM::State() // 12
	);

	static FSM::FiniteStateMachine function_(
		9,
		FSM::State(1, FSM::StateRelation('f', 1)),
		FSM::State(1, FSM::StateRelation('u', 2)),
		FSM::State(1, FSM::StateRelation('n', 3)),
		FSM::State(1, FSM::StateRelation('c', 4)),
		FSM::State(1, FSM::StateRelation('t', 5)),
		FSM::State(1, FSM::StateRelation('i', 6)),
		FSM::State(1, FSM::StateRelation('o', 7)),
		FSM::State(1, FSM::StateRelation('n', 8)),
		FSM::State()
	);

	static FSM::FiniteStateMachine operator_sign_(
		2,
		FSM::State(5,
			FSM::StateRelation('+', 1), FSM::StateRelation('-', 1), FSM::StateRelation('*', 1), FSM::StateRelation('/', 1),
			FSM::StateRelation('=', 1)
		),
		FSM::State()
	);

	void TNP2021(const char* program, LT::LexTable& lexTable, IT::IdTable& idTable);

	//ParseResults SVV2015(std::string program);

};