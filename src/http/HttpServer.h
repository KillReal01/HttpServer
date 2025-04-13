#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "http/Router.h"
#include "thread/TaskManager.h"


class HttpServer
{
public: 
    explicit HttpServer(int16_t port = 80);
    ~HttpServer();

    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    void SetRouter(const Router& router);
    void SetRoute(HttpMethod method, const std::string& path, HttpHandlerTask handler);

    void Setup();
    void Run();
    void Stop();
    
    void SetPort(int16_t port);

private:
    void RunAsync(TaskManager::Task task);
    void RunInMainLoop(TaskManager::Task task);
  
private:
    int16_t _port;
    mg_mgr* _mgr;
    mg_connection* _conn;
    bool _isRunning;

    Router _router;
    TaskManager _taskManager;

};

#endif // HTTPSERVER_H