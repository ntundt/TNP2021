#pragma once

#include "stdafx.h"

#include "LT.h"
#include "IT.h"

namespace Semantics
{
	IT::IdDatatype GetExpressionDatatype(LT::LexTable& lexemeTable, IT::IdTable& idTable, int position);

	void RunAllChecks(LT::LexTable& lexemeTable, IT::IdTable& identifierTable);
}