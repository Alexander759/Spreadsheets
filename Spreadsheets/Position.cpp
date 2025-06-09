#include "Position.h"

Position::Position(size_t row, size_t col) : row(row), col(col) {
}

Position::Position(const MyString& str) {
	if (this->isPosition(str)) {
		Position pos = fromString(str);
		this->row = pos.col;
		this->col = pos.row;
		return;
	}

	this->col = 1;
	this->row = 1;
}

void Position::setCol(size_t col) {
	this->col = col;
}

size_t Position::getCol() const {
	return this->col;
}

void Position::setRow(size_t row) {
	this->row = row;
}

size_t Position::getRow() const {
	return this->row;
}

bool Position::operator==(const Position& other) {
	return this->col == other.col && this->row == other.row;
}

bool Position::operator!=(const Position& other) {
	return this->col != other.col || this->row != other.row;
}

MyString Position::toString() {
	const size_t LETTERSINALPHABET = 26;
	MyString result;

	size_t colCopy = this->col;
	while (colCopy > 0) {
		colCopy--;
		char letter = 'A' + (colCopy % LETTERSINALPHABET);
		result += MyString(letter);
		colCopy = colCopy / LETTERSINALPHABET;
	};

	result.reverse();


	result += MyString(this->row);

	return result;
}

Position Position::fromString(const MyString& str) {
	MyString strUpper = str.toUpper();

	if (!isPosition(strUpper)) {
		return Position(0, 0);
	}

	size_t endOfCol = 0;
	for (size_t i = 0; i < strUpper.getLength(); i++) {
		if ('1' <= strUpper[i] && strUpper[i] <= '9') {
			endOfCol = i;
			break;
		}
	}
	endOfCol--;

	MyString colStr = strUpper.subStr(0, endOfCol);

	const size_t LETTERSINALPHABET = 26;
	size_t col = 0;
	for (size_t i = 0; i < colStr.getLength(); i++) {
		col *= LETTERSINALPHABET;
		col += colStr[i] - 'A';
		col++;
	}

	size_t row = strUpper.subStr(endOfCol + 1, strUpper.getLength() - 1).toSizeT();

	return Position(row, col);
}

bool Position::isPosition(const MyString& str) {
	if (str.getLength() < 2) {
		return false;
	}

	MyString strUpper = str.toUpper();
	size_t i;
	for (i = 0; i < strUpper.getLength(); i++) {
		if (strUpper[i] < 'A' || 'Z' < strUpper[i]) {
			break;
		}
	}

	if (i == 0 || i == strUpper.getLength()) {
		return false;
	}

	if (strUpper[i] < '1' || '9' < strUpper[i]) {
		return false;
	}
	i++;

	for (; i < strUpper.getLength(); i++) {
		if (strUpper[i] < '0' || '9' < strUpper[i]) {
			return false;
		}
	}

	return true;
}
