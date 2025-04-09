#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "http/Router.h"
#include "thread/TaskManager.h"


class HttpServer
{
public: 
    static HttpServer& Get();

    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    void SetRouter(Router* router);

    void Setup();
    void Run();
    void Stop();

	void SetPort(int16_t port);

private:
    explicit HttpServer(int16_t port = 80);
    ~HttpServer();
    
    

private:
    int16_t _port;
    Router* _router;
    mg_mgr* _mgr;
    mg_connection* _conn;
    bool _isRunning;
};

#endif // HTTPSERVER_H