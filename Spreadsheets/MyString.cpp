#include "MyString.h"
#include <iostream>
#include <stdexcept>
#include <cstring>

MyString::MyString() {
	this->length = 0;
	this->content = new char[this->length + 1];
	this->content[0] = '\0';
}

MyString::MyString(size_t num) {
	size_t numCopy = num;

	int digitsOfNum = 0;
	do {
		numCopy /= 10;
		digitsOfNum++;
	} while (numCopy > 0);

	this->length = digitsOfNum;
	this->content = new char[this->length + 1];
	this->content[this->length] = '\0';
	for (size_t i = 0; i < length; i++) {
		this->content[i] = num % 10 + '0';
		num /= 10;
	}

	this->reverse();
}

MyString::MyString(const char* content) {
	this->length = strlen(content);
	this->content = new char[this->length + 1];
	strcpy_s(this->content, this->length + 1, content);
}

MyString::MyString(const MyString& other) {
	copyFrom(other);
}

MyString::MyString(MyString&& other) {
	this->content = other.content;
	this->length = other.length;
	other.content = nullptr;
	other.length = 0;
}

MyString& MyString::operator=(const MyString& other) {
	if (this != &other) {
		this->free();
		this->copyFrom(other);
	}

	return *this;
}

MyString& MyString::operator=(MyString&& other) {
	if (this != &other) {
		this->free();
		this->content = other.content;
		this->length = other.length;
		other.content = nullptr;
		other.length = 0;
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

void MyString::reverse() {
	char swap;
	for (size_t i = 0; i < length / 2; i++) {
		swap = this->content[i];
		this->content[i] = this->content[length - 1 - i];
		this->content[length - 1 - i] = swap;
	}
}

int MyString::indexOf(char symbol) const {
	for (int i = 0; i < length; i++) {
		if (this->content[i] == symbol) {
			return i;
		}
	}

	return -1;
}

MyString MyString::subStr(size_t start, size_t end) {
	if (end >= this->length || start >= this->length) {
		throw std::out_of_range("Index bigger than length");
	}

	if (end < start) {
		return MyString();
	}

	MyString result;
	result.free();

	result.length = end - start + 1;
	result.content = new char[result.length + 1];
	for (size_t i = 0; i < result.length; i++) {
		result.content[i] = this->content[i + start];
	}
	result.content[result.length] = '\0';

	return result;
}

List<MyString> MyString::split(char separator) {
	List<MyString> result;

	size_t start = 0;
	
	for (size_t i = 0; i < length; i++) {
		if (this->content[i] == separator) {
			if (i > start) {
				result.add(this->subStr(start, i - 1));
			}
			start = i + 1;
		}
	}

	if (start < length) {
		result.add(this->subStr(start, length -	1));
	}

	return result;
}

MyString MyString::toLower() const {
	MyString result(*this);
	for (size_t i = 0; i < length; i++) {
		if ('A' <= result.content[i] && result.content[i] <= 'Z') {
			result.content[i] = result.content[i] + 'a' - 'A';
		}
	}

	return result;
}

MyString MyString::toUpper() const {
	MyString result(*this);
	for (size_t i = 0; i < length; i++) {
		if ('a' <= result.content[i] && result.content[i] <= 'z') {
			result.content[i] = result.content[i] + 'A' - 'a';
		}
	}

	return result;
}

bool MyString::isSizeT() const {
	if (this->length == 0) {
		return false;
	}

	size_t prev = 0;
	size_t max = 1;
	while (max != prev) {
		prev = max;
		max <<= 1;
		max++;
	}

	MyString maxSizeTInMyString(max);
	if (this->length > maxSizeTInMyString.getLength()) {
		return false;
	}

	if (this->length == maxSizeTInMyString.length && *this > maxSizeTInMyString) {
		return false;
	}

	for (size_t i = 0; i < length; i++) {
		if (this->content[i] < '0' || this->content[i] > '9') {
			return false;
		}
	}

	return true;
}

size_t MyString::toSizeT() const {
	size_t result = 0;
	for (size_t i = 0; i < length; i++) {
		if (this->content[i] == ' ') {
			continue;
		}

		result *= 10;
		result += this->content[i] - '0';
	}

	return result;
}

bool MyString::isBool() const {
	return this->toLower() == "true" || this->toLower() == "false";
}

bool MyString::toBool() const {
	if (!isBool()) {
		throw std::exception("Invalid string");
	}

	return this->toLower() == "true";
}

int MyString::compareTo(const MyString& other) const {
	return strcmp(this->content, other.content);
}

bool MyString::operator==(const MyString& other) const {
	return compareTo(other) == 0;
}

bool MyString::operator!=(const MyString& other) const {
	return compareTo(other) != 0;
}

bool MyString::operator>(const MyString& other) const {
	return compareTo(other) > 0;
}

bool MyString::operator>=(const MyString& other) const {
	return compareTo(other) >= 0;
}

bool MyString::operator<(const MyString& other) const {
	return compareTo(other) < 0;
}

bool MyString::operator<=(const MyString& other) const {
	return compareTo(other) <= 0;
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

char MyString::operator[](size_t index) const {
	if (index >= this->length) {
		throw std::out_of_range("Index bigger than length");
	}

	return this->content[index];
}

char& MyString::operator[](size_t index) {
	if (index >= this->length) {
		throw std::out_of_range("Index bigger than length");
	}

	return this->content[index];
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

std::istream& operator>>(std::istream& stream, MyString& string) {
	const int MAXINPUTSIZE = 1024;
	char input[MAXINPUTSIZE];
	stream.getline(input, MAXINPUTSIZE);
	string.setCString(input);

	return stream;
}

std::ostream& operator<<(std::ostream& stream, const MyString& other) {
	stream << other.getCString();
	return stream;
}
