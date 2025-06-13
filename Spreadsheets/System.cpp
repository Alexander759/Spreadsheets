#include "System.h"
#include <iostream>

void System::run() {
	MyString command;

	while (command.toLower() != "exit") {
		std::cin >> command;

		List<MyString> args = command.split();
		if (args.getLength() == 0) {
			continue;
		}

		MyString commandName = args[0];
		args.removeAt(0);
		if (commandName == "new") {
			newTable(args);
		}
		else if (commandName == "open") {
			openTable(args);
		}
	}
}

bool System::validateOpenTable(const List<MyString>& args) {
	return args.getLength() == 1;
}

void System::newTable(const List<MyString>& args) {
	TableConfigure config(args[0]);

	if (config.hasError()) {
		std::cout << config.getErrorMessage() << std::endl;
		return;
	}

	Table table(config);
	startTable(table);
}

void System::openTable(const List<MyString>& args) {
	if (args.getLength() < 2) {
		std::cout << "Not enough arguments" << std::endl;
		return;
	}


	TableConfigure config(args[1]);
	if (config.hasError()) {
		std::cout << config.getErrorMessage() << std::endl;
		return;
	}
	Table table(config);

	std::ifstream stream(args[0].getCString(), std::ios::in | std::ios::binary);
	if (!stream.good() || !stream.is_open()) {
		std::cout << "Couldn't find file for table" << std::endl;
		return;
	}

	stream >> table;
	stream.close();

	startTable(table);
}

void System::startTable(Table& table) {

	MyString command;
	while (command != "close") {
		table.print();
		std::cin >> command;

		if (command.toLower() == "close") {
			break;
		}

		List<MyString> args = command.split();
		if (args.getLength() == 0) {
			continue;
		}

		if (args[0].toLower() == "save") {
			if (args.getLength() < 2) {
				std::cout << "Couldn't save to file" << std::endl;
				continue;
			}

			std::ofstream stream(args[1].getCString(), std::ios::out | std::ios::binary);
			if (!stream.good()) {
				std::cout << "Couldn't save to file" << std::endl;
				continue;
			}

			stream << table;
			stream.close();
			continue;
		}

		if (!Position::isPosition(args[0])) {
			std::cout << "Invalid input" << std::endl;
			continue;
		}

		Cell* currentCell = table.getAtPosition(Position::fromString(args[0]));
		if (currentCell == nullptr) {
			std::cout << "Cell not found" << std::endl;
			continue;
		}

		args.removeAt(0);

		if (args.getLength() == 0) {
			std::cout << "Invalid input" << std::endl;
			continue;
		}

		if (args[0] == "insert") {
			if (args.getLength() < 2) {
				std::cout << "Invalid input" << std::endl;
				continue;
			}

			currentCell->setRawContent(args[1]);
		}
		else if (args[0] == "delete") {
			currentCell->setCellDisplayAndType("", CellType::EmptyCell);
		}
		else {
			currentCell->setRawContent(args[0]);
		}
	}
}
