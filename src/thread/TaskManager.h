#ifndef TASKMANAGER_H
#define TASKMANAGER_H


#include <mutex>
#include <memory>
#include <functional>
#include <string>
#include <vector>

#include "http/Router.h"
#include "thread/ThreadPool.h"

extern "C" {
    #include "http/mongoose.h"
}

class TaskManager
{
public:

    using Task = std::function<void()>;

    explicit TaskManager(size_t threadPoolCapacity = std::thread::hardware_concurrency() - 1);
    ~TaskManager() = default;

    TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;

    void Submit(mg_connection* c, mg_http_message* hm, HttpHandlerTask task);

private:
    size_t _threadPoolCapacity;
    std::unique_ptr<ThreadPool> _threadPoolPtr;
};

#endif // TASKMANAGER_H
