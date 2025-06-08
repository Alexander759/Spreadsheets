#pragma once
#include "MyString.h";
#include "CellType.h";


class ChangeContentArgs {
public:
	ChangeContentArgs(MyString previousContent, CellType previousCellType);

	const MyString& getPreviousContent() const;
	void setPreviousContent(MyString previousContent);

	CellType getPreviousCellType() const;
	CellType setPreviousCellType(CellType previousCellType);

private:
	MyString previousContent;
	CellType previousCellType;
};

