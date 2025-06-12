#pragma once
#include "MyString.h"

class Command {
public:
	Command();
	Command(const MyString& name, bool (*validateCommand)(const List<MyString>& args),
		void (*commandFunction)(const List<MyString>& args));

	const MyString& getName() const;

	void run(const List<MyString>& args);

private:
	MyString name;
	bool (*validateCommand)(const List<MyString>& args);
	void (*commandFunction)(const List<MyString>& args);
};
