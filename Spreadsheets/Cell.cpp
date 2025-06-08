#include "Cell.h"
#include "Table.h"
#include <stdexcept>

Cell::Cell() {
    this->type = CellType::EmptyCell;
    this->activeFunction = nullptr;
}

const Position& Cell::getPosition() const {
    return this->position;
}

void Cell::setPosition(const Position& position) {
    this->position = position;
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

CellType Cell::getCellType() const {
    return this->type;
}

void Cell::setCellDisplayAndType(const MyString& displayContent, CellType type) {
    ChangeContentArgs args(this->displayContent, this->type);

    this->displayContent = displayContent;
    this->type = type;

    for (size_t i = 0; i < givingTo.getLength(); i++) {
        Cell* cellPtr = table->getAtPosition(givingTo[i]);
        if (cellPtr == nullptr) {
            givingTo.removeAt(i);
            i--;
            continue;
        }

        if (cellPtr->activeFunction != nullptr) {
            cellPtr->activeFunction(cellPtr, *this, args);
        }
    }
}

List<Position>& Cell::getDependents() {
    return this->givingTo;
}

void Cell::parseRawContent(Cell& cell) {
    if (cell.rawContent.isBool()) {
        handleBoolContent(cell);
        return;
    }

    if (cell.rawContent.isDouble()) {
        handleDoubleContent(cell);
        return;
    }

    if (cell.rawContent[0] == '=' && cell.rawContent.indexOf('(') != -1) {
        handleFormulaContent(cell);
        return;
    }

    if (cell.rawContent[0] == '=') {
        handleReferenceContent(cell);
        return;
    }
    
    handleStringContent(cell);
}

void Cell::handleBoolContent(Cell& cell) {
    cell.removeOldEdges();
    cell.setCellDisplayAndType(cell.rawContent.toUpper(), CellType::Bool);
}

void Cell::handleDoubleContent(Cell& cell) {
    cell.removeOldEdges();
    cell.setCellDisplayAndType(cell.rawContent, CellType::Number);
}

void Cell::handleStringContent(Cell& cell) {
    cell.removeOldEdges();
    cell.setCellDisplayAndType(cell.rawContent, CellType::String);
}

void Cell::handleReferenceContent(Cell& cell) {
    MyString positionInString = cell.rawContent.subStr(cell.rawContent.indexOf('=') + 1, cell.rawContent.getLength() - 1);

    if (!Position::isPosition(positionInString)) {
        throw std::invalid_argument("Not position");
    }

    Position positionOfReferencedCell = Position::fromString(positionInString);

    Cell* referencedCell = cell.table->getAtPosition(positionOfReferencedCell);

    Cell::addEdge(&cell, referencedCell);

    cell.activeFunction = changedReference;
}

void Cell::handleFormulaContent(Cell& cell) {
}

void Cell::addEdge(Cell* gets, Cell* gives) {
    if (gets == nullptr || gives == nullptr) {
        return;
    }

    gets->gettingFrom.add(gives->getPosition());
    gives->givingTo.add(gets->getPosition());
}

void Cell::removeEdge(Cell* gets, Cell* gives) {
    if (gets == nullptr || gives == nullptr) {
        return;
    }

    gives->givingTo.remove(gets->getPosition());
    gets->gettingFrom.remove(gives->getPosition());
}


void Cell::changedReference(Cell* reciever, const Cell& sender, const ChangeContentArgs& args) {
    reciever->setCellDisplayAndType(sender.getDisplayContent(), sender.getCellType());
}

void Cell::removeOldEdges() {
    for (size_t i = 0; i < this->gettingFrom.getLength(); i++) {
        Cell::removeEdge(this, table->getAtPosition(this->gettingFrom[i]));
    }
}

