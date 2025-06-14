#include "Cell.h"
#include "Table.h"
#include "Parser.h"
#include "MyString.h"
#include "FormulasHolder.h"
#include <stdexcept>

MyString Cell::errorStateMessage = "#VALUE!";

Cell::Cell() {
    this->type = CellType::EmptyCell;
    this->activeFunction = nullptr;
    this->table = nullptr;
}

void Cell::setTable(Table* table) {
    if (table == nullptr) {
        return;
    }

    this->table = table;
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
    if (this->table == nullptr) {
        return;
    }

    this->table->setCurrentRowsAndCols(this->position);
    this->rawContent = rawContent;
    parseRawContent(*this);
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

    this->onChangeContent(args);
}

List<Position>& Cell::getDependents() {
    return this->dependantTo;
}

void Cell::parseRawContent(Cell& cell) {
    if (cell.table == nullptr) {
        return;
    }

    RawContentType rawContentType = Parser::getRawContentTypeFromInput(cell.rawContent);

    if (rawContentType == RawContentType::EmptyContent) {
        handleNoContent(cell);
        return;
    }

    if (rawContentType == RawContentType::Bool) {
        handleBoolContent(cell);
        return;
    }

    if (rawContentType == RawContentType::Number) {
        handleDoubleContent(cell);
        return;
    }

    if (rawContentType == RawContentType::Expression) {
        handleFormulaContent(cell);
        return;
    }

    if (rawContentType == RawContentType::Reference) {
        handleReferenceContent(cell);
        return;
    }
    
    if (rawContentType == RawContentType::String) {
        handleStringContent(cell);
        return;
    }

    cell.setCellDisplayAndType(Cell::errorStateMessage, CellType::Error);
}

void Cell::handleNoContent(Cell& cell) {
    cell.setCellDisplayAndType("", CellType::EmptyCell);
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
    cell.setCellDisplayAndType(cell.rawContent.subStr(1, cell.rawContent.getLength() - 2), CellType::String);
}

void Cell::handleReferenceContent(Cell& cell) {
    MyString positionInString = cell.rawContent.subStr(cell.rawContent.indexOf('=') + 1, cell.rawContent.getLength() - 1);

    if (!Position::isPosition(positionInString)) {
        cell.setCellDisplayAndType(Cell::errorStateMessage, CellType::Error);
        return;
    }

    Position positionOfReferencedCell = Position::fromString(positionInString);

    Cell* referencedCell = cell.table->getAtPosition(positionOfReferencedCell);

    if (referencedCell == nullptr) {
        cell.setCellDisplayAndType(Cell::errorStateMessage, CellType::Error);
        return;
    }

    if (referencedCell->hasPathTo(cell.getPosition())) {
        cell.setCellDisplayAndType(Cell::errorStateMessage, CellType::Error);
        return;
    }

    Cell::addEdge(&cell, referencedCell);
    cell.activeFunction = changedReference;

    cell.setCellDisplayAndType(referencedCell->getDisplayContent(), referencedCell->getCellType());
}

void Cell::handleFormulaContent(Cell& cell) {
    MyString functionName = Parser::getExpressionName(cell.rawContent);
    List<MyString> arguments = Parser::getArgumentsFromExpression(cell.rawContent);
    bool functionIsFound = false;
    for (size_t i = 0; i < FormulasHolder::getFormulas().getLength(); i++) {
        const Formula& formula = FormulasHolder::getFormulas()[i];

        if (formula.getName().toUpper() == functionName.toUpper()) {
            functionIsFound = true;
            formula.getOnRun()(&cell, arguments);
            cell.activeFunction = formula.getOnChange();
        }

    }

    if (!functionIsFound) {
        cell.setCellDisplayAndType(Cell::errorStateMessage, CellType::Error);
        return;
    }
}

bool Cell::hasPathTo(const Position& position) {
    if (this->position == position) {
        return true;
    }

    List<Position> visited;

    for (size_t i = 0; i < this->dependsFrom.getLength(); i++) {
        Cell* cell = table->getAtPosition(this->dependsFrom[i]);

        if (cell == nullptr) {
            continue;
        }

        if (cell->hasPathTo(position, visited)) {
            return true;
        }
    }

    return false;
}

bool Cell::hasPathTo(const Position& position, List<Position>& visited) {
    if (this->position == position) {
        return true;
    }

    if (visited.contains(this->position)) {
        return false;
    }

    for (size_t i = 0; i < this->dependsFrom.getLength(); i++) {
        Cell* cell = table->getAtPosition(this->dependsFrom[i]);
        
        if (cell == nullptr) {
            continue;
        }

        if (cell->hasPathTo(position, visited)) {
            return true;
        }
    }

    visited.add(this->position);
    return false;
}

void Cell::onChangeContent(const ChangeContentArgs& args) {
    for (size_t i = 0; i < this->dependantTo.getLength(); i++) {
        Cell* cellPtr = table->getAtPosition(dependantTo[i]);
        if (cellPtr == nullptr) {
            dependantTo.removeAt(i);
            i--;
            continue;
        }

        if (cellPtr->activeFunction != nullptr) {
            cellPtr->activeFunction(cellPtr, this, &args);
        }
    }
}

void Cell::addEdge(Cell* dependant, Cell* dependancy) {
    if (dependant == nullptr || dependancy == nullptr) {
        return;
    }

    if (!dependant->dependsFrom.contains(dependancy->getPosition())) {
        dependant->dependsFrom.add(dependancy->getPosition());
    }

    if (!dependancy->dependantTo.contains(dependant->getPosition())) {
        dependancy->dependantTo.add(dependant->getPosition());
    }
}

void Cell::removeEdge(Cell* dependant, Cell* dependancy) {
    if (dependant == nullptr || dependancy == nullptr) {
        return;
    }

    dependant->dependsFrom.remove(dependancy->getPosition());
    dependancy->dependantTo.remove(dependant->getPosition());
}


void Cell::changedReference(Cell* reciever, const Cell* sender, const ChangeContentArgs* args) {
    reciever->setCellDisplayAndType(sender->getDisplayContent(), sender->getCellType());
}

void Cell::removeOldEdges() {
    for (size_t i = 0; i < this->dependsFrom.getLength(); i++) {
        Cell::removeEdge(this, table->getAtPosition(this->dependsFrom[i]));
    }
 
    this->dependsFrom.clear();
    this->activeFunction = nullptr;
}

std::ofstream& operator<<(std::ofstream& stream, const Cell& cell) {
    if (!stream.good()) {
        return stream;
    }

    stream << cell.rawContent;
    return stream;
}

std::ifstream& operator>>(std::ifstream& stream, Cell& cell) {
    if (!stream.good()) {
        return stream;
    }

    MyString str;
    stream >> str;
    cell.setRawContent(str);

    return stream;
}
