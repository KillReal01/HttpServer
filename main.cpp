#include <stdio.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

extern "C" {
    #include "mongoose.h"
}

#include "Router.h"
#include "Helper.h"
#include "HttpHandler.h"
#include "HttpServer.h"


int main()
{
    Helper::SetupLogger();

    if (geteuid() != 0)
    {
        spdlog::error("Process was started by a non-root user");
        return 1;
    }

    Router router;
    router.Register(HttpMethod::GET, "/info", HttpHandler::HandleInfo);
    router.Register(HttpMethod::GET, "/log", HttpHandler::HandleLog);
    router.Register(HttpMethod::POST, "/upload", HttpHandler::HandleUpload);

    HttpServer::Get().Init(&router);
    HttpServer::Get().Run();

    return 0;
}
