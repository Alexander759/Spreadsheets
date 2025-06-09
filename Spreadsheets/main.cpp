#include <iostream>
#include "TableConfigure.h"
#include "Position.h"
#include "Table.h"

int main()
{
	TableConfigure config("config.txt");
	Table t(config);

	t.getAtPosition(Position(2, 1))->setRawContent("1");
	t.getAtPosition(Position(1, 1))->setCellDisplayAndType("Done", CellType::String);
	t.getAtPosition(Position(3, 1))->setCellDisplayAndType("test test test test", CellType::String);
	t.getAtPosition(Position(3, 2))->setCellDisplayAndType("Finaly working", CellType::String);
	t.getAtPosition(Position(4, 1))->setRawContent("=A3");
	t.getAtPosition(Position(4, 1))->setRawContent("false");
	
	t.print();
}
