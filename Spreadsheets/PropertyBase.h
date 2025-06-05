#pragma once
#include "MyString.h"

class PropertyBase {
public:
	PropertyBase(const MyString& name);

	virtual bool setFromString(const MyString& str) = 0;
	virtual bool isStringValid(const MyString& str) = 0;

	bool hasError();

	void setName(const MyString& name);
	const MyString& getName() const;

	const MyString& getErrorMessage() const;

	virtual ~PropertyBase() = default;

	void setErrorMessage(const MyString& message);
	
	void setErrorFlag(bool flag);

private:
	MyString name;
	MyString errorMessage;
	bool errorFlag;
};