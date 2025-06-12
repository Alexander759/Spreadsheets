#include "Command.h"
#include <iostream>

Command::Command() {
	this->validateCommand = nullptr;
	this->commandFunction = nullptr;
}

Command::Command(const MyString& name, bool(*validateCommand)(const List<MyString>& args),
	void(*commandFunction)(const List<MyString>& args)) {
	this->name = name;
	this->validateCommand = validateCommand;
	this->commandFunction = commandFunction;
}

const MyString& Command::getName() const {
	return this->name;
}

void Command::run(const List<MyString>& args) {
	if (!validateCommand(args)) {
		std::cout << "Invalid arguments" << std::endl;
	}

	commandFunction(args);
}
