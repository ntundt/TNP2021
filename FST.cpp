#pragma once

#include "stdafx.h"

#include "FST.h"

namespace FSM {

	StateRelation::StateRelation(char symbol, short targetNode)
	{
		this->symbol = symbol;
		this->targetNode = targetNode;
	}

	State::State()
	{
		this->numberOfRelations = 0;
		this->relations = NULL;
	}

	State::State(short numberOfRelations, StateRelation rel, ...)
	{
		this->numberOfRelations = numberOfRelations;
		this->relations = new StateRelation[numberOfRelations];
		StateRelation* tmp = &rel;
		for (int i = 0; i < numberOfRelations; i++)
		{
			this->relations[i].symbol = tmp->symbol;
			this->relations[i].targetNode = tmp->targetNode;
			tmp++;
		}
	}

	FiniteStateMachine::FiniteStateMachine(short numberOfStates, State n, ...)
	{
		this->numberOfStates = numberOfStates;
		this->nodes = new State[numberOfStates];
		State* ptr = &n;
		for (int i = 0; i < numberOfStates; i++)
		{
			nodes[i] = ptr[i];
		}
		this->rstates = new short[numberOfStates];
		memset(this->rstates, 0xFF, sizeof(short) * numberOfStates);
		this->rstates[0] = 0;
		this->position = -1;
	}

	bool step(FiniteStateMachine& fst, short*& rstates)
	{
		bool rc = false;
		std::swap(rstates, fst.rstates);

		for (short i = 0; i < fst.numberOfStates; i++)
		{
			if (rstates[i] == fst.position)
			{
				for (short j = 0; j < fst.nodes[i].numberOfRelations; j++)
				{
					if (fst.nodes[i].relations[j].symbol == fst.chain[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].targetNode] = fst.position + 1;
						rc = true;
					}
				}
			}
		}

		return rc;
	}

	bool parseChain(FiniteStateMachine& fsm, const char* chain)
	{
		fsm.chain = (char*)chain;
		fsm.position = -1;
		short* rstates = new short[fsm.numberOfStates];
		memset(rstates, 0xFF, sizeof(short) * fsm.numberOfStates);
		memset(fsm.rstates, 0xFF, sizeof(short) * fsm.numberOfStates);
		fsm.rstates[0] = 0;
		short chainLength = strlen(fsm.chain);
		bool rc = true;
		for (short i = 0; i < chainLength && rc; i++)
		{
			fsm.position++;
			rc = step(fsm, rstates);
		}
		delete[] rstates;
		return (rc ? (fsm.rstates[fsm.numberOfStates - 1] == chainLength) : rc);
	}

}