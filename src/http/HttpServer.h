#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "http/Router.h"
#include "thread/ThreadPool.h"


class HttpServer
{
public: 
    explicit HttpServer(int16_t port = 80);
    ~HttpServer();

    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    void SetRoute(HttpMethod method, const std::string& path, HttpHandlerTask handler);

    void Setup();
    void Run();
    void Stop();
    
    void SetPort(int16_t port);

    void RunAsync(Task task);
    void RunInMainLoop(Task task);
  
private:
    int16_t _port;
    mg_mgr* _mgr;
    mg_connection* _conn;
    bool _isRunning;

    Router _router;
    ThreadPool _threadPool;
};

#endif // HTTPSERVER_H