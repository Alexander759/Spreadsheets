#include "Cell.h"

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

    this->contentChanged.trigger(this, &args);
}

Event<Cell, ChangeContentArgs>& Cell::getContentChangedEvent() {
    return this->contentChanged;
}

List<Cell*>& Cell::getDependencies() {
    return this->dependencies;
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
    cell.unsubscribeFromEvents();
    cell.setCellDisplayAndType(cell.rawContent.toUpper(), CellType::Bool);
}

void Cell::handleDoubleContent(Cell& cell) {
    cell.unsubscribeFromEvents();
    cell.setCellDisplayAndType(cell.rawContent, CellType::Number);
}

void Cell::handleStringContent(Cell& cell) {
    cell.unsubscribeFromEvents();
    cell.setCellDisplayAndType(cell.rawContent, CellType::String);
}

void Cell::handleReferenceContent(Cell& cell) {
}

void Cell::handleFormulaContent(Cell& cell) {
}

void Cell::changedReference(Cell* sender, ChangeContentArgs* args) {
    if (sender == nullptr || args == nullptr) {
        return;
    }

    this->setCellDisplayAndType(sender->getDisplayContent(), sender->getCellType());
}

void Cell::unsubscribeFromEvents() {
    for (size_t i = 0; i < this->dependencies.getLength(); i++) {
        this->dependencies[i]->getContentChangedEvent().unsubscribe(activeFunction);
    }
}

