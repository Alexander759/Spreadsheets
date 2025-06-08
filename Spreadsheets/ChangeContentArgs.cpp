#include "ChangeContentArgs.h"

ChangeContentArgs::ChangeContentArgs(MyString previousContent, CellType previousCellType) {
    this->previousContent = previousContent;
    this->previousCellType = previousCellType;
}

const MyString& ChangeContentArgs::getPreviousContent() const {
    return this->previousContent;
}

void ChangeContentArgs::setPreviousContent(MyString previousContent) {
    this->previousCellType = previousCellType;
}

CellType ChangeContentArgs::getPreviousCellType() const {
    return this->previousCellType;
}

void ChangeContentArgs::setPreviousCellType(CellType previousCellType) {
    this->previousCellType = previousCellType;
}
