#pragma once
#include "MyString.h"
#include "CellType.h"
#include "Event.hpp"
#include "ChangeContentArgs.h"

class Cell {
public:
	Cell();

	const MyString& getRawContent() const;
	void setRawContent(const MyString& rawContent);

	const MyString& getDisplayContent() const;
	void setDisplayContent(const MyString& displayContent);

	CellType getCellType() const;
	void setType(CellType type);

private:
	MyString rawContent;
	MyString displayContent;
	CellType type;
	Event<Cell, ChangeContentArgs> contentChanged;
};
