#pragma once
#include "MyString.h"

class Position {
public:
	Position(size_t row = 1, size_t col = 1);
	Position(const MyString& str);
	void setCol(size_t col);
	size_t getCol() const;
	
	void setRow(size_t row);
	size_t getRow() const;

	bool operator==(const Position& other);
	bool operator!=(const Position& other);

	MyString toString();
	
	static Position fromString(const MyString& str);
	static bool isPosition(const MyString& str);

private:
	size_t col;
	size_t row;
};

