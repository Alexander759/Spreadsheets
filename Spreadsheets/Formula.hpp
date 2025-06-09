#pragma once
#include "MyString.h"
#include "List.hpp"

template <typename T>
class Formula {
public:
	MyString name;

	void getParameters(const List<MyString>& parameters);

	void (*f)(T p);
private:

};