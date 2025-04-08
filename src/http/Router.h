#ifndef ROUTER_H
#define ROUTER_H

#include "mongoose.h"
#include <functional>
#include <string>
#include <vector>

enum class HttpMethod { GET, POST, PUT, DELETE, ANY };

using Handler = std::function<void(mg_connection*, mg_http_message*)>;

struct Route
{
    HttpMethod method;
    std::string path;
    Handler handler;
};

class Router
{
public:
    void Register(HttpMethod method, const std::string& path, Handler handler);
    void Handle(mg_connection* c, int ev, void* ev_data);

private:
    bool MethodMatches(HttpMethod m, const std::string& req) const;

private:
    std::vector<Route> routes;
};


#endif // ROUTER_H