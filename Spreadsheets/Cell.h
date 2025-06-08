#pragma once
#include "MyString.h"
#include "CellType.h"
#include "Event.hpp"
#include "ChangeContentArgs.h"
#include "Position.h"

class Cell {
public:
	Cell();

	const Position& getPosition() const;
	void setPosition(const Position& position);

	const MyString& getRawContent() const;
	void setRawContent(const MyString& rawContent);

	const MyString& getDisplayContent() const;

	CellType getCellType() const;

	void setCellDisplayAndType(const MyString& displayContent, CellType type);

	Event<Cell, ChangeContentArgs>& getContentChangedEvent();
	List<Cell*>& getDependencies();

private:

	static void parseRawContent(Cell& cell);
	static void handleBoolContent(Cell& cell);
	static void handleDoubleContent(Cell& cell);
	static void handleStringContent(Cell& cell);
	static void handleReferenceContent(Cell& cell);
	static void handleFormulaContent(Cell& cell);

	void changedReference(Cell* sender, ChangeContentArgs* args);
	void unsubscribeFromEvents();

	Position position;
	MyString rawContent;
	MyString displayContent;
	CellType type;
	Event<Cell, ChangeContentArgs> contentChanged;
	List<Cell*> dependencies;

	void (*activeFunction)(Cell* sender, ChangeContentArgs* args);
};
