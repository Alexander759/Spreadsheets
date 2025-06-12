#include "System.h"
#include <iostream>

System::System() {

}

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
		else if (command == "open") {

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
