#pragma once

namespace Specification
{
	const int datatypesCount = 2;
	const char* datatypes[] = {
		"integer",
		"string"
	};

	std::regex* string_literal_ = new std::regex("\\\"([^\\\"]*)\\\"");
	std::regex* decimal_integer_literal_ = new std::regex("[0-9]+d{0,1}");
	std::regex* octal_integer_literal_ = new std::regex("[0-7]+o");
	std::regex* operator_ = new std::regex("\\+|\\*|-|/|%|==|<=|>=|<<|>>|<|>|!=");
	std::regex* identifier_ = new std::regex("[a-zA-Z_][a-zA-Z0-9_]*");

	std::regex lexeme("\\\"([^\\\"]*)\\\"|[a-zA-Z_][a-zA-Z0-9_]*|[0-7]+o|[0-9]+d{0,1}|\\+|\\*|<<|>>|-|/|%|==|=|<=|>=|<|>|!=|output|input|return|create|destroy|if|else|elseif|while|\\(|\\)|\\{|\\}|\\[|\\]|;|,");

}