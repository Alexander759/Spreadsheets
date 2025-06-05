#include "PropertyBase.h"

PropertyBase::PropertyBase(const MyString& name) : name(name), errorFlag(true) {
	this->errorMessage = MyString("ABORTING! ") + this->name + " - Missing property!";
}

bool PropertyBase::hasError() {
	return this->errorFlag;
}

void PropertyBase::setName(const MyString& name) {
	this->name = name;
}

const MyString& PropertyBase::getName() const {
	return this->name;
}

const MyString& PropertyBase::getErrorMessage() const {
	return this->errorMessage;
}

void PropertyBase::setErrorMessage(const MyString& message) {
	this->errorMessage = message;
}

void PropertyBase::setErrorFlag(bool flag) {
	this->errorFlag = flag;
}
