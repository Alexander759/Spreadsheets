#pragma once
#include "CellType.h"
#include "RawContentType.h"
#include "MyString.h"
#include "ArgumentType.h"
#include "Position.h"

class Parser {
public:
	static RawContentType getRawContentTypeFromInput(const MyString& str);
	static ArgumentType getArgumentType(const MyString& str);

	static MyString getExpressionName(const MyString& str);
	static List<MyString> getArgumentsFromExpression(const MyString& str);
	static List<Position> getPositionsInRange(const MyString& str);
};
