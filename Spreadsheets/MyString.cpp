#include "MyString.h"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <fstream>

MyString::MyString() {
	this->length = 0;
	this->content = new char[this->length + 1];
	this->content[0] = '\0';
}

MyString::MyString(char symbol) {
	this->length = 1;
	this->content = new char[this->length + 1];
	this->content[0] = symbol;
	this->content[1] = '\0';
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

MyString::MyString(int num) {
	this->length = 0;
	this->content = new char[1];
	this->content[0] = '\0';
	bool isNegative = false;

	if (num < 0) {
		isNegative = true;
		num *= -1;
	}
	size_t positiveNum = num;
	*this = MyString(positiveNum);

	if (isNegative) {
		*this = MyString("-") + *this;
	}
}


MyString::MyString(double num) {
	this->length = 0;
	this->content = new char[1];
	this->content[0] = '\0';

	bool isNegative = false;
	if (num < 0) {
		num *= -1;
		isNegative = true;
	}

	int integerPart = num;
	num -= integerPart;
	*this += MyString(integerPart);

	const int PRECISION = 10;

	int leadingZeroes = 0;
	double numCopy = num;
	while (numCopy < 0.1 && leadingZeroes < PRECISION) {
		numCopy *= 10;
		leadingZeroes++;
	}

	if (leadingZeroes < PRECISION) {
		*this += ".";
		for (size_t i = 0; i < leadingZeroes; i++) {
			*this += "0";
		}

		numCopy = num;
		for (size_t i = 0; i < PRECISION; i++) {
			numCopy *= 10;
		}

		size_t copyFirstSixDigits = numCopy;
		MyString afterDecimal(copyFirstSixDigits);
		size_t lastZeroIndex = afterDecimal.getLength();
		for (int i = afterDecimal.getLength() - 1; i >= 0; i--) {
			if (afterDecimal.content[i] != '0') {
				lastZeroIndex = i + 1;
				break;
			}
		}

		afterDecimal = afterDecimal.subStr(0, lastZeroIndex - 1);
		*this += afterDecimal;
	}

	if (isNegative) {
		*this = MyString("-") + *this;
	}
}

MyString::MyString(const char* content) {
	if (content == nullptr) {
		throw std::invalid_argument("Nullptrs can't be referenced");
	}

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

MyString MyString::subStr(size_t start, size_t end) const {
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

List<MyString> MyString::split(char separator) const {
	List<MyString> result;

	size_t start = 0;
	bool insideQuatations = false;
	size_t parentheses = 0;

	for (size_t i = 0; i < length; i++) {
		if (this->content[i] == separator && !insideQuatations && parentheses == 0) {
			if (i > start) {
				result.add(this->subStr(start, i - 1));
			}
			start = i + 1;
		}

		if (this->content[i] == '"' && parentheses == 0) {
			insideQuatations = !insideQuatations;
		}

		if (this->content[i] == '(' && !insideQuatations) {
			parentheses++;
		}

		if (this->content[i] == ')' && parentheses > 0 && !insideQuatations) {
			parentheses--;
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

bool MyString::isDouble() const {
	size_t i = 0;

	while (i < this->length && this->content[i] == ' ') {
		i++;
	}


	if (i < this->length && (this->content[i] == '-' || this->content[i] == '+')) {
		i++;
	}

	bool hasDigitsBefore = false;

	while (i < this->length && this->content[i] != '.' && this->content[i] != ',') {
		if (this->content[i] < '0' || this->content[i] > '9') {
			return false;
		}
		hasDigitsBefore = true;
		i++;
	}

	bool hasDecimalPoint = false;
	if (i < this->length && (this->content[i] == '.' || this->content[i] == ',')) {
		hasDecimalPoint = true;
		i++;
	}

	bool hasDigitsAfter = false;
	while (i < this->length && this->content[i] != ' ') {
		if (this->content[i] < '0' || this->content[i] > '9') {
			return false;
		}
		hasDigitsAfter = true;
		i++;
	}

	while (i < this->length && this->content[i] == ' ') {
		i++;
	}

	return i == this->length && (hasDigitsBefore || (hasDecimalPoint && hasDigitsAfter));
}

double MyString::toDouble() const {
	if (!isDouble()) {
		throw std::invalid_argument("Couldn't convert to double");
	}

	size_t leftPart = 0;

	size_t i = 0;

	while (this->content[i] == ' ') {
		i++;
	}

	bool isNegative = false;
	if (this->content[i] == '-') {
		isNegative = true;
		i++;
	}
	else if (this->content[i] == '+') {
		i++;
	}

	while (i < this->length && this->content[i] != '.' && this->content[i] != ',') {
		if (this->content[i] < '0' || this->content[i] > '9') {
			throw std::invalid_argument("Couldn't convert to double");
		}

		leftPart *= 10;
		leftPart += this->content[i] - '0';
		i++;
	}

	if (i < this->length && (this->content[i] == '.' || this->content[i] == ',')) {
		i++;
	}

	size_t rightPart = 0;

	double divideBy = 1;
	while (i < this->length && this->content[i] != ' ') {
		if (this->content[i] < '0' || this->content[i] > '9') {
			throw std::invalid_argument("Couldn't convert to double");
		}

		rightPart *= 10;
		rightPart += this->content[i] - '0';
		divideBy *= 10;
		i++;
	}

	double result = leftPart + (rightPart / divideBy);

	if (isNegative) {
		result *= -1;
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
	if (!this->isSizeT()) {
		throw std::invalid_argument("Can't convert to size_t");
	}
	
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

MyString MyString::removeFreeSpaces() const {
	MyString result;
	bool isInQuotations = false;
	for (size_t i = 0; i < length; i++) {
		if (this->content[i] == ' ' && !isInQuotations) {
			continue;
		}

		if (this->content[i] == '"') {
			isInQuotations = !isInQuotations;
		}

		result += this->content[i];
	}

	return result;
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

std::ofstream& operator<<(std::ofstream& stream, const MyString& str) {
	if (!stream.good()) {
		return stream;
	}

	stream.write(reinterpret_cast<const char*>(&str.length), sizeof(size_t));
	stream.write(reinterpret_cast<const char*>(str.content), str.length + 1);

	return stream;
}

std::ifstream& operator>>(std::ifstream& stream, MyString& str) {
	if (!stream.good()) {
		return stream;
	}

	str.free();

	stream.read(reinterpret_cast<char*>(&str.length), sizeof(size_t));

	str.content = new char[str.length + 1];
	stream.read(reinterpret_cast<char*>(str.content), str.length + 1);

	return stream;
}

std::istream& operator>>(std::istream& stream, MyString& str) {
	const int MAXINPUTSIZE = 1024;
	char input[MAXINPUTSIZE];
	stream.getline(input, MAXINPUTSIZE);
	str.setCString(input);

	return stream;
}

std::ostream& operator<<(std::ostream& stream, const MyString& str) {
	stream << str.getCString();
	return stream;
}
