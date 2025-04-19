#pragma once
#include "CoreMinimal.h"
#include "mutex"
#include "vector"

template <typename T>
class TSVector {
public:

	TSVector TSVector(){}

	void Append(T item) {

	}



private:
	std::vector<T> _items;
	std::mutex _lock;
};