#include <iostream>
#include "TableConfigure.h"

int main()
{
    TableConfigure config("config.txt");
    std::cout << config.hasError() << std::endl;
    std::cout << config.getErrorMessage() << std::endl;
    int a = 5;
}
