#include "Router.h"
#include <spdlog/spdlog.h>


void Router::Register(HttpMethod method, const std::string& path, Handler handler)
{
    spdlog::info("Register route: {}", path);
    routes.emplace_back(Route{method, path, std::move(handler)});
}


void Router::Handle(mg_connection* c, int ev, void* ev_data)
{
    if (ev != MG_EV_HTTP_MSG)
        return;

    mg_http_message* hm = static_cast<mg_http_message*>(ev_data);
    std::string method(hm->method.buf, hm->method.len);

    for (const auto& route : routes)
    {
        if ((route.method == HttpMethod::ANY || MethodMatches(route.method, method)) &&
            mg_match(hm->uri, mg_str(route.path.c_str()), nullptr))
        {
            spdlog::info("Accepted: {}", std::string(hm->uri.buf, hm->uri.len));
            route.handler(c, hm);
            return;
        }
    }

    mg_http_reply(c, static_cast<int>(HttpStatusCode::NotFound), "", R"({"error":"Not found"})");
}


bool Router::MethodMatches(HttpMethod m, const std::string& req) const
{
    switch (m)
    {
        case HttpMethod::GET:    return req == "GET";
        case HttpMethod::POST:   return req == "POST";
        case HttpMethod::PUT:    return req == "PUT";
        case HttpMethod::DELETE: return req == "DELETE";
        case HttpMethod::ANY:    return true;
    }
    return false;
}