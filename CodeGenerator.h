#pragma once

#include "stdafx.h"

#include "LT.h"
#include "IT.h"

namespace CodeGenerator
{
	std::string GenerateCode(LT::LexTable& lexTable, IT::IdTable& idTable);
}