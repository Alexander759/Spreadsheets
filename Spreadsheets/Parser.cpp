#include "Parser.h"
#include "Position.h"
#include "RawContentType.h"

RawContentType Parser::getRawContentTypeFromInput(const MyString& str) {
    if (str == "") {
        return RawContentType::EmptyCell;
    }

    if (str.isBool()) {
        return RawContentType::Bool;
    }

    if (str.isDouble()) {
        return RawContentType::Number;
    }

    if (str[0] == '=' && str.indexOf('(') != -1 && str.indexOf('(') < str.indexOf(')')) {
        return RawContentType::Expression;
    }

    if (str[0] == '=') {
        return RawContentType::Reference;
    }

    if (str[0] == '"' && str[str.getLength() - 1] == '"') {
        return RawContentType::String;
    }

    return RawContentType::Error;
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
    if (Parser::getRawContentTypeFromInput(str) != RawContentType::Expression) {
        return MyString();
    }

    return str.subStr(str.indexOf('=') + 1, str.indexOf('(') - 1);
}

List<MyString> Parser::getArgumentsFromExpression(const MyString& str) {
    return str.subStr(str.indexOf('(') + 1, str.indexOf(')') - 1).removeFreeSpaces().split(',');
}

List<Position> Parser::getPositionsInRange(const MyString& str) {
    List<Position> result;

    if (Parser::getArgumentType(str) != ArgumentType::Range) {
        return result;
    }

    size_t separatorIndex = str.indexOf(':');
    Position pos1 = Position::fromString(str.subStr(0, separatorIndex - 1));
    Position pos2 = Position::fromString(str.subStr(separatorIndex + 1, str.getLength() -1));

    size_t rowStart = pos1.getRow() < pos2.getRow() ? pos1.getRow() : pos2.getRow();
    size_t rowEnd = pos1.getRow() > pos2.getRow() ? pos1.getRow() : pos2.getRow();
    size_t colStart = pos1.getCol() < pos2.getCol() ? pos1.getCol() : pos2.getCol();
    size_t colEnd = pos1.getCol() > pos2.getCol() ? pos1.getCol() : pos2.getCol();

    for (size_t i = rowStart; i <= rowEnd; i++) {
        for (size_t j = colStart; j <= colEnd; j++) {
            result.add(Position(i, j));
        }
    }

    return result;
}
