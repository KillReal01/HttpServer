#include "http/Router.h"
#include "http/HttpHandler.h"
#include "thread/TaskManager.h"
#include "utils/NamedLogger.h"


static NamedLogger s_logger("Router");


void Router::Register(HttpMethod method, const std::string& path, HttpHandlerTask handler)
{
    s_logger.Info("Register route: {}", path);
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
            s_logger.Debug("[ThreadID: {}]  Handle {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())), std::string(hm->uri.buf, hm->uri.len));
            s_logger.Info("Accepted: {} {}", std::string(hm->method.buf, hm->method.len), std::string(hm->uri.buf, hm->uri.len));
            TaskManager::Get().Submit(c, hm, route.handler);
            return;
        }
    }

    TaskManager::Get().Submit(c, hm, HttpHandler::HandleError);
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