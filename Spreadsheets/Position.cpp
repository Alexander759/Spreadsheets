#include "Position.h"

Position::Position(size_t row, size_t col) : row(row), col(col) {
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
