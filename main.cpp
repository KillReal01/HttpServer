#include <stdio.h>
#include <unistd.h>

extern "C" {
    #include "http/mongoose.h"
}

#include "http/HttpHandler.h"
#include "http/HttpServer.h"
#include "http/Router.h"
#include "thread/TaskManager.h"
#include "utils/LoggerManager.h"
#include "utils/NamedLogger.h"


static NamedLogger s_logger("Main");


int main()
{
    LoggerManager::SetupLogger();
    s_logger.Debug("[ThreadID: {}]", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())));

    if (geteuid() != 0)
    {
        s_logger.Error("Process was started by a non-root user");
        return 1;
    }

    HttpServer server;
    server.SetPort(1616);

    server.SetRoute(HttpMethod::GET, "/info", HttpHandler::HandleInfo);
    server.SetRoute(HttpMethod::GET, "/log", HttpHandler::HandleLog);
    server.SetRoute(HttpMethod::POST, "/upload", HttpHandler::HandleUpload);

    server.Setup();
    server.Run();

    return 0;
}
