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

    Router router;
    router.Register(HttpMethod::GET, "/info", HttpHandler::HandleInfo);
    router.Register(HttpMethod::GET, "/log", HttpHandler::HandleLog);
    router.Register(HttpMethod::POST, "/upload", HttpHandler::HandleUpload);

	HttpServer::Get().SetPort(1616);
    HttpServer::Get().SetRouter(&router);

    HttpServer::Get().Setup();
    HttpServer::Get().Run();

    return 0;
}
