#include "Cell.h"

Cell::Cell() {
    this->type = CellType::EmptyCell;
}

const MyString& Cell::getRawContent() const {
    return this->rawContent;
}

void Cell::setRawContent(const MyString& rawContent) {
    this->rawContent = rawContent;
}

const MyString& Cell::getDisplayContent() const {
    return this->displayContent;
}

void Cell::setDisplayContent(const MyString& displayContent) {
    this->displayContent = displayContent;
}

CellType Cell::getCellType() const {
    return this->type;
}

void Cell::setType(CellType type) {
    this->type = type;
}
