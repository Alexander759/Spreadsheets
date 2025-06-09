#include "Parser.h"
#include "Position.h"

CellType Parser::getCellTypeFromInput(const MyString& str) {
    if (str.isBool()) {
        return CellType::Bool;
    }

    if (str.isDouble()) {
        return CellType::Number;
    }

    if (str[0] == '=' && str.indexOf('(') != -1 && str.indexOf('(') < str.indexOf(')')) {
        return CellType::Reference;
    }

    if (str[0] == '=') {
        return CellType::Reference;
    }

    if (str[0] == '"' && str[str.getLength() - 1] == '"') {
        return CellType::String;
    }

    return CellType::Error;
}

ArgumentType Parser::getArgumentType(const MyString& str) {
    if (str.isBool()) {
        return ArgumentType::Bool;
    }
    
    if (str.isDouble()) {
        return ArgumentType::Number;
    }

    if (Position::isPosition(str)) {
        return ArgumentType::Reference;
    }

    if (str[0] == '"' && str[str.getLength() -1] == '"') {
        return ArgumentType::String;
    }
    
    size_t indexSeparator = str.indexOf(':');
    if (indexSeparator != -1 && indexSeparator != str.getLength() - 1) {
        if (Position::isPosition(str.subStr(0, indexSeparator - 1))
            && Position::isPosition(str.subStr(indexSeparator + 1, str.getLength() - 1))) {
            return ArgumentType::Range;
        }
    }

    return ArgumentType::Invalid;
}

MyString Parser::getExpressionName(const MyString& str) {
    if (Parser::getCellTypeFromInput(str) != CellType::Expression) {
        return MyString();
    }

    return str.subStr(str.indexOf('=') + 1, str.indexOf('(') - 1);
}

List<MyString> Parser::getArgumentsFromExpression(const MyString& str) {
    return str.subStr(str.indexOf('(') + 1, str.indexOf(')') - 1).split(',');
}
