#pragma once
#include "CellType.h"
#include "MyString.h"
#include "ArgumentType.h"

class Parser {
public:
	static CellType getCellTypeFromInput(const MyString& str);
	static ArgumentType getArgumentType(const MyString& str);

	static MyString getExpressionName(const MyString& str);
	static List<MyString> getArgumentsFromExpression(const MyString& str);

private:

};
