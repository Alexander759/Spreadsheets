#pragma once
#include "Cell.h"
#include "Position.h"

class Table {
public:
	Cell* getAtPosition(const Position& position);
private:
	List<List<Cell>> content;
};