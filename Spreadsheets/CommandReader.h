#pragma once
#include "MyString.h"
#include "List.hpp"

class Reader {
public:
	static List<MyString> readCommand();
	static List<List<MyString>> readProperties(const MyString& fileName);
private:
	static List<MyString> readProperty(std::istream& str);

};