#include "http/HttpServer.h"
#include "http/HttpHandler.h"
#include "http/Router.h"
#include "utils/NamedLogger.h"

extern "C" {
    #include "http/mongoose.h"
}


static NamedLogger s_logger("HttpServer");


HttpServer::HttpServer(int16_t port)
    :   _port(port),
        _isRunning(false)
{
    s_logger.Debug("[ThreadID: {}] HttpServer created", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())));
}


HttpServer::~HttpServer()
{
    Stop();
    s_logger.Debug("[ThreadID: {}] HttpServer destroyed", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())));
}


void HttpServer::SetRouter(const Router& router)
{
    _router = router;
}


void HttpServer::SetRoute(HttpMethod method, const std::string& path, HttpHandlerTask handler)
{
    _router.Register(method, path, handler);
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
        s_logger.Error("Failed to bind to port {}", _port);
        std::exit(1);
    }

    _conn->fn_data = &_router;

    s_logger.Info("Server setup at {}", addr);
}


void HttpServer::Run()
{
    s_logger.Info("Server is running");
    if (!_mgr || !_conn)
    {
        s_logger.Error("Server is not properly initialized");
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
        s_logger.Info("Server stopped");
    }
}


void HttpServer::RunAsync(TaskManager::Task task)
{
    s_logger.Info("RunAsync");
    // _taskManager.Submit(task);
}


void HttpServer::RunInMainLoop(TaskManager::Task task)
{
    s_logger.Info("RunInMainLoop");
    task();
}