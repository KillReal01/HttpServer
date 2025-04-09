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

class Router
{
public:
    void Register(HttpMethod method, const std::string& path, HttpHandlerTask handler);
    void Handle(mg_connection* c, int ev, void* ev_data);

private:
    bool MethodMatches(HttpMethod m, const std::string& req) const;

private:
    std::vector<Route> routes;
};


#endif // ROUTER_H