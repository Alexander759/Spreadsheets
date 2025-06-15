#pragma once

#include <stdexcept>
#include <cmath>

template<typename T>
class List {
public:
	List();
	List(const List<T>& other);
	List(List<T>&& other);
	List<T>& operator=(const List<T>& other);
	List<T>& operator=(List<T>&& other);
	~List();

	void add(const T& item);
	void remove(const T& item);
	int indexOf(const T& item) const;
	bool contains(const T& item) const;
	void removeAt(size_t index);
	void clear();

	template<typename CallableType>
	T& FirstOrDefault(CallableType func);

	size_t getLength() const;
	bool isEmpty() const;

	const T& operator[](size_t index) const;
	T& operator[](size_t index);
private:
	void copyFrom(const List<T>& other);
	void free();
	void resizeUp();
	void resizeDown();
	void shrinkToFit();
	void resize(size_t newCapacity);

	T* content;
	size_t length;
	size_t capacity;

	static size_t defaultCapacity;
	static double resizeConstant;
};

template<typename T>
size_t List<T>::defaultCapacity = 8;

template<typename T>
double List<T>::resizeConstant = 1.5;

template<typename T>
inline List<T>::List() {
	this->capacity = List<T>::defaultCapacity;
	this->length = 0;
	this->content = new T[this->capacity];
}

template<typename T>
inline List<T>::List(const List<T>& other) {
	copyFrom(other);
}

template<typename T>
inline List<T>::List(List<T>&& other) {
	this->capacity = other.capacity;
	this->length = other.length;
	this->content = other.content;

	other.capacity = 0;
	other.length = 0;
	other.content = nullptr;
}

template<typename T>
inline List<T>& List<T>::operator=(const List<T>& other) {
	if (this != &other) {
		this->free();
		this->copyFrom(other);
	}

	return *this;
}

template<typename T>
inline List<T>& List<T>::operator=(List<T>&& other) {
	if (this != &other) {
		this->free();

		this->capacity = other.capacity;
		this->length = other.length;
		this->content = other.content;

		other.capacity = 0;
		other.length = 0;
		other.content = nullptr;
	}

	return *this;
}

template<typename T>
inline List<T>::~List() {
	this->free();
}

template<typename T>
inline void List<T>::add(const T& item) {
	if (this->length == this->capacity) {
		this->resizeUp();
	}

	this->content[this->length] = item;
	this->length++;
}

template<typename T>
inline void List<T>::remove(const T& item) {
	int index = this->indexOf(item);
	if (index != -1) {
		this->removeAt(index);
	}
}

template<typename T>
inline int List<T>::indexOf(const T& item) const {
	for (size_t i = 0; i < length; i++) {
		if (this->content[i] == item) {
			return i;
		}
	}

	return -1;
}

template<typename T>
inline bool List<T>::contains(const T& item) const {
	for (size_t i = 0; i < length; i++) {
		if (this->content[i] == item) {
			return true;
		}
	}

	return false;
}

template<typename T>
inline void List<T>::removeAt(size_t index) {
	if (index >= this->length) {
		throw std::out_of_range("Index was out of range");
	}

	for (size_t i = index; i < length - 1; i++) {
		this->content[i] = this->content[i + 1];
	}

	this->length--;

	if (this->length < static_cast<size_t>(this->capacity / List<T>::resizeConstant)) {
		resizeDown();
	}

}

template<typename T>
inline void List<T>::clear() {
	this->length = 0;
	this->shrinkToFit();
}

template<typename T>
inline size_t List<T>::getLength() const {
	return this->length;
}

template<typename T>
inline bool List<T>::isEmpty() const {
	return this->length == 0;
}

template<typename T>
inline const T& List<T>::operator[](size_t index) const {
	if (index >= this->length) {
		throw std::out_of_range("Index was out of range");
	}

	return this->content[index];
}

template<typename T>
inline T& List<T>::operator[](size_t index) {
	if (index >= this->length) {
		throw std::out_of_range("Index was out of range");
	}

	return this->content[index];
}

template<typename T>
inline void List<T>::copyFrom(const List<T>& other) {
	this->capacity = other.capacity;
	this->length = other.length;
	this->content = new T[this->capacity];

	for (size_t i = 0; i < this->length; i++) {
		this->content[i] = other.content[i];
	}
}
template<typename T>
inline void List<T>::free() {
	delete[] this->content;
	this->content = nullptr;
	this->length = 0;
	this->capacity = 0;
}

template<typename T>
inline void List<T>::resizeUp() {
	size_t newCapacity = static_cast<size_t>(this->capacity * List<T>::resizeConstant);
	resize(newCapacity);
}

template<typename T>
inline void List<T>::resizeDown() {
	size_t newCapacity = static_cast<size_t>(this->capacity / List<T>::resizeConstant);

	if (newCapacity < List<T>::defaultCapacity) {
		if (this->capacity == List<T>::defaultCapacity) {
			return;
		}

		newCapacity = List<T>::defaultCapacity;
	}

	resize(newCapacity);
}

template<typename T>
inline void List<T>::shrinkToFit() {
	size_t newCapacity = this->length < List<T>::defaultCapacity ? defaultCapacity : this->length;
	resize(newCapacity);
}

template<typename T>
inline void List<T>::resize(size_t newCapacity) {
	T* newContent = new T[newCapacity];

	for (size_t i = 0; i < length; i++) {
		newContent[i] = this->content[i];
	}

	delete[] this->content;
	this->content = newContent;
	this->capacity = newCapacity;
}

template<class T>
template<typename CallableType>
inline T& List<T>::FirstOrDefault(CallableType func) {
	if (this->length == 0) {
		throw std::runtime_error("List is empty");
	}

	for (size_t i = 0; i < length; i++) {
		if (func(this->content[i])) {
			return this->content[i];
		}
	}

	return this->content[0];
}
