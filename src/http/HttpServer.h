#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "Router.h"

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
	void SetPort(int16_t port);

private:
    explicit HttpServer(int16_t port = 80);
    void Setup();

private:
    int16_t _port;
    Router* _router;
    mg_mgr* _mgr;
    mg_connection* _conn;
    bool _isRunning;
};

#endif // HTTPSERVER_H