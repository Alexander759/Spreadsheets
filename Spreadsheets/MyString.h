#pragma once
#include "List.hpp"

class MyString {
public:
	MyString();
	MyString(size_t num);
	MyString(const char* content);
	MyString(const MyString& other);
	MyString(MyString&& other);
	MyString& operator=(const MyString& other);
	MyString& operator=(MyString&& other);
	~MyString();

	const char* getCString() const;
	void setCString(const char* str);

	size_t getLength() const;

	void concat(const MyString& other);
	void reverse();

	int indexOf(char symbol) const;
	MyString subStr(size_t start, size_t end);
	List<MyString> split(char separator = ' ');

	MyString toLower() const;
	MyString toUpper() const;

	bool isSizeT() const;
	size_t toSizeT() const;

	bool isBool() const;
	bool toBool() const;

	int compareTo(const MyString& other) const;

	bool operator==(const MyString& other) const;
	bool operator!=(const MyString& other) const;
	bool operator>(const MyString& other) const;
	bool operator>=(const MyString& other) const;
	bool operator<(const MyString& other) const;
	bool operator<=(const MyString& other) const;

	MyString operator+(const MyString& other) const;
	MyString& operator+=(const MyString& other);

	char operator[](size_t index) const;
	char& operator[](size_t index);

	friend std::istream& operator>>(std::istream& stream, MyString& other);
	friend std::ostream& operator<<(std::ostream& stream, const MyString& other);
private:
	void copyFrom(const MyString& other);
	void free();

	char* content;
	size_t length;
};