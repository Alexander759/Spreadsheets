#pragma once
#include "MyString.h"
#include "List.hpp"
#include "Cell.h"

class Formula {
public:
	Formula();
	Formula(const MyString& name, void (*onRun)(Cell* currentCell, const List<MyString>& args), 
		void (*onChange)(Cell* receiver, const Cell* sender, const ChangeContentArgs* args));

	const MyString& getName() const;
	void (*getOnRun() const)(Cell*, const List<MyString>&);
	void (*getOnChange() const)(Cell* receiver, const Cell* sender, const ChangeContentArgs* args);

private:
	MyString name;
	void (*onRun)(Cell* currentCell, const List<MyString>& args);
	void (*onChange)(Cell* receiver, const Cell* sender, const ChangeContentArgs* args);

};