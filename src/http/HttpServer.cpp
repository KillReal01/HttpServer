#include "http/HttpServer.h"
#include "http/HttpHandler.h"
#include "http/Router.h"
#include <spdlog/spdlog.h>

extern "C" {
    #include "http/mongoose.h"
}


HttpServer& HttpServer::Get()
{
    static HttpServer instance;
    return instance;
}


HttpServer::HttpServer(int16_t port)
    :   _port(port),
        _isRunning(false)
{
    spdlog::debug("HttpServer, ThreadID: {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())));
    spdlog::info("HttpServer created");
}


HttpServer::~HttpServer()
{
    Stop();
    spdlog::info("HttpServer destroyed");
}


void HttpServer::SetRouter(Router* router)
{
    _router = router;
}


void HttpServer::SetPort(int16_t port)
{
	_port = port;
}


void HttpServer::Setup()
{
    _mgr = new mg_mgr;
    mg_mgr_init(_mgr);

    std::string addr = "http://0.0.0.0:" + std::to_string(_port);
    _conn = mg_http_listen(_mgr, addr.c_str(), HttpHandler::EventHandler, nullptr);

    if (!_conn)
    {
        spdlog::error("Failed to bind to port {}", _port);
        std::exit(1);
    }

    _conn->fn_data = _router;

    spdlog::info("Server running at {}", addr);
}


void HttpServer::Run()
{
    if (!_mgr || !_conn)
    {
        spdlog::error("Server is not properly initialized");
        return;
    }

    _isRunning = true;
    while (_isRunning)
    {
        mg_mgr_poll(_mgr, 1000);
    }
}


void HttpServer::Stop()
{
    if (_mgr)
    {
        mg_mgr_free(_mgr);
        delete _mgr;
        _mgr = nullptr;
        _conn = nullptr;
        _isRunning = false;
        spdlog::info("Server stopped");
    }
}