#pragma once

#include "stdafx.h"

#include "Greibach.h"

#define GRB_ERROR_SERIES 600

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'), 6,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, 6,
			Rule::Chain(5, TS('c'), TS('t'), TS('i'), TS(';'), NS('S')),
			Rule::Chain(12, TS('c'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('T'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(11, TS('c'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('T'), TS('}'), TS(';'), NS('S')),

			Rule::Chain(4, TS('c'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(11, TS('c'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('T'), TS('}'), TS(';')),
			Rule::Chain(10, TS('c'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('T'), TS('}'), TS(';'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 1, 2,
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F')),

			Rule::Chain(2, TS('t'), TS('i'))
		),
		Rule(NS('T'), GRB_ERROR_SERIES + 2, 20,
			Rule::Chain(10, TS('7'), TS('('), NS('E'), TS(')'), TS('{'), NS('T'), TS('}'), NS('R'), TS(';'), NS('T')),
			Rule::Chain(9, TS('7'), TS('('), NS('E'), TS(')'), TS('{'), NS('T'), TS('}'), TS(';'), NS('T')),
			Rule::Chain(5, TS('c'), TS('t'), TS('i'), TS(';'), NS('T')),
			Rule::Chain(4, TS('d'), TS('i'), TS(';'), NS('T')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('T')),
			Rule::Chain(5, TS('i'), TS('('), TS(')'), TS(';'), NS('T')),
			Rule::Chain(6, TS('i'), TS('('), NS('U'), TS(')'), TS(';'), NS('T')),
			Rule::Chain(6, TS('p'), TS('('), NS('E'), TS(')'), TS(';'), NS('T')),
			Rule::Chain(6, TS('n'), TS('('), TS('i'), TS(')'), TS(';'), NS('T')),
			Rule::Chain(6, TS('r'), TS('('), NS('E'), TS(')'), TS(';'), NS('T')),

			Rule::Chain(9, TS('7'), TS('('), NS('E'), TS(')'), TS('{'), NS('T'), TS('}'), NS('R'), TS(';')),
			Rule::Chain(8, TS('7'), TS('('), NS('E'), TS(')'), TS('{'), NS('T'), TS('}'), TS(';')),
			Rule::Chain(4, TS('c'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('d'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), TS(';')),
			Rule::Chain(5, TS('i'), TS('('), NS('U'), TS(')'), TS(';')),
			Rule::Chain(5, TS('p'), TS('('), NS('E'), TS(')'), TS(';')),
			Rule::Chain(5, TS('n'), TS('('), TS('i'), TS(')'), TS(';')),
			Rule::Chain(5, TS('r'), TS('('), NS('E'), TS(')'), TS(';'))
		),
		Rule(NS('R'), GRB_ERROR_SERIES + 3, 3, 
			Rule::Chain(8, TS('8'), TS('('), NS('E'), TS(')'), TS('{'), NS('T'), TS('}'), NS('R')),
			Rule::Chain(7, TS('8'), TS('('), NS('E'), TS(')'), TS('{'), NS('T'), TS('}')),
			Rule::Chain(4, TS('e'), TS('{'), NS('T'), TS('}'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 4, 10,
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('U'), TS(')')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),

			Rule::Chain(3, TS('l'), TS('o'), NS('E')),
			Rule::Chain(3, TS('i'), TS('o'), NS('E')),
			Rule::Chain(5, TS('i'), TS('('), TS(')'), TS('o'), NS('E')),
			Rule::Chain(6, TS('i'), TS('('), NS('U'), TS(')'), TS('o'), NS('E')),
			Rule::Chain(5, TS('('), NS('E'), TS(')'), TS('o'), NS('E'))
		),
		Rule(NS('U'), GRB_ERROR_SERIES + 5, 20,
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('U'), TS(')')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(3, TS('l'), TS('o'), NS('E')),
			Rule::Chain(3, TS('i'), TS('o'), NS('E')),
			Rule::Chain(5, TS('i'), TS('('), TS(')'), TS('o'), NS('E')),
			Rule::Chain(6, TS('i'), TS('('), NS('U'), TS(')'), TS('o'), NS('E')),
			Rule::Chain(5, TS('('), NS('E'), TS(')'), TS('o'), NS('E')),

			Rule::Chain(3, TS('l'), TS(','), NS('U')),
			Rule::Chain(3, TS('i'), TS(','), NS('U')),
			Rule::Chain(5, TS('i'), TS('('), TS(')'), TS(','), NS('U')),
			Rule::Chain(6, TS('i'), TS('('), NS('U'), TS(')'), TS(','), NS('U')),
			Rule::Chain(5, TS('('), NS('E'), TS(')'), TS(','), NS('U')),
			Rule::Chain(5, TS('l'), TS('o'), NS('E'), TS(','), NS('U')),
			Rule::Chain(5, TS('i'), TS('o'), NS('E'), TS(','), NS('U')),
			Rule::Chain(7, TS('i'), TS('('), TS(')'), TS('o'), NS('E'), TS(','), NS('U')),
			Rule::Chain(8, TS('i'), TS('('), NS('U'), TS(')'), TS('o'), NS('E'), TS(','), NS('U')),
			Rule::Chain(7, TS('('), NS('E'), TS(')'), TS('o'), NS('E'), TS(','), NS('U'))
		)
	);

	Rule::Chain::Chain(short parametersCount, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = parametersCount];
		int* p = (int*)&s;
		for (short i = 0; i < parametersCount; ++i) nt[i] = (GRBALPHABET)p[i];
	}

	Rule::Rule(GRBALPHABET baseSymbol, int piderror, short psize, Chain c, ...)
	{
		nn = baseSymbol;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	}

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	}

	Greibach getGreibach() { return greibach; }

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size and rules[k].nn != pnn) k++;
		if (k < size) prule = rules[rc = k];
		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size and chains[j].nt[0] != t) ++j;
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	}

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	}
}