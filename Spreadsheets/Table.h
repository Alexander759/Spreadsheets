#pragma once
#include "Cell.h"
#include "Position.h"
#include "TableConfigure.h"

class Table {
public:
	Table(const TableConfigure& config);
	Table(const Table& other);
	Table(Table&& other);
	Table& operator=(const Table& other);
	Table& operator=(Table&& other);
	~Table();

	Cell* getAtPosition(const Position& position);
	const Cell* getAtPosition(const Position& position) const;

	void draw() const;
private:

	void copyFrom(const Table& other);
	void copyStatic(const Table& other);
	void free();

	bool autoFit;
	bool clearConsoleAfterCommand;
	Alignment alignment;
	size_t initialTableRows;
	size_t initialTableCols;
	size_t maxTableRows;
	size_t maxTableCols;
	size_t visibleCellSymbols;
	Cell** cells;
};