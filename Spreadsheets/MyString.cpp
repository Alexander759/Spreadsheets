#include "MyString.h"
#include <iostream>
#include <cstring>

MyString::MyString() {
	this->length = 0;
	this->content = new char[this->length + 1];
	this->content[0] = '\0';
}

MyString::MyString(const MyString& other) {
	copyFrom(other);
}

MyString& MyString::operator=(const MyString& other) {
	if (this != &other) {
		this->free();
		this->copyFrom(other);
	}

	return *this;
}

MyString::~MyString() {
	this->free();
}

const char* MyString::getCString() const {
	return this->content;
}

void MyString::setCString(const char* str) {
	if (str == nullptr) {
		return;
	}

	delete[] this->content;
	this->length = strlen(str);
	this->content = new char[this->length + 1];
	strcpy_s(this->content, this->length + 1, str);
}

size_t MyString::getLength() const {
	return this->length;
}

void MyString::concat(const MyString& other) {
	size_t newLength = this->length + other.length;

	char* newContent = new char[newLength + 1];

	strcpy_s(newContent, newLength + 1, this->content);
	strcat_s(newContent, newLength + 1, other.content);

	delete[] this->content;
	this->content = newContent;
	this->length = newLength;
}

MyString MyString::operator+(const MyString& other) const {
	MyString result(*this);
	result += other;
	return result;
}

MyString& MyString::operator+=(const MyString& other) {
	this->concat(other);
	return *this;
}

void MyString::copyFrom(const MyString& other) {
	this->length = other.length;
	this->content = new char[this->length + 1];
	strcpy_s(this->content, this->length + 1, other.content);
}

void MyString::free() {
	delete[] this->content;
	this->content = nullptr;
	this->length = 0;
}
