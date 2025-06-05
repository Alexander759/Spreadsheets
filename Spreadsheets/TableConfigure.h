#pragma once
#include "Alignment.h"
#include "MyString.h"

class TableConfigure {
public:
	TableConfigure(const MyString& fileName);

	bool hasError() const;
	const MyString& getErrorMessage() const;

	bool getAutoFit() const;
	bool getClearConsoleAfterCommand() const;
	Alignment getAlignment() const;
	size_t getInitialTableRows() const;
	size_t getInitialTableCols() const;
	size_t getMaxTableRows() const;
	size_t getMaxTableCols() const;
	size_t getVisibleCellSymbols() const;

private:
	bool error;

	bool autoFit;
	bool clearConsoleAfterCommand;
	Alignment alignment;
	size_t initialTableRows;
	size_t initialTableCols;
	size_t maxTableRows;
	size_t maxTableCols;
	size_t visibleCellSymbols;

	MyString errorMessage;
};