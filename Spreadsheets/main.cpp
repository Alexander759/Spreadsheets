#include <iostream>
#include "TableConfigure.h"
#include "Position.h"

int main()
{
	Position p = Position::fromString("AA3");
	std::cout << p.getRow() << " " << p.getCol() << std::endl;;
}
