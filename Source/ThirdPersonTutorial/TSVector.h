#pragma once
#include "CoreMinimal.h"
#include "mutex"
#include "vector"

template <typename T>
class TSVector {
public:

	TSVector TSVector(){}

	void Append(T* item) {
		_lock.lock();
		_items.push_back(item);
		_lock.unlock();

	}

	//Copies current vector to a new vector and then erases original's content
	std::vector<T*>* GetItems() {
		_lock.lock();
		std::vector<T*>* copy = new std::vector<T*>;
		copy->assign(_items.begin(), _items.end());

		_items.erase(_items.begin(), _items.end());
		_lock.unlock();
		return copy;
	}

	int Size() {
		return _items.size();
	}



private:
	std::vector<T*> _items;
	std::mutex _lock;
};