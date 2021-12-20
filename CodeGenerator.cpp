#pragma once

#include "stdafx.h"

#include "LT.h"
#include "IT.h"

namespace CodeGenerator
{

	static int ifCounter = 0;
	static int ifBlockCounter = 0;
	static int elseIfCounter = 0;

	static int PRNCounter = 0;

	char* nextLabel(const char* prefix, uint32_t value, const char* suffix)
	{
		char* label = new char[20];
		char* buffer = new char[12];
		_itoa_s(value, buffer, 12, 10);
		strcpy_s(label, 20, prefix);
		strncat_s(label, 20, buffer, 12);
		strncat_s(label, 20, suffix, 12);
		delete[] buffer;
		return label;
	}

	void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}

	const char* cstrToAssemblyInitializer(const char* str)
	{
		std::string result(str);
		result = "\"" + result + "\",0";
		ReplaceStringInPlace(result, "\"\\n\"", "10");
		ReplaceStringInPlace(result, "\"\\n", "10,\"");
		ReplaceStringInPlace(result, "\\n\"", "\",10");
		ReplaceStringInPlace(result, "\\n", "\",10,\"");
		size_t size = result.length() + 1;
		char* r = new char[size];
		strcpy_s(r, size, result.c_str());
		return r;
	}

	struct AssemblyCodeLine
	{
		virtual char* toString() = 0;
	};

	struct Command : public AssemblyCodeLine
	{
		char* name = nullptr;
		char* parameter1 = nullptr;
		char* parameter2 = nullptr;
		Command(const char* name)
		{
			this->name = new char[strlen(name) + 1];
			strcpy_s(this->name, strlen(name) + 1, name);
		}
		Command(const char* name, const char* parameter1)
		{
			this->name = new char[strlen(name) + 1];
			strcpy_s(this->name, strlen(name) + 1, name);
			this->parameter1 = new char[strlen(parameter1) + 1];
			strcpy_s(this->parameter1, strlen(parameter1) + 1, parameter1);
		}
		Command(const char* name, const char* parameter1, const char* parameter2)
		{
			this->name = new char[strlen(name) + 1];
			strcpy_s(this->name, strlen(name) + 1, name);
			this->parameter1 = new char[strlen(parameter1) + 1];
			strcpy_s(this->parameter1, strlen(parameter1) + 1, parameter1);
			this->parameter2 = new char[strlen(parameter2) + 1];
			strcpy_s(this->parameter2, strlen(parameter2) + 1, parameter2);
		}
		char* toString() override
		{
			int namelen = strlen(this->name), par1len = 0, par2len = 0;
			if (this->parameter1 != nullptr)
			{
				par1len = strlen(this->parameter1);
			}
			if (this->parameter2 != nullptr)
			{
				par2len = strlen(this->parameter2);
			}
			char* result = new char[namelen + par1len + par2len + 4];
			strcpy_s(result, namelen + par1len + par2len + 4, this->name);
			strncat_s(result, namelen + par1len + par2len + 4, " ", 1);
			if (this->parameter1 != nullptr)
			{
				strncat_s(result, namelen + par1len + par2len + 4, this->parameter1, par1len);
			}
			if (this->parameter2 != nullptr)
			{
				strncat_s(result, namelen + par1len + par2len + 4, ",", 1);
				strncat_s(result, namelen + par1len + par2len + 4, this->parameter2, par2len);
			}
			return result;
		}
		~Command()
		{
			delete this->name;
			delete this->parameter1;
			delete this->parameter2;
		}
	};

	struct Label : public AssemblyCodeLine
	{
		char* name;
		Label(const char* name)
		{
			this->name = new char[strlen(name) + 1];
			strcpy_s(this->name, strlen(name) + 1, name);
		}
		char* toString() override
		{
			char* result = new char[strlen(this->name) + 2];
			strcpy_s(result, strlen(this->name) + 2, this->name);
			strncat_s(result, strlen(this->name) + 2, ":", 1);
			return result;
		}
		~Label()
		{
			delete this->name;
		}
	};
	
	struct Comment : public AssemblyCodeLine
	{
		char* contents;
		Comment(const char* contents)
		{
			this->contents = new char[strlen(contents) + 1];
			strcpy_s(this->contents, strlen(contents) + 1, contents);
		}
		char* toString() override
		{
			char* result = new char[strlen(this->contents) + 3];
			strcpy_s(result, strlen(this->contents) + 3, "; ");
			strncat_s(result, strlen(this->contents) + 3, this->contents, strlen(this->contents) + 2);
			return result;
		}
	};

	struct CodeBlock
	{
		std::vector<AssemblyCodeLine*> code;

		bool procedure = false;
		const char* name = "";
		std::vector<IT::Entry> parameters;

		void AddCommand(const char* name)
		{
			code.push_back(new Command(name));
		}

		void AddCommand(const char* name, const char* parameter1)
		{
			code.push_back(new Command(name, parameter1));
		}

		void AddCommand(const char* name, const char* parameter1, const char* parameter2)
		{
			code.push_back(new Command(name, parameter1, parameter2));
		}

		void AddCommand(Command command)
		{
			code.push_back(&command);
		}

		void AddLabel(const char* label)
		{
			code.push_back(new Label(label));
		}

		void AddComment(const char* comment)
		{
			code.push_back(new Comment(comment));
		}

		void AddParameter(IT::Entry entry)
		{
			this->parameters.push_back(entry);
		}

		std::string toString()
		{
			std::stringstream result;

			if (strcmp(name, "_@_main") == 0)
			{
				result << "main PROC";
			}
			else if (this->procedure)
			{
				result << name << " PROC uses eax ebx ecx edx edi esi";
			}
			else
			{
				result << name << " PROC uses ebx ecx edx edi esi";
			}

			for (unsigned int i = 0; i < parameters.size(); i++)
			{
				result << ", " << IT::GetQualifiedIdentifierName(parameters[i]) << " :dword";
			}
				
			if (strcmp(name, "_@_main") == 0)
			{
				result << "\npush offset consoleTitle\n"
					<< "call SetConsoleTitleA\n";
			}
			result << '\n';

			for (unsigned int i = 0; i < code.size(); i++)
			{
				result << code[i]->toString() << '\n';
			}


			if (strcmp(name, "_@_main") == 0)
			{
				result << "main ENDP";
			}
			else
			{
				result << name << " ENDP";
			}

			return result.str();
		}
	};

	struct ExternalFunction
	{
		const char* name;
		int parametersCount;
	};

	struct Value
	{
		const char* name;
		const char* type;
		const char* value;
		IT::IdDatatype originalType;
	};

	struct AssemblyProgram
	{
		std::vector<ExternalFunction> externalFunctions = {
			{ "GetProcessHeap", 1 },
			{ "HeapAlloc", 3 },
			{ "HeapFree", 3 },
			{ "GetStdHandle", 1 },
			{ "SetConsoleTitleA", 1 },
			{ "WriteConsoleA", 5 },
			{ "ExitProcess", 1 },
			{ "toString", 1 },
			{ "toInteger", 1 },
			{ "inputInteger", 0 },
			{ "inputString", 0 },
			{ "shiftRight", 2 },
			{ "shiftLeft", 2 },
			{ "stringEquals", 2 },
			{ "stringConcat", 2 },
			{ "print", 1 }
		};

		uint32_t stackSize = 4096;

		std::vector<Value> constants = {
			{ "ConsoleTitle", "db", "\"TNP2021 program\", 0", IT::IdDatatype::string },
			{ "EmptyStr", "db", "0", IT::IdDatatype::string }
		};

		std::vector<Value> staticVariables = {

		};

		std::vector<CodeBlock*> procedures;

		std::string toString()
		{
			std::stringstream s;
			s << ".586\n";
			s << ".model flat, stdcall\n\n";
			s << "includelib kernel32.lib\n";
			s << "includelib TNPSTL.lib\n";
			s << "includelib msvcrt.lib\n";
			s << "includelib Iphlpapi.lib\n\n";
			for (unsigned int i = 0; i < externalFunctions.size(); i++)
			{
				s << externalFunctions[i].name << " proto ";
				for (int j = 0; j < externalFunctions[i].parametersCount; j++)
				{
					s << ":dword" << (j + 1 < externalFunctions[i].parametersCount ? "," : "");
				}
				s << '\n';
			}
			s << ".stack " << stackSize << "\n\n";

			s << ".const\n\n";
			for (unsigned int i = 0; i < constants.size(); i++)
			{
				s << constants[i].name << " " << constants[i].type << " " << constants[i].value << '\n';
			}

			s << "\n.data\n\n";
			for (unsigned int i = 0; i < staticVariables.size(); i++)
			{
				s << staticVariables[i].name << " " << staticVariables[i].type << " " << staticVariables[i].value << '\n';
			}

			s << "\n.code\n\n";
			for (unsigned int i = 0; i < procedures.size(); i++)
			{
				s << procedures[i]->toString() << "\n\n";
			}

			s << "END main";

			return s.str();
		}

		static AssemblyProgram* getInstance()
		{
			static AssemblyProgram* instance = nullptr;
			if (instance == 0)
			{
				instance = new AssemblyProgram;
			}
			return instance;
		}

		static char* cstrToAssemblyInitializer(const char* str)
		{
			int len = strlen(str);
			char* initializer = new char[len + 5];
			strcpy_s(initializer, len + 5, "\"");
			strncat_s(initializer, len + 5, str, len);
			strncat_s(initializer, len + 5, "\", 0", len);
			return initializer;
		}
	};

	struct RegisterOrImmediate32
	{
		virtual const char* getName() = 0;
	};

	struct Register : public RegisterOrImmediate32
	{
		const char* name;
		const char* f8bitName;
		const char* contents;
		uint32_t used;
		bool dontUse;

		const char* getName() override
		{
			return this->name;
		}

		Register(const char* name, const char* f8bitName, const char* contents, uint32_t used, bool dontUse)
			: name(name),
			f8bitName(f8bitName),
			contents(contents),
			used(used),
			dontUse(dontUse)
		{ }
	};

	struct Immediate32 : public RegisterOrImmediate32
	{
		uint32_t value;

		const char* getName() override
		{
			char* buffer = new char[16];
			_itoa_s(this->value, buffer, 16, 10);
			return buffer;
		}
	};

	struct Memory
	{
		// Стек состояний регистров.
		std::stack<Register*> registersStack;
		// Представление стека процессора. Пушить через push_front().
		std::list<const char*> stack;
		std::list<const char*> updatedValues;
		uint32_t pushedRegistersSize = 0;

		Memory()
		{
			Register* newRegisters = (Register*)calloc(6, sizeof(Register));
			if (newRegisters == nullptr)
			{
				throw "Ошибка выделения памяти.";
			}
			newRegisters[0] = Register("eax", "al", "", 0, false);
			newRegisters[1] = Register("ebx", "bl", "", 0, false);
			newRegisters[2] = Register("ecx", "cl", "", 0, false);
			newRegisters[3] = Register("edx", "dl", "", 0, false);
			newRegisters[4] = Register("esi", "sil", "", 0, false);
			newRegisters[5] = Register("edi", "dil", "", 0, false);
			registersStack.push(newRegisters);
		}

		// Удалить временные переменные и литералы
		void clearTemp()
		{
			Register* registers = registersStack.top();
			for (int i = 0; i < 6; i++)
			{
				if (strncmp(registers[i].contents, "#PRNTemp", 8) == 0 || strncmp(registers[i].contents, "literal", 7) == 0)
				{
					this->setRegisterContents(registers[i].name, "");
				}
			}
		}

		// Удалить временные переменные и литералы, которых нет в списке
		void clearTempNotPresent(std::list<const char*> needed)
		{
			Register* registers = registersStack.top();
			for (int i = 0; i < 6; i++)
			{
				if (strncmp(registers[i].contents, "#PRNTemp", 8) == 0 || strncmp(registers[i].contents, "literal", 7) == 0)
				{
					bool del = true;
					for (std::list<const char*>::iterator n = needed.begin(); n != needed.end(); n++)
					{
						if (strcmp(registers[i].contents, *n) == 0)
						{
							del = false;
						}
					}
					if (del)
					{
						setRegisterContents(registers[i].name, "");
					}
				}
			}
		}

		// Поместить регистры в стек.
		// Обнуляет значения полей contents. 
		void pushRegisters(CodeBlock& function)
		{
			function.AddCommand("pushad");
			Register* newRegisters = (Register*)calloc(6, sizeof(Register));
			if (newRegisters == nullptr)
			{
				throw "Ошибка выделения памяти.";
			}
			newRegisters[0] = Register("eax", "al", "", 0, false);
			newRegisters[1] = Register("ebx", "bl", "", 0, false);
			newRegisters[2] = Register("ecx", "cl", "", 0, false);
			newRegisters[3] = Register("edx", "dl", "", 0, false);
			newRegisters[4] = Register("esi", "sil", "", 0, false);
			newRegisters[5] = Register("edi", "dil", "", 0, false);
			registersStack.push(newRegisters);
			stack.push_front("#PushedRegisters");
		}

		// Восстанавливает значения регистров из стека. Восстанавливает значения полей contents и приоритеты использования.
		// Устанваливает esp в соответствующее значение.
		void popRegisters(CodeBlock& function)
		{
			this->clearLocalStack(function);
			function.AddCommand("popad");
			delete[] registersStack.top();
			registersStack.pop();
			stack.pop_front();
			Register* registers = registersStack.top();
			for (int i = 0; i < 6; i++)
			{
				if (strcmp(registersStack.top()[i].contents, "") != 0)
				{
					uint32_t location = this->isStoredInStack(registers[i].contents);
					if (location != UINT_MAX)
					{
						function.AddCommand("mov", registers[i].name, this->espOffset(location));
						continue;
					}

					for (unsigned int j = 0; j < function.parameters.size(); j++)
					{
						if (strcmp(IT::GetQualifiedIdentifierName(function.parameters[j]), registers[i].contents) == 0)
						{
							function.AddCommand("mov", registers[i].name, registers[i].contents);
							continue;
						}
					}

					AssemblyProgram* program = AssemblyProgram::getInstance();
					for (std::vector<Value>::iterator in = program->staticVariables.begin(); in != program->staticVariables.end(); in++)
					{
						if (strcmp(in->name, registers[i].contents) == 0)
						{
							if (in->originalType == IT::IdDatatype::string)
							{
								function.AddCommand("mov", registers[i].name, Memory::offset(in->name));
								continue;
							}
							else if (in->originalType == IT::IdDatatype::integer)
							{
								function.AddCommand("mov", registers[i].name, in->name);
								continue;
							}
						}
					}
				}
			}
		}

		// Проверить, хранится ли значение в каком-либо из регистров.
		// Не принимает во внимание флаг dontUse.
		Register* isStoredInRegisters(const char* fqin)
		{
			Register* registers = registersStack.top();
			for (int i = 0; i < 6; i++)
			{
				if (strcmp(registers[i].contents, fqin) == 0)
				{
					return &registers[i];
				}
			}
			return nullptr;
		}

		// Проверить, хранится ли переменная в стеке. 
		// Возвращает смещение относительно ESP в байтах, если хранится, и UINT_MAX, если не хранится. 
		uint32_t isStoredInStack(const char* fqin)
		{
			uint32_t offset = 0;
			if (stack.size() == 0)
			{
				return UINT_MAX;
			}
			for (std::list<const char*>::iterator i = stack.begin(); i != stack.end(); i++)
			{
				if (strcmp(*i, "#PushedRegisters") == 0)
				{
					offset += 7;
				}
				else if (strcmp(*i, fqin) == 0)
				{
					return offset * 4;
				}
				offset++;
			}
			return UINT_MAX;
		}

		// Получить размер использованного программой стека в байтах.
		uint32_t getStackSize()
		{
			uint32_t size = 0;
			for (std::list<const char*>::iterator i = stack.begin(); i != stack.end(); i++)
			{
				if (strcmp(stack.front(), "#PushedRegisters") == 0)
				{
					size += 32;
				}
				else
				{
					size += 4;
				}
			}
			return size;
		}

		// Получить размер использованного лоокальными переменными стека в байтах.
		uint32_t getLocalStackSize()
		{
			uint32_t offset = 0;
			for (std::list<const char*>::iterator i = stack.begin(); i != stack.end(); i++)
			{
				if (strcmp(*i, "#PushedRegisters") == 0)
				{
					break;
				}
				offset++;
			}
			return offset * 4;
		}

		// Удалить локальные переменные из стека, сдвинув esp.
		void clearLocalStack(CodeBlock& function)
		{
			uint32_t offset = 0;
			for (std::list<const char*>::iterator i = stack.begin(); i != stack.end(); i++)
			{
				if (strcmp(*i, "#PushedRegisters") == 0)
				{
					break;
				}
				offset++;
			}
			if (this->getLocalStackSize() > 0)
			{
				function.AddCommand("add", "esp", nextLabel("", offset * 4, ""));
				while (offset != 0)
				{
					stack.pop_front();
					offset--;
				}
			}
		}

		// Удалить локальные переменные функции из стека, сдвинув esp.
		void clearStack(CodeBlock& function)
		{
			if (this->getStackSize() > 0)
			{
				function.AddCommand("add", "esp", nextLabel("", this->getStackSize(), ""));
			}
		}

		// Получить любой регистр, в котором сейчас ничего не хранится.
		// Принимает во внимание флаг dontUse.
		Register* freeRegister()
		{
			Register* registers = registersStack.top();
			for (int i = 0; i < 6; i++)
			{
				if (strcmp(registers[i].contents, "") == 0 && !registers[i].dontUse)
				{
					return &registers[i];
				}
			}
			return nullptr;
		}

		// Получить объект регистра по имени.
		Register* getRegisterByName(const char* name)
		{
			Register* registers = registersStack.top();
			for (int i = 0; i < 6; i++)
			{
				if (strcmp(registers[i].name, name) == 0)
				{
					return &registers[i];
				}
			}
			return nullptr;
		}

		// Получить свободный регистр. Если свободных нет, очистить тот, который использовался давнее всего.
		// Принимает во внимание флаг dontUse. Не обновляет значения полей contents.
		Register* freeRegister(CodeBlock& function)
		{
			Register* registers = registersStack.top();

			Register* is = this->freeRegister();
			if (is) return is;

			uint32_t minValue = 6, minIndex = 0;
			for (int i = 0; i < 6; i++)
			{
				if (registers[i].used < minValue && !registers[i].dontUse)
				{
					minValue = registers[i].used;
					minIndex = i;
				}
			}

			if (strcmp(registers[minIndex].contents, "") != 0)
			{
				this->unloadRegister(function, registers[minIndex].name);
			}

			this->setRegisterContents(registers[minIndex].name, "");
			return &registers[minIndex];
		}

		// Получить регистр с именем name. Если он занят, освободить, переместив данные из него в другой регистр.
		// Обновляет значения полей contents. Не обновляет приоритет использования.
		Register* freeRegister(CodeBlock& function, const char* name)
		{
			Register* from = this->getRegisterByName(name);
			if (strcmp(from->contents, "") == 0)
			{
				return from;
			}

			Register* to = this->freeRegister(function);
			if (to == from)
			{
				return to;
			}

			function.AddCommand("mov", to->name, from->name);
			this->setRegisterContents(to->name, from->contents);
			this->setRegisterContents(from->name, "");

			return from;
		}

		// Сохранить значение, хранящееся в регистре, в стек или в статическую переменную.
		// Обновляет значение поля contents. Не обновляет приоритет использования.
		void unloadRegister(CodeBlock& function, const char* name)
		{
			Register* save = this->getRegisterByName(name);
			if (strcmp(save->contents, "") == 0)
			{
				return;
			}

			uint32_t offset = isStoredInStack(save->contents);
			if (offset != UINT_MAX)
			{
				updatedValues.push_back(save->contents);
				function.AddCommand("mov", espOffset(offset), save->name);
				this->setRegisterContents(save->name, "");
				return;
			}

			for (unsigned int i = 0; i < function.parameters.size(); i++)
			{
				if (strcmp(IT::GetQualifiedIdentifierName(function.parameters[i]), save->contents) == 0)
				{
					function.AddCommand("mov", save->contents, save->name);
					this->setRegisterContents(save->name, "");
					return;
				}
			}

			AssemblyProgram* program = AssemblyProgram::getInstance();
			for (std::vector<Value>::iterator i = program->staticVariables.begin(); i != program->staticVariables.end(); i++)
			{
				if (strcmp(i->name, save->contents) == 0)
				{
					function.AddCommand("mov", i->name, save->name);
					this->setRegisterContents(save->name, "");
					return;
				}
			}

			for (std::vector<Value>::iterator i = program->constants.begin(); i != program->constants.end(); i++)
			{
				if (strcmp(i->name, save->contents) == 0)
				{
					return;
				}
			}

			/*function.AddCommand("push", save->name);
			stack.push_front(save->contents);*/
		}

		// Загрузить переменную в регистр.
		// Принимает во внимание флаг dontUse. Обновляет значение поля contents. Обновляет приоритет использования.
		Register* loadToRegister(CodeBlock& function, const char* fqin)
		{
			Register* op = this->isStoredInRegisters(fqin);
			if (op)
			{
				this->useRegister(op->name);
				return op;
			}

			op = this->freeRegister(function);
			uint32_t location = this->isStoredInStack(fqin);
			if (location != UINT_MAX)
			{
				function.AddCommand("mov", op->name, this->espOffset(location));
				this->useRegister(op->name);
				this->setRegisterContents(op->name, fqin);
				return op;
			}

			for (unsigned int i = 0; i < function.parameters.size(); i++)
			{
				if (strcmp(IT::GetQualifiedIdentifierName(function.parameters[i]), fqin) == 0)
				{
					function.AddCommand("mov", op->name, fqin);
					this->useRegister(op->name);
					this->setRegisterContents(op->name, fqin);
					return op;
				}
			}

			AssemblyProgram* program = AssemblyProgram::getInstance();
			for (std::vector<Value>::iterator i = program->staticVariables.begin(); i != program->staticVariables.end(); i++)
			{
				if (strcmp(i->name, fqin) == 0)
				{
					if (i->originalType == IT::IdDatatype::string)
					{
						function.AddCommand("mov", op->name, Memory::offset(i->name));
					}
					else if (i->originalType == IT::IdDatatype::integer)
					{
						function.AddCommand("mov", op->name, i->name);
					}
					this->useRegister(op->name);
					this->setRegisterContents(op->name, fqin);
					return op;
				}
			}
			for (std::vector<Value>::iterator i = program->constants.begin(); i != program->constants.end(); i++)
			{
				if (strcmp(i->name, fqin) == 0)
				{
					if (i->originalType == IT::IdDatatype::string)
					{
						function.AddCommand("mov", op->name, Memory::offset(i->name));
					}
					else if (i->originalType == IT::IdDatatype::integer)
					{
						function.AddCommand("mov", op->name, i->name);
					}
					this->useRegister(op->name);
					this->setRegisterContents(op->name, fqin);
					return op;
				}
			}

			return op;
		}

		// Загрузить переменную в регистр с именем name.
		// Не принимает во внимание флаг dontUse. Обновляет значение поля contents. Обновляет приоритет использования.
		Register* loadToRegister(CodeBlock& function, const char* fqin, const char* name)
		{
			Register* op = this->isStoredInRegisters(fqin);
			Register* target = this->getRegisterByName(name);
			if (op == target)
			{
				return op;
			}
			if (op && !op->dontUse)
			{
				if (op != target)
				{
					this->swapRegisterContents(function, op->name, target->name);
					this->useRegister(op->name);
				}
				return op;
			}

			target = this->freeRegister(function, name);

			uint32_t location = this->isStoredInStack(fqin);
			if (location != UINT_MAX)
			{
				function.AddCommand("mov", target->name, this->espOffset(location));
				this->useRegister(target->name);
				this->setRegisterContents(target->name, fqin);
				return target;
			}

			for (unsigned int i = 0; i < function.parameters.size(); i++)
			{
				if (strcmp(IT::GetQualifiedIdentifierName(function.parameters[i]), fqin) == 0)
				{
					function.AddCommand("mov", target->name, fqin);
					this->useRegister(target->name);
					this->setRegisterContents(target->name, fqin);
					return target;
				}
			}

			AssemblyProgram* program = AssemblyProgram::getInstance();
			for (std::vector<Value>::iterator i = program->staticVariables.begin(); i != program->staticVariables.end(); i++)
			{
				if (strcmp(i->name, fqin) == 0)
				{
					if (i->originalType == IT::IdDatatype::string)
					{
						function.AddCommand("mov", target->name, Memory::offset(i->name));
					}
					else if (i->originalType == IT::IdDatatype::integer)
					{
						function.AddCommand("mov", target->name, i->name);
					}
					this->useRegister(target->name);
					this->setRegisterContents(target->name, fqin);
					return target;
				}
			}
			for (std::vector<Value>::iterator i = program->constants.begin(); i != program->constants.end(); i++)
			{
				if (strcmp(i->name, fqin) == 0)
				{
					if (i->originalType == IT::IdDatatype::string)
					{
						function.AddCommand("mov", target->name, Memory::offset(i->name));
					}
					else if (i->originalType == IT::IdDatatype::integer)
					{
						function.AddCommand("mov", target->name, i->name);
					}
					this->useRegister(target->name);
					this->setRegisterContents(target->name, fqin);
					return target;
				}
			}

			return target;
		}

		// Обменять содержимое регистров.
		// Не принимает во внимание флаг dontUse. Обновляет значения полей contents. Обновляет приоритет использования.
		void swapRegisterContents(CodeBlock& function, const char* name1, const char* name2)
		{
			Register* reg1 = this->getRegisterByName(name1);
			Register* reg2 = this->getRegisterByName(name2);
			function.AddCommand("xchg", reg1->name, reg2->name);
			const char* temp = reg1->contents;
			this->setRegisterContents(reg1->name, reg2->contents);
			this->setRegisterContents(reg2->name, temp);
			this->useRegister(reg2->name);
			this->useRegister(reg1->name);
		}

		// Загрузить константу в регистр с именем name (если не указано, в любой).
		// Если указано name, то не принимает во внимание флаг dontUse, если не указано — принимает. Не обновляет значение 
		// поля contents. Обновляет приоритет использования.
		Register* loadConstantToRegister(CodeBlock& function, uint32_t value, const char* name = nullptr) 
		{
			Register* target;
			if (name == nullptr)
			{
				target = this->freeRegister(function);
			}
			else
			{
				target = this->freeRegister(function, name);
			}
			char buffer[16];
			_itoa_s(value, buffer, 10);
			function.AddCommand("mov", target->name, buffer);
			this->useRegister(target->name);
			return target;
		}

		// Обновить значение поля contents у регистра с именем name.
		void setRegisterContents(const char* name, const char* contents)
		{
			Register* registers = registersStack.top();

			Register* target = this->getRegisterByName(name);

			target->contents = contents;
		}

		// Обновить приоритет использования у регистра с именем name.
		void useRegister(const char* name)
		{
			Register* registers = registersStack.top();

			int target = -1;
			for (int i = 0; i < 6; i++)
			{
				if (strcmp(name, registers[i].name) == 0)
				{
					target = i;
					break;
				}
			}

			if (target == -1) return;

			for (int i = 0; i < 6; i++)
			{
				if (registers[i].used > registers[target].used)
				{
					registers[i].used--;
				}
			}

			registers[target].used = 6;
		}

		static const char* espOffset(uint32_t offset)
		{
			char* off = new char[20];
			strcpy_s(off, 20, "[esp");
			if (offset > 0)
			{
				strncat_s(off, 20, "+", 2);
				strncat_s(off, 20, std::to_string(offset).c_str(), 20);
			}
			strncat_s(off, 20, "]", 2);
			return off;
		}

		static const char* offset(const char* name)
		{
			int len = strlen(name) + 8;
			char* off = new char[len];
			strcpy_s(off, len, "offset ");
			strncat_s(off, len, name, len);
			return off;
		}
	};

	struct MetIdentifier
	{
		IT::Entry* itEntry = nullptr;
		bool pushed = false;
		bool popped = false;
	};

	IT::Entry* PRNTemp(IT::IdDatatype datatype, int number)
	{
		IT::Entry* PolishReverseNotationTemp = new IT::Entry;
		strcpy_s(PolishReverseNotationTemp->name, nextLabel("#PRNTemp",number,""));
		PolishReverseNotationTemp->iddatatype = IT::IdDatatype::integer;
		PolishReverseNotationTemp->idtype = IT::IdType::reserved;
		return PolishReverseNotationTemp;
	}

	std::list<const char*> identifiers(std::list<IT::Entry*> ids)
	{
		std::list<const char*> idents;
		for (std::list<IT::Entry*>::iterator i = ids.begin(); i != ids.end(); i++)
		{
			idents.push_back(IT::GetQualifiedIdentifierName(**i));
		}
		return idents;
	}

	void ExpressionCodeGenerate(CodeBlock*& function, LT::LexTable& lexemeTable, int& index, IT::IdTable& identifierTable, Memory* memory)
	{
		std::list<IT::Entry*> metIdentifiers;
		std::list<IT::Entry*> procParameters;
		while (lexemeTable.table[index].lexemeType != LEX_SEMICOLON && lexemeTable.table[index].lexemeType != LEX_RIGHT_BRACKET)
		{
			if (lexemeTable.table[index].lexemeType == LEX_IDENTIFIER)
			{
				if (identifierTable.table[lexemeTable.table[index].identifierTableIndex].idtype != IT::IdType::function)
				{
					metIdentifiers.push_back(&identifierTable.table[lexemeTable.table[index].identifierTableIndex]);

					if (lexemeTable.table[index + 1].lexemeType == LEX_SEMICOLON || lexemeTable.table[index + 1].lexemeType == LEX_RIGHT_BRACKET)
					{
						Register* op = memory->loadToRegister(*function, IT::GetQualifiedIdentifierName(*metIdentifiers.back()));
						memory->setRegisterContents(op->name, nextLabel("#PRNTemp", PRNCounter++, ""));
					}
				}
				else if (identifierTable.table[lexemeTable.table[index].identifierTableIndex].iddatatype != IT::IdDatatype::procedure)
				{
					const char* fqin = IT::GetQualifiedIdentifierName(identifierTable.table[lexemeTable.table[index].identifierTableIndex]);
					Register* eax = memory->getRegisterByName("eax");
					memory->useRegister(eax->name);
					if (identifierTable.table[lexemeTable.table[index].identifierTableIndex].external)
					{
						Register* registers = memory->registersStack.top();
						registers[0].dontUse = registers[2].dontUse = registers[3].dontUse = true;

						memory->clearTempNotPresent(identifiers(metIdentifiers));

						memory->freeRegister(*function, "eax");
						memory->freeRegister(*function, "ecx");
						memory->freeRegister(*function, "edx");

						registers[0].dontUse = registers[2].dontUse = registers[3].dontUse = false;
					}
					else
					{
						memory->clearTempNotPresent(identifiers(metIdentifiers));
						memory->freeRegister(*function, "eax");
					}

					for (int i = 0; i < identifierTable.table[lexemeTable.table[index].identifierTableIndex].parametersCount; i++)
					{
						memory->stack.pop_front();
					}

					function->AddCommand("call", fqin);

					memory->setRegisterContents(eax->name, nextLabel("#PRNTemp", PRNCounter, ""));

					metIdentifiers.push_back(PRNTemp(identifierTable.table[lexemeTable.table[index].identifierTableIndex].iddatatype, PRNCounter++));
				}
				else
				{
					const char* fqin = IT::GetQualifiedIdentifierName(identifierTable.table[lexemeTable.table[index].identifierTableIndex]);
					
					function->AddCommand("call", fqin);

					uint32_t size = procParameters.size(), sizeTemp;
					while (size != 0)
					{
						memory->stack.pop_front();
						size--;
					}
					while (!procParameters.empty())
					{
						const char* fqin1 = IT::GetQualifiedIdentifierName(*procParameters.front());
						memory->stack.push_front(fqin1);
						procParameters.pop_front();
						size++;
					}

					sizeTemp = size;

					const char* save = memory->stack.front();

					while (size > 0)
					{
						Register* f = memory->isStoredInRegisters(save);
						if (f != nullptr)
						{
							memory->setRegisterContents(f->name, "");
						}
						Register* temp = memory->loadToRegister(*function, save);
						for (unsigned int i = 0; i < sizeTemp - size + 1; i++)
						{
							memory->stack.pop_front();
						}
						save = memory->stack.front();
						for (unsigned int i = 0; i < sizeTemp - size + 1; i++)
						{
							memory->stack.push_front("#empty");
						}
						memory->unloadRegister(*function, temp->name);

						size--;
					}
					function->AddCommand("add", "esp", nextLabel("", sizeTemp * 4, ""));
					
					for (unsigned int i = 0; i < sizeTemp; i++)
					{
						memory->stack.pop_front();
					}
				}

			}
			else if (lexemeTable.table[index].lexemeType == LEX_LITERAL)
			{
				const char* fqin = IT::GetQualifiedIdentifierName(identifierTable.table[lexemeTable.table[index].identifierTableIndex]);
				metIdentifiers.push_back(&identifierTable.table[lexemeTable.table[index].identifierTableIndex]);

				if (lexemeTable.table[index + 1].lexemeType == LEX_SEMICOLON || lexemeTable.table[index + 1].lexemeType == LEX_RIGHT_BRACKET)
				{
					Register* op;
					if (metIdentifiers.back()->iddatatype == IT::IdDatatype::integer)
					{
						op = memory->loadConstantToRegister(*function, metIdentifiers.back()->value);
					}
					else
					{
						op = memory->loadToRegister(*function, IT::GetQualifiedIdentifierName(*metIdentifiers.back()));
					}
					memory->setRegisterContents(op->name, nextLabel("#PRNTemp", PRNCounter++, ""));
				}
			}
			else if (lexemeTable.table[index].lexemeType == LEX_OPERATOR)
			{
				Register* operand2 = nullptr;
				IT::IdDatatype operand2type;
				if (metIdentifiers.back()->idtype != IT::IdType::literal_ || metIdentifiers.back()->iddatatype != IT::IdDatatype::integer)
				{
					const char* fqin2 = IT::GetQualifiedIdentifierName(*metIdentifiers.back());
					operand2type = metIdentifiers.back()->iddatatype;
					if (strcmp(lexemeTable.table[index].originalLexeme, "/") == 0 
						|| strcmp(lexemeTable.table[index].originalLexeme, "%") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, "*") == 0)
					{
						Register* eax = memory->getRegisterByName("eax");
						Register* edx = memory->getRegisterByName("edx");
						eax->dontUse = edx->dontUse = true;

						operand2 = memory->loadToRegister(*function, fqin2);
						
						eax->dontUse = edx->dontUse = false;
					}
					else if (strcmp(lexemeTable.table[index].originalLexeme, "<<") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, ">>") == 0)
					{
						operand2 = memory->loadToRegister(*function, fqin2, "ecx");
					}
					else
					{
						operand2 = memory->loadToRegister(*function, fqin2);
					}
				}
				else
				{
					operand2type = metIdentifiers.back()->iddatatype;
					if (strcmp(lexemeTable.table[index].originalLexeme, "/") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, "%") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, "*") == 0)
					{
						Register* eax = memory->getRegisterByName("eax");
						Register* edx = memory->getRegisterByName("edx");
						eax->dontUse = edx->dontUse = true;

						operand2 = memory->loadConstantToRegister(*function, metIdentifiers.back()->value);
						memory->setRegisterContents(operand2->name, metIdentifiers.back()->name);

						eax->dontUse = edx->dontUse = false;
					}
					else if (strcmp(lexemeTable.table[index].originalLexeme, "<<") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, ">>") == 0)
					{
						operand2 = memory->loadConstantToRegister(*function, metIdentifiers.back()->value, "ecx");
						memory->setRegisterContents(operand2->name, metIdentifiers.back()->name);
					}
					else
					{
						operand2 = memory->loadConstantToRegister(*function, metIdentifiers.back()->value);
						memory->setRegisterContents(operand2->name, metIdentifiers.back()->name);
					}
				}
				operand2->dontUse = true;
				metIdentifiers.pop_back();

				Register* operand1 = nullptr;
				IT::IdDatatype operand1type;
				if (metIdentifiers.back()->idtype != IT::IdType::literal_ || metIdentifiers.back()->iddatatype != IT::IdDatatype::integer)
				{
					const char* fqin1 = IT::GetQualifiedIdentifierName(*metIdentifiers.back());
					operand1type = metIdentifiers.back()->iddatatype;
					if (strcmp(lexemeTable.table[index].originalLexeme, "/") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, "%") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, "*") == 0)
					{
						Register* eax = memory->getRegisterByName("eax");
						Register* edx = memory->getRegisterByName("edx");
						eax->dontUse = edx->dontUse = operand2->dontUse = true;

						operand1 = memory->loadToRegister(*function, fqin1, "eax");
						memory->freeRegister(*function, "edx");
						function->AddCommand("cdq");

						eax->dontUse = edx->dontUse = operand2->dontUse = false;
					}
					else if (strcmp(lexemeTable.table[index].originalLexeme, "<<") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, ">>") == 0)
					{
						Register* ecx = memory->getRegisterByName("ecx");
						ecx->dontUse = true;
						operand1 = memory->loadToRegister(*function, fqin1);
						ecx->dontUse = false;
					}
					else
					{
						operand1 = memory->loadToRegister(*function, fqin1);
					}
				}
				else
				{
					operand1type = metIdentifiers.back()->iddatatype;
					if (strcmp(lexemeTable.table[index].originalLexeme, "/") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, "%") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, "*") == 0)
					{
						Register* eax = memory->getRegisterByName("eax");
						Register* edx = memory->getRegisterByName("edx");
						eax->dontUse = edx->dontUse = operand2->dontUse = true;

						operand1 = memory->loadConstantToRegister(*function, metIdentifiers.back()->value, "eax");
						memory->setRegisterContents(operand1->name, metIdentifiers.back()->name);
						memory->freeRegister(*function, "edx");
						function->AddCommand("cdq");

						eax->dontUse = edx->dontUse = operand2->dontUse = false;
					}
					else if (strcmp(lexemeTable.table[index].originalLexeme, "<<") == 0
						|| strcmp(lexemeTable.table[index].originalLexeme, ">>") == 0)
					{
						Register* ecx = memory->getRegisterByName("ecx");
						ecx->dontUse = true;
						operand1 = memory->loadConstantToRegister(*function, metIdentifiers.back()->value);
						memory->setRegisterContents(operand1->name, metIdentifiers.back()->name);
						ecx->dontUse = false;
					}
					else
					{
						operand1 = memory->loadConstantToRegister(*function, metIdentifiers.back()->value);
						memory->setRegisterContents(operand1->name, metIdentifiers.back()->name);
					}
				}
				operand1->dontUse = true;
				metIdentifiers.pop_back();

				if (strcmp(lexemeTable.table[index].originalLexeme, "+") == 0)
				{
					if (operand1type == IT::IdDatatype::integer && operand2type == IT::IdDatatype::integer)
					{
						function->AddCommand("add", operand1->name, operand2->name);
						memory->useRegister(operand1->name);
						memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));

						metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
					}
					else if (operand1type == IT::IdDatatype::string && operand2type == IT::IdDatatype::string)
					{
						function->AddCommand("push", operand2->name);
						function->AddCommand("push", operand1->name);
						function->AddCommand("call", "stringConcat");
						memory->setRegisterContents("eax", nextLabel("#PRNTemp", PRNCounter, ""));

						metIdentifiers.push_back(PRNTemp(IT::IdDatatype::string, PRNCounter++));
					}
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, "-") == 0)
				{
					function->AddCommand("sub", operand1->name, operand2->name);
					memory->useRegister(operand1->name);
					memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, "/") == 0)
				{
					function->AddCommand("idiv", operand2->name);
					memory->useRegister("eax");
					memory->setRegisterContents("eax", nextLabel("#PRNTemp", PRNCounter, ""));
					memory->setRegisterContents("edx", "");

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, "%") == 0)
				{
					function->AddCommand("idiv", operand2->name);
					memory->useRegister("edx");
					memory->setRegisterContents("edx", nextLabel("#PRNTemp", PRNCounter, ""));
					memory->setRegisterContents("eax", "");

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, "*") == 0)
				{
					function->AddCommand("imul", operand2->name);
					memory->useRegister("eax");
					memory->setRegisterContents("eax", nextLabel("#PRNTemp", PRNCounter, ""));
					memory->setRegisterContents("edx", "");

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, "==") == 0)
				{
					if (operand1type == IT::IdDatatype::integer && operand2type == IT::IdDatatype::integer)
					{
						static int equalsCounter = 0;
						function->AddCommand("cmp", operand1->name, operand2->name);
						memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));
						char* cmpNotEquals = nextLabel("eq", equalsCounter, "ne");
						char* cmpEnd = nextLabel("eq", equalsCounter++, "end");
						function->AddCommand("jne", cmpNotEquals);
						function->AddCommand("mov", operand1->name, "1");
						function->AddCommand("jmp", cmpEnd);
						function->AddLabel(cmpNotEquals);
						function->AddCommand("xor", operand1->name, operand1->name);
						function->AddLabel(cmpEnd);

						memory->useRegister(operand1->name);
						memory->useRegister(operand2->name);

						metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
					}
					else if (operand1type == IT::IdDatatype::string && operand2type == IT::IdDatatype::string)
					{
						function->AddCommand("push", operand1->name);
						memory->setRegisterContents(operand1->name, "");
						function->AddCommand("push", operand2->name);
						memory->setRegisterContents(operand2->name, "");
						memory->freeRegister(*function, "eax");
						function->AddCommand("call", "stringEquals");

						memory->useRegister("eax");
						memory->setRegisterContents("eax", nextLabel("#PRNTemp", PRNCounter, ""));

						metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
					}
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, "!=") == 0)
				{
					if (operand1type == IT::IdDatatype::integer && operand2type == IT::IdDatatype::integer)
					{
						static int notEqualsCounter = 0;
						function->AddCommand("cmp", operand1->name, operand2->name);
						memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));
						char* cmpEquals = nextLabel("neq", notEqualsCounter, "e");
						char* cmpEnd = nextLabel("neq", notEqualsCounter++, "end");
						function->AddCommand("je", cmpEquals);
						function->AddCommand("mov", operand1->name, "1");
						function->AddCommand("jmp", cmpEnd);
						function->AddLabel(cmpEquals);
						function->AddCommand("xor", operand1->name, operand1->name);
						function->AddLabel(cmpEnd);

						memory->useRegister(operand1->name);
						memory->useRegister(operand2->name);

						metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
					}
					else if (operand1type == IT::IdDatatype::string && operand2type == IT::IdDatatype::string)
					{
						function->AddCommand("push", operand1->name);
						memory->setRegisterContents(operand1->name, "");
						function->AddCommand("push", operand2->name);
						memory->setRegisterContents(operand2->name, "");
						memory->freeRegister(*function, "eax");
						function->AddCommand("call", "stringEquals");
						function->AddCommand("xor", "eax", "1");

						memory->useRegister("eax");
						memory->setRegisterContents("eax", nextLabel("#PRNTemp", PRNCounter, ""));

						metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
					}
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, "<") == 0)
				{
					static int lessThanCounter = 0;
					function->AddCommand("cmp", operand1->name, operand2->name);
					char* cmpGreaterOrEquals = nextLabel("less", lessThanCounter, "ge");
					function->AddCommand("jge", cmpGreaterOrEquals);
					function->AddCommand("mov", operand1->name, "1");
					char* cmpEnd = nextLabel("less", lessThanCounter, "end");
					function->AddCommand("jmp", cmpEnd);
					function->AddLabel(cmpGreaterOrEquals);
					function->AddCommand("xor", operand1->name, operand1->name);
					function->AddLabel(cmpEnd);

					lessThanCounter++;
					
					memory->useRegister(operand1->name);
					memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, "<=") == 0)
				{
					static int lessOrEqualsCounter = 0;
					function->AddCommand("cmp", operand1->name, operand2->name);
					char* cmpGreater = nextLabel("lessOrEquals", lessOrEqualsCounter, "g");
					function->AddCommand("jg", cmpGreater);
					function->AddCommand("mov", operand1->name, "1");
					char* cmpEnd = nextLabel("lessOrEquals", lessOrEqualsCounter++, "end");
					function->AddCommand("jmp", cmpEnd);
					function->AddLabel(cmpGreater);
					function->AddCommand("xor", operand1->name, operand1->name);
					function->AddLabel(cmpEnd);

					lessOrEqualsCounter++;

					memory->useRegister(operand1->name);
					memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, ">") == 0)
				{
					static int moreThanCounter = 0;
					function->AddCommand("cmp", operand1->name, operand2->name);
					char* cmpLessOrEquals = nextLabel("more", moreThanCounter, "le");
					function->AddCommand("jle", cmpLessOrEquals);
					function->AddCommand("mov", operand1->name, "1");
					char* cmpEnd = nextLabel("more", moreThanCounter, "end");
					function->AddCommand("jmp", cmpEnd);
					function->AddLabel(cmpLessOrEquals);
					function->AddCommand("xor", operand1->name, operand1->name);
					function->AddLabel(cmpEnd);

					moreThanCounter++;

					memory->useRegister(operand1->name);
					memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, ">=") == 0)
				{
					static int moreOrEqualsCounter = 0;
					function->AddCommand("cmp", operand1->name, operand2->name);
					char* cmpLess = nextLabel("moreOrEquals", moreOrEqualsCounter, "l");
					function->AddCommand("jl", cmpLess);
					function->AddCommand("mov", operand1->name, "1");
					char* cmpEnd = nextLabel("moreOrEquals", moreOrEqualsCounter, "end");
					function->AddCommand("jmp", cmpEnd);
					function->AddLabel(cmpLess);
					function->AddCommand("xor", operand1->name, operand1->name);
					function->AddLabel(cmpEnd);

					moreOrEqualsCounter++;

					memory->useRegister(operand1->name);
					memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, "<<") == 0)
				{
					function->AddCommand("shl", operand1->name, operand2->f8bitName);
					memory->useRegister(operand1->name);
					memory->useRegister(operand2->name);
					memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				else if (strcmp(lexemeTable.table[index].originalLexeme, ">>") == 0)
				{
					function->AddCommand("sar", operand1->name, operand2->f8bitName);
					memory->useRegister(operand1->name);
					memory->useRegister(operand2->name);
					memory->setRegisterContents(operand1->name, nextLabel("#PRNTemp", PRNCounter, ""));

					metIdentifiers.push_back(PRNTemp(IT::IdDatatype::integer, PRNCounter++));
				}
				operand1->dontUse = operand2->dontUse = false;
			}
			else if (lexemeTable.table[index].lexemeType == LEX_PRINT)
			{
				Register* operand = nullptr;
				if (metIdentifiers.back()->idtype != IT::IdType::literal_ || metIdentifiers.back()->iddatatype != IT::IdDatatype::integer)
				{
					const char* fqin = IT::GetQualifiedIdentifierName(*metIdentifiers.back());
					operand = memory->loadToRegister(*function, fqin);
				}
				else
				{
					operand = memory->loadConstantToRegister(*function, metIdentifiers.back()->value);
				}
				metIdentifiers.pop_back();
				function->AddCommand("pushad");
				function->AddCommand("push", operand->name);
				function->AddCommand("call", "print");
				function->AddCommand("popad");
			}
			else if (lexemeTable.table[index].lexemeType == LEX_PUSH_MARK)
			{
				if (!metIdentifiers.empty())
				{
					if (metIdentifiers.back()->idtype != IT::IdType::literal_ || metIdentifiers.back()->iddatatype != IT::IdDatatype::integer)
					{
						Register* operand = nullptr;
						const char* fqin = IT::GetQualifiedIdentifierName(*metIdentifiers.back());
						operand = memory->loadToRegister(*function, fqin);
						procParameters.push_back(metIdentifiers.back());
						metIdentifiers.pop_back();
						function->AddCommand("push", operand->name);
					}
					else
					{
						function->AddCommand("push", nextLabel("", metIdentifiers.back()->value, ""));
						metIdentifiers.pop_back();
					}
					memory->stack.push_front("#functionParameter");
				}
			}
			else if (lexemeTable.table[index].lexemeType == LEX_EQUALS)
			{
				const char* fqin1 = IT::GetQualifiedIdentifierName(*metIdentifiers.back());
				Register* value = memory->loadToRegister(*function, fqin1);
				metIdentifiers.pop_back();
				const char* fqin2 = IT::GetQualifiedIdentifierName(*metIdentifiers.back());
				Register* storage = memory->loadToRegister(*function, fqin2);
				function->AddCommand("mov", storage->name, value->name);
				memory->unloadRegister(*function, storage->name);
			}

			if (lexemeTable.table[index + 1].lexemeType != LEX_RIGHT_BRACKET)
			{
				memory->clearTempNotPresent(identifiers(metIdentifiers));
			}
			
			index++;
		}
		if (lexemeTable.table[index].lexemeType == LEX_SEMICOLON)
		{
			memory->clearTemp();
		}
	}

	struct ProgramBlock
	{
		const char* label;
		bool whileBlock;
	};

	void FunctionCodeGenerate(CodeBlock*& function, LT::LexTable& lexemeTable, int& index, IT::IdTable& identifierTable)
	{
		Memory* memory = new Memory;

		int nestedLevel = 0;
		int line = -1;

		std::stack<ProgramBlock> blocks;
		while (true)
		{
			if (line != lexemeTable.table[index].stringNumber)
			{
				line = lexemeTable.table[index].stringNumber;
				function->AddComment(LT::GetLine(lexemeTable, index));
			}

			if (lexemeTable.table[index].lexemeType == LEX_LEFT_CURLY_BRACKET)
			{
				nestedLevel++;

				if (nestedLevel != 1)
				{
					memory->pushRegisters(*function);
				}
			}
			else if (lexemeTable.table[index].lexemeType == LEX_RIGHT_CURLY_BRACKET)
			{
				memory->clearLocalStack(*function);

				nestedLevel--;
				if (nestedLevel == 0)
				{
					if (function->procedure)
					{
						function->AddCommand("ret", "0");
					}
					break;
				}
				
				memory->popRegisters(*function);
				if (!blocks.empty())
				{
					if (blocks.top().whileBlock)
					{
						function->AddCommand("jmp", blocks.top().label);
					}
					else
					{
						if (lexemeTable.table[index + 1].lexemeType == LEX_ELSEIF || lexemeTable.table[index + 1].lexemeType == LEX_ELSE)
						{
							const char* blockLabel = blocks.top().label;
							blocks.pop();
							function->AddCommand("jmp", blocks.top().label);
							function->AddLabel(blockLabel);
						}
						else
						{
							function->AddLabel(blocks.top().label);
							blocks.pop();
							function->AddLabel(blocks.top().label);
							blocks.pop();
						}
					}
				}
			}
			else if (lexemeTable.table[index].lexemeType == LEX_IF) 
			{
				extern int ifCounter;
				extern int ifBlockCounter;
				index += 2;
				ExpressionCodeGenerate(function, lexemeTable, index, identifierTable, memory);
				Register* condition = memory->isStoredInRegisters(nextLabel("#PRNTemp", PRNCounter - 1, ""));
				memory->setRegisterContents(condition->name, "");
				function->AddCommand("cmp", condition->name, "0");
				char* blockLabel = nextLabel("ifBlock", ifCounter, "end");
				char* label = nextLabel("if", ifCounter, "end");
				function->AddCommand("je", label);
				blocks.push({ blockLabel, false });
				blocks.push({ label, false });
				ifCounter++;
				ifBlockCounter++;
			}
			else if (lexemeTable.table[index].lexemeType == LEX_ELSEIF)
			{
				extern int ifCounter;
				index += 2;
				ExpressionCodeGenerate(function, lexemeTable, index, identifierTable, memory);
				Register* condition = memory->isStoredInRegisters(nextLabel("#PRNTemp", PRNCounter - 1, ""));
				memory->setRegisterContents(condition->name, "");
				function->AddCommand("cmp", condition->name, "0");
				char* label = nextLabel("if", ifCounter++, "end");
				function->AddCommand("je", label);
				blocks.push({ label, false });
			}
			else if (lexemeTable.table[index].lexemeType == LEX_ELSE)
			{
				blocks.push({ nextLabel("if", ifCounter++, "end"), false });
			}
			else if (lexemeTable.table[index].lexemeType == LEX_CREATE)
			{
				Register* reg = memory->freeRegister(*function);
				const char* fqin = IT::GetQualifiedIdentifierName(identifierTable.table[lexemeTable.table[index + 2].identifierTableIndex]);
				function->AddCommand("xor", reg->name, reg->name);
				memory->setRegisterContents(reg->name, fqin);
				function->AddCommand("push", reg->name);
				memory->stack.push_front(fqin);
				index += 2;
			}
			else if (lexemeTable.table[index].lexemeType == LEX_RETURN)
			{
				index += 2;
				ExpressionCodeGenerate(function, lexemeTable, index, identifierTable, memory);
				memory->loadToRegister(*function, nextLabel("#PRNTemp", PRNCounter - 1, ""), "eax");
				memory->clearStack(*function);
				if (strcmp(function->name, "_@_main") != 0)
				{
					function->AddCommand("ret", nextLabel("", function->parameters.size() * 4, ""));
				}
				else
				{
					function->AddCommand("push", "eax");
					function->AddCommand("call", "ExitProcess");
				}
			}
			else
			{
				ExpressionCodeGenerate(function, lexemeTable, index, identifierTable, memory);
			}
			index++;
		}

		index++;
	}

	std::string GenerateCode(LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		int index = 0;
		LT::Entry* entry;

		CodeBlock* currentFunction;

		for (int i = 0; i < idTable.size; i++)
		{
			if (idTable.table[i].idtype == IT::IdType::literal_ && idTable.table[i].iddatatype == IT::IdDatatype::string)
			{
				AssemblyProgram::getInstance()->constants.push_back({ idTable.table[i].name, "db", cstrToAssemblyInitializer((const char*)idTable.table[i].value), idTable.table[i].iddatatype });
			}
			else if (idTable.table[i].idtype == IT::IdType::literal_ && idTable.table[i].iddatatype == IT::IdDatatype::integer)
			{
				AssemblyProgram::getInstance()->constants.push_back({ idTable.table[i].name, "dd", nextLabel("",idTable.table[i].value,""), idTable.table[i].iddatatype });
			}
		}

		while (index < lexTable.size)
		{
			entry = &lexTable.table[index];

			if (entry->lexemeType == LEX_CREATE)
			{
				if (lexTable.table[index + 2].lexemeType == LEX_FUNCTION)
				{
					currentFunction = new CodeBlock();
					currentFunction->procedure = idTable.table[lexTable.table[index + 3].identifierTableIndex].iddatatype == IT::IdDatatype::procedure;
					IT::Entry* idTableEntry = &idTable.table[lexTable.table[index + 3].identifierTableIndex];
					currentFunction->name = IT::GetQualifiedIdentifierName(*idTableEntry);
					for (int i = lexTable.table[index + 3].identifierTableIndex + 1; i < idTable.size && idTable.table[i].idtype == IT::IdType::functionParameter; i++)
					{
						currentFunction->AddParameter(idTable.table[i]);
					}

					index += 5 + idTableEntry->parametersCount * 3;
					FunctionCodeGenerate(currentFunction, lexTable, index, idTable);
					AssemblyProgram::getInstance()->procedures.push_back(currentFunction);
				}
				else
				{
					IT::Entry idEntry = idTable.table[lexTable.table[index + 2].identifierTableIndex];
					if (idEntry.iddatatype == IT::IdDatatype::integer)
					{
						char* buffer = new char[16];
						_itoa_s((unsigned int)idEntry.value, buffer, 16, 10);
						AssemblyProgram::getInstance()->staticVariables.push_back({ IT::GetQualifiedIdentifierName(idEntry), "dd", buffer, IT::IdDatatype::integer });
					}
					else if (idEntry.iddatatype == IT::IdDatatype::string)
					{
						AssemblyProgram::getInstance()->staticVariables.push_back({ IT::GetQualifiedIdentifierName(idEntry), "dd",  "offset EmptyStr", IT::IdDatatype::string });
					}
				}
			}

			index++;
		}

		return AssemblyProgram::getInstance()->toString();
	}
}