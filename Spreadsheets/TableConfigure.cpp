#include "TableConfigure.h"
#include "Reader.h"
#include "PropertyBase.h"
#include "Property.hpp"
#include <iostream>

TableConfigure::TableConfigure(const MyString& fileName) {
    this->error = false;
    List<List<MyString>> propertiesInString = Reader::readProperties(fileName);

    List<PropertyBase*> props;
    Property<size_t> initialTableRows("initialTableRows", &this->initialTableRows);
    Property<size_t> initialTableCols("initialTableCols", &this->initialTableCols);
    Property<size_t> maxTableRows("maxTableRows", &this->maxTableRows);
    Property<size_t> maxTableCols("maxTableCols", &this->maxTableCols);
    Property<bool> autoFit("autoFit", &this->autoFit);
    Property<size_t> visibleCellSymbols("visibleCellSymbols", &this->visibleCellSymbols);
    Property<Alignment> initialAlignment("initialAlignment", &this->alignment);
    Property<bool> clearConsoleAfterCommand("clearConsoleAfterCommand", &this->clearConsoleAfterCommand);

    props.add(&initialTableRows);
    props.add(&initialTableCols);
    props.add(&maxTableRows);
    props.add(&maxTableCols);
    props.add(&autoFit);
    props.add(&visibleCellSymbols);
    props.add(&initialAlignment);
    props.add(&clearConsoleAfterCommand);

    for (size_t i = 0; i < propertiesInString.getLength(); i++) {
        if (propertiesInString[i].getLength() < 2) {
            if (propertiesInString[i].getLength() == 0) {
                continue;
            }

            PropertyBase*& prop = props.FirstOrDefault([propertiesInString, i](PropertyBase* prop)
                -> bool {return prop->getName() == propertiesInString[i][0]; });

            if (prop->getName() == propertiesInString[i][0]) {
                prop->setErrorFlag(true);
                prop->setErrorMessage("Invalid value");
            }
                
            continue;
        }

        for (size_t j = 0; j < propertiesInString[i].getLength(); j++) {
            PropertyBase*& prop = props.FirstOrDefault([propertiesInString, i](PropertyBase* prop)
                -> bool {return prop->getName() == propertiesInString[i][0]; });

            if (prop->getName() == propertiesInString[i][0]) {
                prop->setFromString(propertiesInString[i][1]);
            }
        }
    }

    for (size_t i = 0; i < props.getLength(); i++) {
        if (props[i]->hasError()) {
            this->errorMessage = props[i]->getErrorMessage();
            this->error = true;
        }
    }
}

bool TableConfigure::hasError() const {
    return this->error;
}

const MyString& TableConfigure::getErrorMessage() const {
    return this->errorMessage;
}

bool TableConfigure::getAutoFit() const {
    return this->autoFit;
}

bool TableConfigure::getClearConsoleAfterCommand() const {
    return this->clearConsoleAfterCommand;
}

Alignment TableConfigure::getAlignment() const {
    return this->alignment;
}

size_t TableConfigure::getInitialTableRows() const {
    return this->initialTableRows;
}

size_t TableConfigure::getInitialTableCols() const {
    return this->initialTableCols;
}

size_t TableConfigure::getMaxTableRows() const {
    return this->maxTableRows;
}

size_t TableConfigure::getMaxTableCols() const {
    return this->maxTableCols;
}

size_t TableConfigure::getVisibleCellSymbols() const {
    return this->visibleCellSymbols;
}
