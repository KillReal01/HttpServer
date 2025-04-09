#include "thread/ThreadPool.h"
#include "utils/NamedLogger.h"


static NamedLogger s_logger("ThreadPool");


ThreadPool::ThreadPool(size_t numThreads)
	:	_numThreads(numThreads),
		_stop(false)
{
	s_logger.Debug("[ThreadID: {}] Number threads is {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())), _numThreads);

	for (size_t i = 0; i < numThreads; ++i)
	{
		_workers.emplace_back([this] {
			while (true)
			{
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(this->_queueMutex);
					this->_condition.wait(lock, [this] {
						return this->_stop || !this->_tasks.empty();
					});

					if (this->_stop && this->_tasks.empty())
						return;

					task = std::move(this->_tasks.front());
					this->_tasks.pop();
				}
				task();
			}
		});
	}
}

ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(_queueMutex);
		_stop = true;
	}
	_condition.notify_all();
	for (std::thread& worker : _workers)
		worker.join();
}

void ThreadPool::Enqueue(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(_queueMutex);
		if (_stop)
			throw std::runtime_error("enqueue on stopped ThreadPool");

		_tasks.push(std::move(task));
	}
	_condition.notify_one();
}