#include "Reader.h"
#include <iostream>
#include <fstream>

List<List<MyString>> Reader::readProperties(const MyString& fileName) {
    std::ifstream stream(fileName.getCString(), std::ios::in);

    List<List<MyString>> result;

    if (!stream.is_open()) {
        return List<List<MyString>>();
    }

    while (true) {
        List<MyString> newProperty = Reader::readProperty(stream);
        if (stream.fail()) {
            break;
        }

        if (!newProperty.isEmpty()) {
            result.add(newProperty);
        }
    }

    return result;
}

List<MyString> Reader::readProperty(std::istream& stream) {
    MyString newProperty;
    stream >> newProperty;
    return newProperty.split(':');
}
