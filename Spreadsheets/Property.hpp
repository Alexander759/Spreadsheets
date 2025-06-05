#pragma once
#include "PropertyBase.h"
#include <stdexcept>
#include "Alignment.h"

template<typename T>
class Property : public virtual PropertyBase {
public:
	Property(const MyString& name, T* propertyValueHolder);
	~Property() = default;

	bool setFromString(const MyString& str) override;
	bool isStringValid(const MyString& str) override;

private:
	T parseValue(const MyString& str);

	T* propertyValueHolder;
};

template<typename T>
inline Property<T>::Property(const MyString& name, T* propertyValueHolder) : PropertyBase(name) {
	if (propertyValueHolder == nullptr) {
		throw std::invalid_argument("Nullptr can not be referenced");
	}
	
	this->propertyValueHolder = propertyValueHolder;
}

template<typename T>
inline bool Property<T>::setFromString(const MyString& str) {
	if (!isStringValid(str)) {
		this->setErrorFlag(true);
		this->setErrorMessage(MyString("ABORTING! ") + this->getName() + ":" + str + " - Invalid value");
		return false;
	}

	this->setErrorFlag(false);
	*propertyValueHolder = parseValue(str);
	return true;
}

template<typename T>
inline bool Property<T>::isStringValid(const MyString& str) {
	throw std::exception("Call only for implemented types");
}

template<>
inline bool Property<size_t>::isStringValid(const MyString& str) {
	return str.isSizeT() && str.toSizeT() > 0;
}

template<>
inline bool Property<bool>::isStringValid(const MyString& str) {
	return str.isBool();
}

template<>
inline bool Property<Alignment>::isStringValid(const MyString& str) {
	MyString strLower = str.toLower();

	return strLower == "left" || strLower == "center" || strLower == "right";
}

template<typename T>
inline T Property<T>::parseValue(const MyString& str) {
	throw std::exception("Call only for implemented types");
}

template<>
inline size_t Property<size_t>::parseValue(const MyString& str) {
	return str.toSizeT();
}

template<>
inline bool Property<bool>::parseValue(const MyString& str) {
	return str.toBool();
}

template<>
inline Alignment Property<Alignment>::parseValue(const MyString& str) {
	MyString strLower = str.toLower();

	if (strLower == "center") {
		return Alignment::Center;
	}
	else if (strLower == "right") {
		return Alignment::Right;
	}

	return Alignment::Left;
}
