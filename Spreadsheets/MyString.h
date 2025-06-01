#pragma once

class MyString {
public:
	MyString();
	MyString(const MyString& other);
	MyString& operator=(const MyString& other);
	~MyString();

	const char* getCString() const;
	void setCString(const char* str);

	size_t getLength() const;

	void concat(const MyString& other);

	MyString operator+(const MyString& other) const;
	MyString& operator+=(const MyString& other);

private:
	void copyFrom(const MyString& other);
	void free();

	char* content;
	size_t length;
};