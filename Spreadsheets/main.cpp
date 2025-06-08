#include <iostream>
#include "TableConfigure.h"
#include "Position.h"
#include "Table.h"

int main()
{
	TableConfigure config("config.txt");
	Table t(config);
}
