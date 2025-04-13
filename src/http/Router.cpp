#include "http/Router.h"
#include "http/HttpHandler.h"
#include "http/HttpServer.h"
#include "utils/NamedLogger.h"


static NamedLogger s_logger("Router");


void Router::Register(HttpMethod method, const std::string& path, HttpHandlerTask handler)
{
    s_logger.Info("Register route: {}", path);
    _routes.emplace_back(Route{method, path, std::move(handler)});
}



void Router::Handle(mg_connection* c, int ev, void* ev_data)
{
    if (ev != MG_EV_HTTP_MSG)
        return;

    mg_http_message* hm = static_cast<mg_http_message*>(ev_data);
    std::string method(hm->method.buf, hm->method.len);

    RequestContext ctx;
    ctx.conn = c;
    ctx.method = std::string(hm->method.buf, hm->method.len);
    ctx.uri = std::string(hm->uri.buf, hm->uri.len);

    for (const auto& route : _routes)
    {
        if ((route.method == HttpMethod::ANY || MethodMatches(route.method, method)) &&
            mg_match(hm->uri, mg_str(route.path.c_str()), nullptr))
        {
            s_logger.Info("Accepted from: {}, method: {}, uri: {}",
                    GetClientAddress(c),
                    std::string(hm->method.buf, hm->method.len),
                    std::string(hm->uri.buf, hm->uri.len));

            route.handler(c, hm);

            auto c_ptr = std::shared_ptr<mg_connection>(c, [](mg_connection* ptr) {});
            auto hm_ptr = std::shared_ptr<mg_http_message>(hm, [](mg_http_message* ptr) {});

            _server->RunAsync([this, ctx](){
                s_logger.Debug("[ThreadID: {}] Start RunAsync {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())), GetClientAddress(ctx.conn));
                std::this_thread::sleep_for(std::chrono::seconds(5));
                s_logger.Debug("[ThreadID: {}] Finish RunAsync {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())), GetClientAddress(ctx.conn));
            });
            return;
        }
    }

    HttpHandler::HandleError(c, hm); //
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