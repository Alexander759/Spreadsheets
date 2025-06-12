#include "FormulasHolder.h"
#include "Parser.h"
#include "ArgumentType.h"
#include "Table.h"
#include "List.hpp"
#include "Formula.h"


List<Formula> FormulasHolder::formulas;

const List<Formula>& FormulasHolder::getFormulas() {
	return FormulasHolder::formulas;
}

void FormulasHolder::setFormulas() {
	FormulasHolder::formulas.add(Formula("SUM", onRunSum, onChangeSum));
	FormulasHolder::formulas.add(Formula("MIN", onRunMin, onChangeMin));
	FormulasHolder::formulas.add(Formula("MAX", onRunMax, onChangeMax));

}

void FormulasHolder::makeCellInvalid(Cell* cell) {
	cell->setCellDisplayAndType(Cell::errorStateMessage, CellType::Error);
}

void FormulasHolder::onRunSum(Cell* cell, const List<MyString>& args) {
	double result = 0;
	for (size_t i = 0; i < args.getLength(); i++) {
		ArgumentType type = Parser::getArgumentType(args[i]);

		switch (type) {
		case ArgumentType::Number:
			result += args[i].toDouble();
			break;
		case ArgumentType::Bool:
			result += args[i].toBool() ? 1 : 0;
			break;
		case ArgumentType::String:
			break;
		case ArgumentType::Reference:
			break;
		case ArgumentType::Range: {
			List<Position> positions = Parser::getPositionsInRange(args[i]);
			for (size_t i = 0; i < positions.getLength(); i++) {
				Cell* c = cell->table->getAtPosition(positions[i]);
				if (c->getCellType() == CellType::Number) {
					result += c->getDisplayContent().toDouble();
					Cell::addEdge(cell, c);
				}
				else if (c->getCellType() == CellType::Bool) {
					result += c->getDisplayContent().toBool() ? 1 : 0;
					Cell::addEdge(cell, c);
				}
				else if (c->getCellType() == CellType::Error) {
					makeCellInvalid(cell);
				}

				Cell::addEdge(cell, c);
			}
		}
								break;
		case ArgumentType::Invalid:
			makeCellInvalid(cell);
			return;
		default:
			break;

		}
	}

	cell->setCellDisplayAndType(MyString(result), CellType::Number);
}

void FormulasHolder::onChangeSum(Cell* cell, const Cell* sender, const ChangeContentArgs* args) {
	onRunSum(cell, Parser::getArgumentsFromExpression(cell->getRawContent()));
}

void FormulasHolder::onRunMin(Cell* cell, const List<MyString>& args) {
	if (args.getLength() != 1 || Parser::getArgumentType(args[0]) != ArgumentType::Range) {
		makeCellInvalid(cell);
		return;
	}

	List<Position> positions = Parser::getPositionsInRange(args[0]);

	Cell* currentMinPtr = nullptr;

	for (size_t i = 0; i < positions.getLength(); i++) {
		Cell* currentCellPtr = cell->table->getAtPosition(positions[i]);
		if (currentCellPtr == nullptr) {
			makeCellInvalid(cell);
			return;
		}

		Cell::addEdge(cell, currentCellPtr);
	}

	for (size_t i = 0; i < positions.getLength(); i++) {
		Cell* currentCellPtr = cell->table->getAtPosition(positions[i]);

		if (currentCellPtr == nullptr || currentCellPtr->getCellType() == CellType::Error) {
			makeCellInvalid(cell);
			return;
		}

		if (currentCellPtr->getCellType() == CellType::Number) {
			if (currentMinPtr == nullptr 
				|| currentMinPtr->getDisplayContent().toDouble() > currentCellPtr->getDisplayContent().toDouble()) {
				currentMinPtr = currentCellPtr;
			}
		}
	}

	if (currentMinPtr == nullptr) {
		makeCellInvalid(cell);
		return;
	}

	cell->setCellDisplayAndType(currentMinPtr->getDisplayContent(), CellType::Number);
}

void FormulasHolder::onChangeMin(Cell* cell, const Cell* sender, const ChangeContentArgs* args) {
	onRunMin(cell, Parser::getArgumentsFromExpression(cell->getRawContent()));
}

void FormulasHolder::onRunMax(Cell* cell, const List<MyString>& args) {
	if (args.getLength() != 1 || Parser::getArgumentType(args[0]) != ArgumentType::Range) {
		makeCellInvalid(cell);
		return;
	}

	List<Position> positions = Parser::getPositionsInRange(args[0]);

	Cell* currentMinPtr = nullptr;

	for (size_t i = 0; i < positions.getLength(); i++) {
		Cell* currentCellPtr = cell->table->getAtPosition(positions[i]);
		if (currentCellPtr == nullptr) {
			makeCellInvalid(cell);
			return;
		}

		Cell::addEdge(cell, currentCellPtr);
	}

	for (size_t i = 0; i < positions.getLength(); i++) {
		Cell* currentCellPtr = cell->table->getAtPosition(positions[i]);

		if (currentCellPtr == nullptr || currentCellPtr->getCellType() == CellType::Error) {
			makeCellInvalid(cell);
			return;
		}

		if (currentCellPtr->getCellType() == CellType::Number) {
			if (currentMinPtr == nullptr
				|| currentMinPtr->getDisplayContent().toDouble() < currentCellPtr->getDisplayContent().toDouble()) {
				currentMinPtr = currentCellPtr;
			}
		}
	}

	if (currentMinPtr == nullptr) {
		makeCellInvalid(cell);
		return;
	}

	cell->setCellDisplayAndType(currentMinPtr->getDisplayContent(), CellType::Number);
}

void FormulasHolder::onChangeMax(Cell* cell, const Cell* sender, const ChangeContentArgs* args) {
	onRunMax(cell, Parser::getArgumentsFromExpression(cell->getRawContent()));
}
