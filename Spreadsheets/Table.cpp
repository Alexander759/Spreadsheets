#include "Table.h"
#include <iostream>

Table::Table(const TableConfigure& config) {
	this->autoFit = config.getAutoFit();
	this->clearConsoleAfterCommand = config.getClearConsoleAfterCommand();
	this->alignment = config.getAlignment();
	this->initialTableRows = config.getInitialTableRows() +	1;
	this->initialTableCols = config.getInitialTableCols() + 1;
	this->maxTableRows = config.getMaxTableRows() + 1;
	this->maxTableCols = config.getMaxTableCols() + 1;
	this->visibleCellSymbols = config.getVisibleCellSymbols();

	this->cells = new Cell*[this->maxTableRows];

	for (size_t i = 0; i < this->maxTableRows; i++) {
		this->cells[i] = new Cell[this->maxTableCols];

		for (size_t j = 0; j < this->maxTableCols; j++) {
			this->cells[i][j].setPosition(Position(i, j));
			this->cells[i][j].setTable(this);
		}
	}

	for (size_t i = 1; i < this->maxTableRows; i++) {
		MyString stringPosition = Position(i, 1).toString();
		stringPosition = stringPosition.subStr(0, stringPosition.indexOf('1') - 1);
		this->cells[i][0].setRawContent(MyString("\"") + stringPosition + "\"");
	}

	for (size_t i = 1; i < this->maxTableCols; i++) {
		this->cells[0][i].setRawContent(i);
	}

	this->currentTableRows = this->initialTableRows;
	this->currentTableCols = this->initialTableCols;
}

Table::Table(const Table& other) {
	copyFrom(other);
}

Table::Table(Table&& other) {
	copyStatic(other);
	this->cells = other.cells;
	for (size_t i = 0; i < this->maxTableRows; i++) {
		for (size_t j = 0; j < this->maxTableCols; j++) {
			this->cells[i][j].setTable(this);
		}
	}

	other.cells = nullptr;
	other.maxTableCols = 0;
	other.maxTableRows = 0;
}

Table& Table::operator=(const Table& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}

	return *this;
}

Table& Table::operator=(Table&& other) {
	if (this != &other) {
		free();
		copyStatic(other);
		this->cells = other.cells;
		for (size_t i = 0; i < this->maxTableRows; i++) {
			for (size_t j = 0; j < this->maxTableCols; j++) {
				this->cells[i][j].setTable(this);
			}
		}

		other.cells = nullptr;
		other.maxTableCols = 0;
		other.maxTableRows = 0;
	}

	return *this;
}

Table::~Table() {
	free();
}

void Table::setNewConfigure(const TableConfigure& config) {
	this->autoFit = config.getAutoFit();
	this->clearConsoleAfterCommand = config.getClearConsoleAfterCommand();
	this->alignment = config.getAlignment();
	this->initialTableRows = config.getInitialTableRows() + 1;
	this->initialTableCols = config.getInitialTableCols() + 1;
	this->maxTableRows = config.getMaxTableRows() + 1;
	this->maxTableCols = config.getMaxTableCols() + 1;
	this->visibleCellSymbols = config.getVisibleCellSymbols();

	this->currentTableRows = this->initialTableRows;
	this->currentTableCols = this->initialTableCols;
}

Cell* Table::getAtPosition(const Position& position) {
	if (position.getRow() >= maxTableRows || position.getCol() >= maxTableCols) {
		return nullptr;
	}

	return &this->cells[position.getRow()][position.getCol()];
}

const Cell* Table::getAtPosition(const Position& position) const {
	if (position.getRow() >= maxTableRows || position.getCol() >= maxTableCols) {
		return nullptr;
	}

	return &this->cells[position.getRow()][position.getCol()];
}

void Table::print() const {

	if (this->clearConsoleAfterCommand) {
		system("cls");
	}

	List<size_t> maxSymbols = this->maxNumberOfCharactersPerColumn();

	for (size_t i = 0; i < this->currentTableRows; i++) {
		
		printRowBorder(maxSymbols);
		printRow(maxSymbols, i);
	}
	
	printRowBorder(maxSymbols);
}

void Table::printRowBorder(const List<size_t>& maxSymbols) const {
	for (size_t j = 0; j < this->currentTableCols; j++) {
		std::cout << "|";

		for (size_t k = 0; k < maxSymbols[j]; k++) {
			std::cout << "-";
		}
	}
	std::cout << "|" << std::endl;
}

void Table::printRow(const List<size_t>& maxSymbols, int i) const {
	for (size_t j = 0; j < this->currentTableCols; j++) {
		std::cout << "|";

		const Cell* cellPtr = this->getAtPosition(Position(i, j));
		size_t currentLength = this->getAtPosition(Position(i, j))->getDisplayContent().getLength();

		if (maxSymbols[j] < currentLength) {
			std::cout << cellPtr->getDisplayContent().subStr(0, maxSymbols[j] - 1);
			continue;
		}

		size_t emptySpacesLength = maxSymbols[j] - currentLength;

		if (this->alignment == Alignment::Left) {
			std::cout << cellPtr->getDisplayContent();

			for (size_t i = 0; i < emptySpacesLength; i++) {
				std::cout << " ";
			}
		}
		else if (this->alignment == Alignment::Center) {
			for (size_t i = 0; i < emptySpacesLength / 2; i++) {
				std::cout << " ";
			}

			if (emptySpacesLength % 2 == 1) {
				std::cout << " ";
			}

			std::cout << cellPtr->getDisplayContent();

			for (size_t i = 0; i < emptySpacesLength / 2; i++) {
				std::cout << " ";
			}
		}
		else {
			for (size_t i = 0; i < emptySpacesLength; i++) {
				std::cout << " ";
			}

			std::cout << cellPtr->getDisplayContent();
		}
	}
	std::cout << "|" << std::endl;
}

List<size_t> Table::maxNumberOfCharactersPerColumn() const {
	List<size_t> result;

	for (size_t i = 0; i < this->currentTableCols; i++) {
		result.add(3);
		for (size_t j = 0; j < this->currentTableRows; j++) {
			size_t currentLength = this->getAtPosition(Position(j, i))->getDisplayContent().getLength();
			if (this->autoFit) {

				if (result[i] < currentLength) {
					result[i] = currentLength;
				}
			}
			else {

				if (this->visibleCellSymbols <= currentLength) {
					result[i] = this->visibleCellSymbols;
					continue;
				}

				if (result[i] < currentLength) {
					result[i] = currentLength;
				}
			}
		}
	}

	return result;

}

void Table::setCurrentRowsAndCols(const Position& pos) {
	if (pos.getRow() > maxTableRows || pos.getCol() > maxTableRows) {
		return;
	}

	if (this->currentTableRows < pos.getRow() + 1) {
		this->currentTableRows = pos.getRow() + 1;
	}

	if (this->currentTableCols < pos.getCol() + 1) {
		this->currentTableCols = pos.getCol() + 1;
	}
}


void Table::copyFrom(const Table& other) {
	copyStatic(other);

	this->cells = new Cell*[this->maxTableRows];

	for (size_t i = 0; i < this->maxTableRows; i++) {
		this->cells[i] = new Cell[this->maxTableCols];

		for (size_t j = 0; j < this->maxTableCols; j++) {
			this->cells[i][j] = other.cells[i][j];
			this->cells[i][j].setTable(this);
		}
	}
}

void Table::copyStatic(const Table& other) {
	this->autoFit = other.autoFit;
	this->clearConsoleAfterCommand = other.clearConsoleAfterCommand;
	this->alignment = other.alignment;
	this->initialTableRows = other.initialTableRows;
	this->initialTableCols = other.initialTableCols;
	this->currentTableRows = other.currentTableRows;
	this->currentTableCols = other.currentTableCols;
	this->maxTableRows = other.maxTableRows;
	this->maxTableCols = other.maxTableCols;
	this->visibleCellSymbols = other.visibleCellSymbols;
}

void Table::free() {
	if (this->cells == nullptr) {
		return;
	}

	for (size_t i = 0; i < this->maxTableRows; i++) {
		delete[] this->cells[i];
	}

	delete[] this->cells;

	this->cells = nullptr;
	this->maxTableCols = 0;
	this->maxTableRows = 0;
}

std::ofstream& operator<<(std::ofstream& stream, const Table& table) {
	if (!stream.good()) {
		return stream;
	}

	stream.write(reinterpret_cast<const char*>(&table.autoFit), sizeof(bool));
	stream.write(reinterpret_cast<const char*>(&table.clearConsoleAfterCommand), sizeof(bool));
	stream.write(reinterpret_cast<const char*>(&table.initialTableRows), sizeof(size_t));
	stream.write(reinterpret_cast<const char*>(&table.initialTableCols), sizeof(size_t));
	stream.write(reinterpret_cast<const char*>(&table.currentTableRows), sizeof(size_t));
	stream.write(reinterpret_cast<const char*>(&table.currentTableCols), sizeof(size_t));
	stream.write(reinterpret_cast<const char*>(&table.maxTableRows), sizeof(size_t));
	stream.write(reinterpret_cast<const char*>(&table.maxTableCols), sizeof(size_t));
	stream.write(reinterpret_cast<const char*>(&table.visibleCellSymbols), sizeof(size_t));
	stream.write(reinterpret_cast<const char*>(&table.alignment), sizeof(Alignment));

	for (size_t i = 0; i < table.maxTableRows; i++) {
		for (size_t j = 0; j < table.maxTableCols; j++) {
			stream << table.cells[i][j];
		}
	}

	return stream;
}

std::ifstream& operator>>(std::ifstream& stream, Table& table) {
	if (!stream.good()) {
		return stream;
	}

	table.free();

	stream.read(reinterpret_cast<char*>(&table.autoFit), sizeof(bool));
	stream.read(reinterpret_cast<char*>(&table.clearConsoleAfterCommand), sizeof(bool));
	stream.read(reinterpret_cast<char*>(&table.initialTableRows), sizeof(size_t));
	stream.read(reinterpret_cast<char*>(&table.initialTableCols), sizeof(size_t));
	stream.read(reinterpret_cast<char*>(&table.currentTableRows), sizeof(size_t));
	stream.read(reinterpret_cast<char*>(&table.currentTableCols), sizeof(size_t));
	stream.read(reinterpret_cast<char*>(&table.maxTableRows), sizeof(size_t));
	stream.read(reinterpret_cast<char*>(&table.maxTableCols), sizeof(size_t));
	stream.read(reinterpret_cast<char*>(&table.visibleCellSymbols), sizeof(size_t));
	stream.read(reinterpret_cast<char*>(&table.alignment), sizeof(Alignment));

	table.cells = new Cell*[table.maxTableRows];
	for (size_t i = 0; i < table.maxTableRows; i++) {
		table.cells[i] = new Cell[table.maxTableCols];
	}

	for (size_t i = 0; i < table.maxTableRows; i++) {
		for (size_t j = 0; j < table.maxTableCols; j++) {
			table.cells[i][j].setPosition(Position(i, j));
			table.cells[i][j].setTable(&table);
			stream >> table.cells[i][j];
		}
	}

	return stream;
}
