#pragma once
#include "MyString.h"
#include "CellType.h"


class ChangeContentArgs {
public:
	ChangeContentArgs(const MyString& previousContent, CellType previousCellType);

	const MyString& getPreviousContent() const;
	void setPreviousContent(MyString previousContent);

	CellType getPreviousCellType() const;
	void setPreviousCellType(CellType previousCellType);

private:
	MyString previousContent;
	CellType previousCellType;
};

