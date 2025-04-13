#ifndef ROUTER_H
#define ROUTER_H

#include <functional>
#include <string>
#include <vector>

extern "C" {
    #include "http/mongoose.h"
}

#include "http/HttpHandler.h"


enum class HttpMethod { GET, POST, PUT, DELETE, ANY };

enum class HttpStatusCode : int
{
    OK                  = 200,
    BadRequest          = 400,
    Unauthorized        = 401,
    NotFound            = 404,
    InternalServerError = 500,
};

struct Route
{
    HttpMethod method;
    std::string path;
    HttpHandlerTask handler;
};

struct RequestContext
{
    mg_connection* conn;          // указатель на соединение (не копируется!)
    std::string method;
    std::string uri;
};

class HttpServer;

class Router
{
    friend class HttpServer;

public:
    explicit Router(HttpServer* server = nullptr) : _server(server) { }

    void Register(HttpMethod method, const std::string& path, HttpHandlerTask handler);
    void Handle(mg_connection* c, int ev, void* ev_data);

private:
    bool MethodMatches(HttpMethod m, const std::string& req) const;

    std::string GetClientAddress(mg_connection* c)
    {
        char ip[INET6_ADDRSTRLEN] = {};
        uint16_t port = 0;
    
        if (c->rem.is_ip6) {
            // IPv6
            memcpy(ip, c->rem.ip, 16);
            inet_ntop(AF_INET6, ip, ip, sizeof(ip));
            port = mg_ntohs(*(uint16_t *)&c->rem.port);
        } else {
            // IPv4
            memcpy(ip, c->rem.ip, 4);
            inet_ntop(AF_INET, ip, ip, sizeof(ip));
            port = mg_ntohs(*(uint16_t *)&c->rem.port);
        }
    
        return std::string(ip) + ":" + std::to_string(port);
    }
    

private:
    std::vector<Route> _routes;
    HttpServer* _server;
};


#endif // ROUTER_H