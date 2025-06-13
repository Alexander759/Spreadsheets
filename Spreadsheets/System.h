#pragma once
#include "Table.h"

class System {
public:
	System() = default;

	void run();

	static bool validateOpenTable(const List<MyString>& args);
	void newTable(const List<MyString>& args);
	void openTable(const List<MyString>& args);
	void startTable(Table& table);
};