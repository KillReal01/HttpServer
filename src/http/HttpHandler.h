#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <functional>

extern "C" {
    #include "http/mongoose.h"
}

using HttpHandlerTask  = std::function<void(mg_connection*, mg_http_message*)>;
using Task = std::function<void()>;

class HttpHandler
{
public: 
    static void HandleInfo(mg_connection* c, mg_http_message* hm);
    static void HandleLog(mg_connection* c, mg_http_message* hm);
    static void HandleUpload(mg_connection* conn, mg_http_message* hm);
    static void HandleError(mg_connection* conn, mg_http_message* hm);
    
    static void EventHandler(mg_connection* c, int ev, void* ev_data);
};

#endif // HTTPHANDLER_H