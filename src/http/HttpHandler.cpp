#include "http/HttpHandler.h"
#include "http/Router.h"
#include "utils/LoggerManager.h"

#include <string>
#include <sstream>
#include <fstream>
#include <spdlog/spdlog.h>


void HttpHandler::HandleInfo(mg_connection* c, mg_http_message* hm)
{
    spdlog::debug("HandleInfo, ThreadID: {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())));
    mg_http_reply(c, static_cast<int>(HttpStatusCode::OK), "Content-Type: text/plain\r\n", "Everything is OK");
}


void HttpHandler::HandleError(mg_connection* c, mg_http_message* hm)
{
    spdlog::debug("HandleError, ThreadID: {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())));
    mg_http_reply(c, static_cast<int>(HttpStatusCode::NotFound), "Content-Type: text/plain\r\n", "Not found");
}


void HttpHandler::HandleLog(mg_connection* c, mg_http_message* hm)
{
    spdlog::debug("HandleLog, ThreadID: {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())));

    std::string path = LoggerManager::GetLogFilename();
    std::ifstream logFile(path, std::ios::in | std::ios::binary);
    
    if (!logFile.is_open())
    {
        spdlog::error("Failed to open log file: {}", path);
        mg_http_reply(c, static_cast<int>(HttpStatusCode::InternalServerError), "Content-Type: text/plain\r\n", "Failed to open log file");
        return;
    }

    mg_http_reply(c, static_cast<int>(HttpStatusCode::OK),
                  "Content-Type: text/plain; charset=utf-8\r\nTransfer-Encoding: chunked\r\n", "");

    const size_t bufferSize = 8192;
    char buffer[bufferSize];

    while (!logFile.eof())
    {
        logFile.read(buffer, bufferSize);
        size_t bytesRead = logFile.gcount();

        if (bytesRead > 0)
        {
            char chunkHeader[32];
            int headerSize = snprintf(chunkHeader, sizeof(chunkHeader), "%zx\r\n", bytesRead);
            mg_send(c, chunkHeader, headerSize);
            mg_send(c, buffer, bytesRead);
            mg_send(c, "\r\n", 2);
        }
    }

    mg_send(c, "0\r\n\r\n", 5);

    logFile.close();
    spdlog::info("Log file sent to client");
}


void HttpHandler::HandleUpload(mg_connection* conn, mg_http_message* hm)
{
    spdlog::debug("HandleUpload, ThreadID: {}", static_cast<size_t>(std::hash<std::thread::id>{}(std::this_thread::get_id())));

    mg_http_part part;
    size_t pos = 0;

    while ((pos = mg_http_next_multipart(hm->body, pos, &part)) > 0)
    {
        std::string filename(part.filename.buf, part.filename.len);
        std::string filepath = "/tmp/" + filename;
        std::string filedata(part.body.buf, part.body.len);

        std::ofstream out(filepath, std::ios::binary);
        if (out.is_open())
        {
            out.write(filedata.data(), filedata.size());
            out.close();
            spdlog::info("Saved upload to {}", filepath);
            mg_http_reply(conn, static_cast<int>(HttpStatusCode::OK), "", "Uploaded to %s\n", filepath.c_str());
        } 
        else
        {
            mg_http_reply(conn, static_cast<int>(HttpStatusCode::InternalServerError), "", "Failed to save file\n");
        }
    }
}


void HttpHandler::EventHandler(mg_connection* c, int ev, void* ev_data)
{
    Router* router = static_cast<Router*>(c->fn_data);
    if (router)
        router->Handle(c, ev, ev_data);
}
  