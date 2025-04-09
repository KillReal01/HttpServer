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
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
};


#endif // THREADPOOL_H