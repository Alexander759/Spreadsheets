#pragma once
#include "Cell.h"
#include "Position.h"
#include "TableConfigure.h"
#include <fstream>

class Table {
public:
	Table() = default;
	Table(const TableConfigure& config);
	Table(const Table& other);
	Table(Table&& other);
	Table& operator=(const Table& other);
	Table& operator=(Table&& other);
	~Table();

	void setNewConfigure(const TableConfigure& config);

	Cell* getAtPosition(const Position& position);
	const Cell* getAtPosition(const Position& position) const;

	void print() const;
	void printRowBorder(const List<size_t>& maxSymbols) const;
	void printRow(const List<size_t>& maxSymbols, int i) const;
	List<size_t> maxNumberOfCharactersPerColumn() const;

	void setCurrentRowsAndCols(const Position& pos);

	friend class FormulasHolder;
	friend std::ofstream& operator<<(std::ofstream& stream, const Table& table);
	friend std::ifstream& operator>>(std::ifstream& stream, Table& table);

private:
	void copyFrom(const Table& other);
	void copyStatic(const Table& other);
	void free();

	bool autoFit;
	bool clearConsoleAfterCommand;
	Alignment alignment;
	size_t initialTableRows;
	size_t initialTableCols;
	size_t currentTableRows;
	size_t currentTableCols;
	size_t maxTableRows;
	size_t maxTableCols;
	size_t visibleCellSymbols;
	Cell** cells;
};