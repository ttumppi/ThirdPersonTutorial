// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "queue"
#include "mutex"

/**
 * 
 */

template<typename T>
class THIRDPERSONTUTORIAL_API TSQueue
{
public:
	TSQueue();
	
	void Enqueue(T* item) {
		_queueLock.lock();
		_items.push(item);
		_queueLock.unlock();
	}

	//Removes the first item and returns it
	//If queue is empty, returns NULL
	T* Dequeue() {

		if (Count() == 0) {
			return NULL;
		}

		_queueLock.lock();
		T* item = _items.front();
		_items.pop();
		_queueLock.unlock();
		return item;
	}

	int Count() {
		return _items.size();
	}


private:
	std::queue<T> _items;
	std::mutex _queueLock;
};
