#pragma once
#include "Public/TSQueue.h"


template <typename T>
class TSQueueBuffer {

private:
	std::vector<TSQueue<T*>>* _buffer;
	std::mutex _lock;
	bool _bufferRetrieved = false;

public:

	void InsertIntoBuffer(T* item) {

		_lock.lock();

		if (_bufferRetrieved) { //Create new queue into buffer and insert item if Queue has been retrieved

			TSQueue<T*>* queue = new TSQueue<T*>();
			_buffer->push_back(queue);

			_bufferRetrieved = false;
		}

		if (_buffer->empty()) { // Create new queue into buffer if buffer empty

			TSQueue<T*>* queue = new TSQueue<T*>();
			_buffer->push_back(queue);
		}

		_buffer->back()->Enqueue(item);


		_lock.unlock();
	}

	TSQueue<T*>* RetrieveBuffer() {

		_lock.lock();

		if (_buffer->begin() == _buffer->end()) { // If buffer is empty
			_lock.unlock();
			return nullptr;
		}

		TSQueue<T*>* queue = _buffer->front();

		_buffer->erase(_buffer->begin());

		_bufferRetrieved = true;

		_lock.unlock();

		return queue;
	}
};