#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>


extern "C" {
    #include "mongoose.h"
}

#include "Router.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>


void HandleInfo(mg_connection* c, mg_http_message* hm)
{
    mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Everything is OK");
}


void HandleLog(mg_connection* c, mg_http_message* hm)
{
    mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Return log");
}


void HandleUpload(mg_connection* conn, mg_http_message* hm)
{
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
            mg_http_reply(conn, 200, "", "Uploaded to %s\n", filepath.c_str());
        } 
        else
        {
            mg_http_reply(conn, 500, "", "Failed to save file\n");
        }
    }
}


extern "C" void EventHandler(mg_connection* c, int ev, void* ev_data)
{
    Router* router = static_cast<Router*>(c->fn_data);
    if (router)
        router->Handle(c, ev, ev_data);
}
  

int main()
{
    if (geteuid() != 0)
    {
        spdlog::error("Process was started by a non-root user");
        return 1;
    }

    Router router;
    router.Register(HttpMethod::GET, "/info", HandleInfo);
    router.Register(HttpMethod::GET, "/log", HandleLog);
    router.Register(HttpMethod::POST, "/upload", HandleUpload);
  
    mg_mgr mgr;
    mg_mgr_init(&mgr);
  
    mg_connection* conn = mg_http_listen(&mgr, "http://0.0.0.0:1616", EventHandler, nullptr);
    if (!conn)
    {
        spdlog::error("Failed to bind to port");
        return 1;
    }

    conn->fn_data = &router;

    spdlog::info("Server running at http://0.0.0.0:1616");

    while (true)
    {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);
    return 0;
}
