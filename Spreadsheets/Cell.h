#pragma once
#include "MyString.h"
#include "CellType.h"
#include "Event.hpp"
#include "ChangeContentArgs.h"
#include "Position.h"

class Table;

class Cell {
public:
	Cell();

	friend class FormulasHolder;

	void setTable(Table* table);

	const Position& getPosition() const;
	void setPosition(const Position& position);

	const MyString& getRawContent() const;
	void setRawContent(const MyString& rawContent);

	const MyString& getDisplayContent() const;

	CellType getCellType() const;

	void setCellDisplayAndType(const MyString& displayContent, CellType type);

	List<Position>& getDependents();

private:

	static void parseRawContent(Cell& cell);
	static void handleBoolContent(Cell& cell);
	static void handleDoubleContent(Cell& cell);
	static void handleStringContent(Cell& cell);
	static void handleReferenceContent(Cell& cell);
	static void handleFormulaContent(Cell& cell);

	static void addEdge(Cell* reference, Cell* referenced);
	static void removeEdge(Cell* reference, Cell* referenced);

	static MyString errorStateMessage;

	bool hasPathTo(const Position& position);
	bool hasPathTo(const Position& position, List<Position>& visited);
	void onChangeContent(const ChangeContentArgs& args);
	static void changedReference(Cell* reciever, const Cell* sender, const ChangeContentArgs* args);
	void removeOldEdges();

	CellType type;
	void (*activeFunction)(Cell* receiver, const Cell* sender, const ChangeContentArgs* args);
	Table* table;
	Position position;
	MyString rawContent;
	MyString displayContent;
	List<Position> dependsFrom;
	List<Position> dependantTo;
};
