#pragma once
#include "Table.h"
#include "Command.h"

class System {
public:
	System();

	void run();

	static bool validateOpenTable(const List<MyString>& args);
	void newTable(const List<MyString>& args);
	void openTable(const List<MyString>& args);
	void startTable(Table& table);

private:

	List<Command> commandsApp;
	List<Command> commandsTable;
};