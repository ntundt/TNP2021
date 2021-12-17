#pragma once

#include "stdafx.h"

namespace FSM
{

	struct StateRelation
	{
		char symbol;
		short targetNode;
		StateRelation(char c = 0x00, short ns = NULL);
	};

	struct State
	{
		short numberOfRelations;
		StateRelation* relations;
		State();
		State(short n, StateRelation rel, ...);
	};

	struct FiniteStateMachine
	{
		char* chain;
		short position;
		short numberOfStates;
		State* nodes;
		short* rstates;
		FiniteStateMachine(short numberOfStates, State n, ...);
	};

	bool parseChain(FiniteStateMachine& fst, char* chain);

}