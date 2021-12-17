#pragma once

#include "LT.h"
#include "IT.h"

namespace PolishNotation
{

	std::list<LT::Entry>* PolishNotation(int& lextableScanPos, LT::LexTable& lextable, IT::IdTable& idtable, bool terminateOnComma = false);

}