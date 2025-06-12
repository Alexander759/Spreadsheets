#pragma once
#include "Cell.h"
#include "MyString.h"
#include "List.hpp"
#include "Formula.h"

class FormulasHolder {
public:
	static const List<Formula>& getFormulas();
	static void setFormulas();

	static void makeCellInvalid(Cell* cell);

	static void onRunSum(Cell* cell, const List<MyString>& args);
	static void onChangeSum(Cell* cell, const Cell* sender, const ChangeContentArgs* args);

	static void onRunAverage(Cell* cell, const List<MyString>& args);
	static void onChangeAverage(Cell* cell, const Cell* sender, const ChangeContentArgs* args);

	static void onRunMin(Cell* cell, const List<MyString>& args);
	static void onChangeMin(Cell* cell, const Cell* sender, const ChangeContentArgs* args);

	static void onRunMax(Cell* cell, const List<MyString>& args);
	static void onChangeMax(Cell* cell, const Cell* sender, const ChangeContentArgs* args);

	static void onRunLen(Cell* cell, const List<MyString>& args);
	static void onChangeLen(Cell* cell, const Cell* sender, const ChangeContentArgs* args);

	static void onRunConcat(Cell* cell, const List<MyString>& args);
	static void onChangeConcat(Cell* cell, const Cell* sender, const ChangeContentArgs* args);

	static void onRunSubstr(Cell* cell, const List<MyString>& args);
	static void onChangeSubstr(Cell* cell, const Cell* sender, const ChangeContentArgs* args);

	static void onRunCount(Cell* cell, const List<MyString>& args);
	static void onChangeCount(Cell* cell, const Cell* sender, const ChangeContentArgs* args);



private:
	static List<Formula> formulas;

	static bool isOneAndIsRange(const List<MyString>& args);

};