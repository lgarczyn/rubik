//
// Created by Louis GARCZYNSKI on 4/30/16.
//

#pragma once

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

template <typename Cube, typename Value>
class ThreadPool {
	typedef std::function<Value(Cube)> Call;
	typedef std::unique_lock<std::mutex> Lock;

  public:
	ThreadPool(const ThreadPool &cpy);

	ThreadPool(int threadCount);
	~ThreadPool();

	std::vector<Value> run(Call call, const std::vector<Cube> &data);

  private:
	void waitForData(int i);
	void display(std::string str);

	bool isKill;

	Call _call;

	std::atomic<int> _dataCounter;
	std::atomic<int> _resetCounter;

	std::mutex _activeCounterMutex;
	int _activeCounter;

	std::mutex _condMutexFinished;     //mutex for _finished
	std::mutex _condMutexStarted;      //mutex for _started
	std::condition_variable _finished; //allow run to wait for threads
	std::condition_variable _started;  //allow threads to zait for run

	std::mutex _displayMutex;

	std::vector<Cube> _data;
	std::vector<Value> _values;
	std::vector<std::thread *> _threads;
};

template <typename Cube, typename Value>
void ThreadPool<Cube, Value>::display(std::string str) {
	(void)str;
	Lock lock(_displayMutex);
	std::cout << str << std::endl;
}

template <typename Cube, typename Value>
ThreadPool<Cube, Value>::ThreadPool(int threadCount) {
	isKill = false;

	//creates threads
	_resetCounter = 0;
	_threads.resize(threadCount);
	display("main thread creating children");
	for (int i = 0; i < threadCount; i++) {
		auto functor = std::bind(&ThreadPool::waitForData, this, i);
		_threads[i] = new std::thread(functor);
	}
}

template <typename Cube, typename Value>
ThreadPool<Cube, Value>::~ThreadPool() {
	//set destroyed flag to true
	isKill = true;

	_started.notify_all();

	//creates threads

	display("main thread deleting children");
	for (size_t i = 0; i < _threads.size(); i++) {
		_threads[i]->join();
		delete _threads[i];
	}
	display("main thread deleted children");
}

template <typename Cube, typename Value>
void ThreadPool<Cube, Value>::waitForData(int i) {
	display(std::to_string(i) + ": thread starts");
	while (1) {
		//wait for started thread
		{
			Lock lock(_condMutexStarted);

			_resetCounter++;

			display(std::to_string(i) + ": thread falls asleep");
			_started.wait(lock);

			display(std::to_string(i) + ": thread wake up");
		}
		//kill if thread started by destructor
		if (isKill) {
			display(std::to_string(i) + ": thread returns");
			return;
		}

		while (1) {
			//will contain the thread data/values index for this loop
			uint index = _dataCounter++;
			display(std::to_string(i) + ": thread assigned work id: " + std::to_string(index) + ", id is " + (index < _data.size() ? "correct" : "incorrect"));

			//if index is under data size
			// calculate data/values couple
			//else
			// set thread as finished
			if (index < _data.size()) {
				display(std::to_string(i) + ": thread starts working on id: " + std::to_string(index));
				_values[index] = _call(_data[index]);
				display(std::to_string(i) + ": thread ends work on id: " + std::to_string(index));
			} else {
				//set another thread as finished
				//if all of them are, notify main thread to stop waiting
				{
					Lock lock(_activeCounterMutex);

					_activeCounter--;

					display(std::to_string(i) + ": thread finished work, " + std::to_string(_activeCounter) + " left working");

					if (_activeCounter == 0)
						_finished.notify_one();
					if (_activeCounter < 0)
						throw std::logic_error("_activeCount < 0");
				}
				//could potentially be fired twice, but shouldn't be a problem
				//BUG possible, no notify
				break;
			}
		}
	}
}

template <typename Cube, typename Value>
std::vector<Value> ThreadPool<Cube, Value>::run(Call call, const std::vector<Cube> &data) {
	//loads data
	_call = call;
	_data = data;

	_values.resize(_data.size());
	_dataCounter = 0;
	_activeCounter = 0;

	//release every threads waiting start loop
	display("main thread loads " + std::to_string(_values.size()) + " elements with " + std::to_string(_threads.size()) + " threads.");
	display("main thread notifying start");
	_activeCounter = _threads.size();
	_started.notify_all();

	{
		//make main thread wait until notify_one is called from waitForData
		Lock lock(_condMutexFinished);
		display("main thread waiting for finish");
		_finished.wait(lock); //BUG possible: wait after notify
	}

	while ((uint)_resetCounter != _threads.size())
		;

	_resetCounter = 0;

	display("main thread data marked as finished");
	return _values;
}
