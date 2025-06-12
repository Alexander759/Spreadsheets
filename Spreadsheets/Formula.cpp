#include "Formula.h"

Formula::Formula() {
	this->onRun = nullptr;
	this->onChange = nullptr;
}

Formula::Formula(const MyString& name, void(*onRun)(Cell* currentCell, const List<MyString>& args), void(*onChange)(Cell* receiver, const Cell* sender, const ChangeContentArgs* args)) {
	this->name = name;
	this->onRun = onRun;
	this->onChange = onChange;
}

const MyString& Formula::getName() const {
	return this->name;
}

void(*Formula::getOnRun() const)(Cell*, const List<MyString>&) {
	return this->onRun;
}

void(*Formula::getOnChange() const)(Cell* receiver, const Cell* sender, const ChangeContentArgs* args) {
	return this->onChange;
}
