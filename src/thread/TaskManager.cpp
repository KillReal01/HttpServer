#include "thread/TaskManager.h"
#include "utils/NamedLogger.h"

#include <string>
#include <cstring>


static NamedLogger s_logger("TaskManager");


TaskManager::TaskManager(size_t threadPoolCapacity)
    :   _threadPoolCapacity(threadPoolCapacity),
        _threadPoolPtr(std::make_unique<ThreadPool>(_threadPoolCapacity))
{ }


TaskManager& TaskManager::Get()
{
    static TaskManager instance;
    return instance;
}


void TaskManager::Submit(mg_connection* c, mg_http_message* hm, HttpHandlerTask task)
{
    std::string method(hm->method.buf, hm->method.len);
    std::string uri(hm->uri.buf, hm->uri.len);
    std::string body(hm->body.buf, hm->body.len);

    s_logger.Debug("[ThreadID: {}] Submit {} {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())), method, uri);

    _threadPoolPtr->Enqueue([=]() {
        task(c, hm);
        // mg_http_message* cloned = new mg_http_message {0};
        
        // cloned->method.buf = new char[method.size() + 1];
        // memcpy(cloned->method.buf, method.c_str(), method.size());
        // cloned->method.len = method.size();
    
        // cloned->uri.buf = new char[uri.size() + 1];
        // memcpy(cloned->uri.buf, uri.c_str(), uri.size());
        // cloned->uri.len = uri.size();
    
        // cloned->body.buf = new char[body.size() + 1];
        // memcpy(cloned->body.buf, body.c_str(), body.size());
        // cloned->body.len = body.size();
    
        // task(c, cloned);
    
        // delete[] cloned->method.buf;
        // delete[] cloned->uri.buf;
        // delete[] cloned->body.buf;
        // delete cloned;
    });
}