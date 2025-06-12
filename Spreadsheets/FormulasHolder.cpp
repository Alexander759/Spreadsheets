#include "FormulasHolder.h"
#include "Parser.h"
#include "ArgumentType.h"
#include "Table.h"
#include "List.hpp"
#include "Formula.h"


List<Formula> FormulasHolder::formulas;

const List<Formula>& FormulasHolder::getFormulas() {
	if (FormulasHolder::formulas.getLength() == 0) {
		setFormulas();
	}

	return FormulasHolder::formulas;
}

void FormulasHolder::setFormulas() {
	FormulasHolder::formulas.add(Formula("SUM", onRunSum, onChangeSum));
	FormulasHolder::formulas.add(Formula("AVERAGE", onRunAverage, onChangeAverage));
	FormulasHolder::formulas.add(Formula("MIN", onRunMin, onChangeMin));
	FormulasHolder::formulas.add(Formula("MAX", onRunMax, onChangeMax));
	FormulasHolder::formulas.add(Formula("LEN", onRunLen, onChangeLen));
	FormulasHolder::formulas.add(Formula("CONCAT", onRunConcat, onChangeConcat));
	FormulasHolder::formulas.add(Formula("SUBSTR", onRunSubstr, onChangeSubstr));
	FormulasHolder::formulas.add(Formula("COUNT", onRunCount, onChangeCount));
}

void FormulasHolder::makeCellInvalid(Cell* cell) {
	cell->setCellDisplayAndType(Cell::errorStateMessage, CellType::Error);
}

void FormulasHolder::onRunSum(Cell* cell, const List<MyString>& args) {
	double result = 0;
	bool atLeastOneValidValue = false;


	for (size_t i = 0; i < args.getLength(); i++) {
		ArgumentType type = Parser::getArgumentType(args[i]);
		switch (type) {
		case ArgumentType::Reference: {
			Cell* currentCell = cell->table->getAtPosition(Position::fromString(args[i]));
			if (currentCell == nullptr || currentCell->hasPathTo(cell->getPosition())) {
				makeCellInvalid(cell);
				return;
			}

			Cell::addEdge(cell, currentCell);

			break;
		}
		case ArgumentType::Range: {
			List<Position> positions = Parser::getPositionsInRange(args[i]);
			for (size_t i = 0; i < positions.getLength(); i++) {
				Cell* currentCell = cell->table->getAtPosition(positions[i]);
				if (currentCell == nullptr || currentCell->hasPathTo(cell->getPosition())) {
					makeCellInvalid(cell);
					return;
				}

				Cell::addEdge(cell, currentCell);
			}
			break;
		}
		default:
			break;

		}


	}

	for (size_t i = 0; i < args.getLength(); i++) {
		ArgumentType type = Parser::getArgumentType(args[i]);

		switch (type) {
		case ArgumentType::Number:
			result += args[i].toDouble();
			atLeastOneValidValue = true;
			break;
		case ArgumentType::Bool:
			result += args[i].toBool() ? 1 : 0;
			atLeastOneValidValue = true;
			break;
		case ArgumentType::String:
			break;
		case ArgumentType::Reference: {
			Cell* currentCell = cell->table->getAtPosition(Position::fromString(args[i]));
			if (currentCell->getCellType() == CellType::Error) {
				makeCellInvalid(cell);
				return;
			}
			else if (currentCell->getCellType() == CellType::Number) {
				result += currentCell->getDisplayContent().toDouble();
				atLeastOneValidValue = true;
			}
			else if (currentCell->getCellType() == CellType::Bool) {
				result += currentCell->getDisplayContent().toBool() ? 1 : 0;
				atLeastOneValidValue = true;
			}

			break;
		}
		case ArgumentType::Range: {
			List<Position> positions = Parser::getPositionsInRange(args[i]);
			for (size_t i = 0; i < positions.getLength(); i++) {
				Cell* currentCell = cell->table->getAtPosition(positions[i]);

				if (currentCell->getCellType() == CellType::Number) {
					result += currentCell->getDisplayContent().toDouble();
					atLeastOneValidValue = true;
				}
				else if (currentCell->getCellType() == CellType::Bool) {
					result += currentCell->getDisplayContent().toBool() ? 1 : 0;
					atLeastOneValidValue = true;
				}
				else if (currentCell->getCellType() == CellType::Error) {
					makeCellInvalid(cell);
					return;
				}
			}
			break;
		}
		case ArgumentType::Invalid:
			makeCellInvalid(cell);
			return;
		default:
			break;

		}
	}

	if (!atLeastOneValidValue) {
		makeCellInvalid(cell);
		return;
	}

	cell->setCellDisplayAndType(MyString(result), CellType::Number);
}

void FormulasHolder::onChangeSum(Cell* cell, const Cell* sender, const ChangeContentArgs* args) {
	onRunSum(cell, Parser::getArgumentsFromExpression(cell->getRawContent()));
}

void FormulasHolder::onRunAverage(Cell* cell, const List<MyString>& args) {
	double number = 0;
	double result = 0;

	for (size_t i = 0; i < args.getLength(); i++) {
		ArgumentType type = Parser::getArgumentType(args[i]);
		switch (type) {
		case ArgumentType::Reference: {
			Cell* currentCell = cell->table->getAtPosition(Position::fromString(args[i]));
			if (currentCell == nullptr || currentCell->hasPathTo(cell->getPosition())) {
				makeCellInvalid(cell);
				return;
			}

			Cell::addEdge(cell, currentCell);

			break;
		}
		case ArgumentType::Range: {
			List<Position> positions = Parser::getPositionsInRange(args[i]);
			for (size_t i = 0; i < positions.getLength(); i++) {
				Cell* currentCell = cell->table->getAtPosition(positions[i]);
				if (currentCell == nullptr || currentCell->hasPathTo(cell->getPosition())) {
					makeCellInvalid(cell);
					return;
				}


				Cell::addEdge(cell, currentCell);
			}
			break;
		}
		default:
			break;

		}


	}

	for (size_t i = 0; i < args.getLength(); i++) {
		ArgumentType type = Parser::getArgumentType(args[i]);

		switch (type) {
		case ArgumentType::Number:
			result += args[i].toDouble();
			number++;
			break;
		case ArgumentType::Bool:
			result += args[i].toBool() ? 1 : 0;
			number++;
			break;
		case ArgumentType::String:
			break;
		case ArgumentType::Reference: {
			Cell* currentCell = cell->table->getAtPosition(Position::fromString(args[i]));
			if (currentCell->getCellType() == CellType::Error) {
				makeCellInvalid(cell);
				return;
			}
			else if (currentCell->getCellType() == CellType::Number) {
				result += currentCell->getDisplayContent().toDouble();
				number++;
			}
			else if (currentCell->getCellType() == CellType::Bool) {
				result += currentCell->getDisplayContent().toBool() ? 1 : 0;
				number++;
			}

			break;
		}
		case ArgumentType::Range: {
			List<Position> positions = Parser::getPositionsInRange(args[i]);
			for (size_t i = 0; i < positions.getLength(); i++) {
				Cell* currentCell = cell->table->getAtPosition(positions[i]);

				if (currentCell->getCellType() == CellType::Number) {
					result += currentCell->getDisplayContent().toDouble();
					number++;
				}
				else if (currentCell->getCellType() == CellType::Bool) {
					result += currentCell->getDisplayContent().toBool() ? 1 : 0;
					number++;
				}
				else if (currentCell->getCellType() == CellType::Error) {
					makeCellInvalid(cell);
					return;
				}
			}
			break;
		}
		case ArgumentType::Invalid:
			makeCellInvalid(cell);
			return;
		default:
			break;

		}
	}

	if (number == 0) {
		makeCellInvalid(cell);
		return;
	}

	cell->setCellDisplayAndType(MyString(result / number), CellType::Number);
}

void FormulasHolder::onChangeAverage(Cell* cell, const Cell* sender, const ChangeContentArgs* args) {
	onRunAverage(cell, Parser::getArgumentsFromExpression(cell->getRawContent()));
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
		if (currentCellPtr == nullptr || currentCellPtr->hasPathTo(cell->getPosition())) {
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
	if (!isOneAndIsRange(args)) {
		makeCellInvalid(cell);
		return;
	}

	List<Position> positions = Parser::getPositionsInRange(args[0]);

	Cell* currentMinPtr = nullptr;

	for (size_t i = 0; i < positions.getLength(); i++) {
		Cell* currentCellPtr = cell->table->getAtPosition(positions[i]);
		if (currentCellPtr == nullptr || currentCellPtr->hasPathTo(cell->getPosition())) {
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

void FormulasHolder::onRunLen(Cell* cell, const List<MyString>& args) {
	if (args.getLength() != 1 || Parser::getArgumentType(args[0]) == ArgumentType::Range) {
		makeCellInvalid(cell);
		return;
	}

	if (Parser::getArgumentType(args[0]) == ArgumentType::Reference) {
		Cell* referencedCell = cell->table->getAtPosition(Position::fromString(args[0]));
		if (referencedCell == nullptr || referencedCell->hasPathTo(cell->getPosition())) {
			makeCellInvalid(cell);
			return;
		}

		Cell::addEdge(cell, referencedCell);
		cell->setCellDisplayAndType(referencedCell->getDisplayContent().getLength(), CellType::Number);
	}
	else {
		cell->setCellDisplayAndType(args[0].getLength(), CellType::Number);
	}
}

void FormulasHolder::onChangeLen(Cell* cell, const Cell* sender, const ChangeContentArgs* args) {
	cell->setCellDisplayAndType(sender->getDisplayContent().getLength(), CellType::Number);
}

void FormulasHolder::onRunConcat(Cell* cell, const List<MyString>& args) {
	if (args.getLength() != 2
		|| Parser::getArgumentType(args[0]) != ArgumentType::Range
		|| Parser::getArgumentType(args[1]) != ArgumentType::String) {
		makeCellInvalid(cell);
		return;
	}

	MyString delim = args[1].subStr(1, args[1].getLength() - 2);

	List<Position> pos = Parser::getPositionsInRange(args[0]);

	for (size_t i = 0; i < pos.getLength(); i++) {
		Cell* current = cell->table->getAtPosition(pos[i]);
		
		if (current == nullptr || current->hasPathTo(cell->getPosition())) {
			makeCellInvalid(cell);
			return;
		}

		Cell::addEdge(cell, current);
	}

	MyString result;
	for (size_t i = 0; i < pos.getLength(); i++) {
		Cell* current = cell->table->getAtPosition(pos[i]);

		if (current->getCellType() == CellType::Error) {
			makeCellInvalid(cell);
			return;
		}

		result += current->getDisplayContent();

		if (i != pos.getLength() - 1) {
			result += delim;
		}
	}

	cell->setCellDisplayAndType(result, CellType::String);
}

void FormulasHolder::onChangeConcat(Cell* cell, const Cell* sender, const ChangeContentArgs* args) {
	onRunConcat(cell, Parser::getArgumentsFromExpression(cell->getRawContent()));
}

void FormulasHolder::onRunSubstr(Cell* cell, const List<MyString>& args) {
	if (args.getLength() != 3
		|| Parser::getArgumentType(args[0]) == ArgumentType::Range
		|| !args[1].isSizeT()
		|| !args[2].isSizeT()) {
		makeCellInvalid(cell);
		return;
	}

	size_t startIndex = args[1].toSizeT();
	size_t length = args[2].toSizeT();

	if (length == 0) {
		makeCellInvalid(cell);
		return;
	}

	MyString argument = args[0];

	if (Parser::getArgumentType(args[0]) == ArgumentType::Reference) {
		Cell* referencedCell = cell->table->getAtPosition(Position::fromString(args[0]));
		
		if (referencedCell == nullptr || referencedCell->hasPathTo(cell->getPosition())) {
			makeCellInvalid(cell);
			return;
		}

		Cell::addEdge(cell, referencedCell);

		if (referencedCell->getCellType() == CellType::Error) {
			makeCellInvalid(cell);
			return;
		}

		argument = referencedCell->getDisplayContent();
	}

	size_t endIndex = startIndex + length;

	if (argument.getLength() < endIndex) {
		makeCellInvalid(cell);
		return;
	}

	MyString result = argument.subStr(startIndex, endIndex);
	cell->setCellDisplayAndType(result, CellType::String);
}

void FormulasHolder::onChangeSubstr(Cell* cell, const Cell* sender, const ChangeContentArgs* args) {
	onRunSubstr(cell, Parser::getArgumentsFromExpression(cell->getRawContent()));
}

void FormulasHolder::onRunCount(Cell* cell, const List<MyString>& args) {
	if (!isOneAndIsRange(args)) {
		makeCellInvalid(cell);
		return;
	}

	List<Position> positions = Parser::getPositionsInRange(args[0]);

	for (size_t i = 0; i < positions.getLength(); i++) {
		Cell* referencedCell = cell->table->getAtPosition(positions[i]);
		if (referencedCell == nullptr || referencedCell->hasPathTo(cell->getPosition())) {
			makeCellInvalid(cell);
			return;
		}

		Cell::addEdge(cell, referencedCell);
	}

	size_t result = 0;
	for (size_t i = 0; i < positions.getLength(); i++) {
		Cell* referencedCell = cell->table->getAtPosition(positions[i]);
		if (referencedCell->getCellType() == CellType::Error) {
			makeCellInvalid(cell);
			return;
		}

		if (referencedCell->getCellType() != CellType::EmptyCell) {
			result++;
		}
	}

	cell->setCellDisplayAndType(MyString(result), CellType::Number);
}

void FormulasHolder::onChangeCount(Cell* cell, const Cell* sender, const ChangeContentArgs* args) {
	onRunCount(cell, Parser::getArgumentsFromExpression(cell->getRawContent()));
}

bool FormulasHolder::isOneAndIsRange(const List<MyString>& args) {
	return args.getLength() == 1 && Parser::getArgumentType(args[0]) == ArgumentType::Range;
}
