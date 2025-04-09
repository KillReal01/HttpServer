#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>


class ThreadPool
{
public:
    explicit ThreadPool(size_t numThreads);
    ~ThreadPool();

    void Enqueue(std::function<void()> task);

private:
    size_t _numThreads;
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;
    std::mutex _queueMutex;
    std::condition_variable _condition;
    std::atomic<bool> _stop;
};


#endif // THREADPOOL_H