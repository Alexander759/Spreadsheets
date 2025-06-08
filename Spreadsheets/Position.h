#pragma once

class Position {
public:
	Position(size_t row = 0, size_t col = 0);

	void setCol(size_t col);
	size_t getCol() const;
	
	void setRow(size_t row);
	size_t getRow() const;

	bool operator==(const Position& other);
	bool operator!=(const Position& other);

private:
	size_t col;
	size_t row;
};

