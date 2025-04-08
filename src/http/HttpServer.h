#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "Router.h"
#include <string>

class HttpServer
{
public: 
    static HttpServer& Get();
    ~HttpServer();

    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    void Init(Router* router);
    void Run();
    void Stop();

private:
    explicit HttpServer(int16_t port);
    void Setup();

private:
    int16_t _port;
    Router* _router;
    mg_mgr* _mgr;
    mg_connection* _conn;
    bool _isRunning;
};

#endif // HTTPSERVER_H