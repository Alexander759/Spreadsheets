#include <iostream>
#include "TableConfigure.h"
#include "Position.h"
#include "Table.h"

int main()
{

	Table t;
	Position p = Position::fromString("AA3");
	std::cout << p.getRow() << " " << p.getCol() << std::endl;;
}
