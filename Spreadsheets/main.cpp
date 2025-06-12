#include <iostream>
#include "TableConfigure.h"
#include "Position.h"
#include "Table.h"
#include "FormulasHolder.h"

int main()
{
	FormulasHolder::setFormulas();

	TableConfigure config("config.txt");
	Table t(config);


	t.getAtPosition(Position(1, 1))->setRawContent("1");
	t.getAtPosition(Position(2, 1))->setRawContent("2");
	t.getAtPosition(Position(3, 1))->setRawContent("3");
	t.getAtPosition(Position(4, 1))->setRawContent("=MAX(A1:A3)");



	t.print();
}
