#include "Table.h"
#include <iostream>

Table::Table(const TableConfigure& config) {
	this->autoFit = config.getAutoFit();
	this->clearConsoleAfterCommand = config.getClearConsoleAfterCommand();
	this->alignment = config.getAlignment();
	this->initialTableRows = config.getInitialTableRows();
	this->initialTableCols = config.getInitialTableRows();
	this->maxTableRows = config.getMaxTableRows();
	this->maxTableCols = config.getMaxTableCols();
	this->visibleCellSymbols = config.getVisibleCellSymbols();

	this->cells = new Cell*[this->maxTableRows];

	for (size_t i = 0; i < this->maxTableRows; i++) {
		this->cells[i] = new Cell[this->maxTableCols];

		for (size_t j = 0; j < this->maxTableCols; j++) {
			this->cells[i][j].setPosition(Position(i, j));
			this->cells[i][j].setTable(this);
		}
	}
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

void Table::draw() const {

	std::cout << "|";
	for (size_t i = 0; i < this->initialTableRows; i++) {
		for (size_t i = 0; i < this->initialTableCols; i++) {
			std::cout << '---';
		}
	}
	std::cout << "|" << std::endl;


	for (size_t i = 0; i < this->initialTableRows; i++) {
		for (size_t i = 0; i < this->initialTableCols; i++) {
			std::cout << '---';
		}
	}


	for (size_t i = 0; i < this->initialTableRows; i++) {
		for (size_t j = 0; j < this->initialTableCols; j++) {

		}
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
