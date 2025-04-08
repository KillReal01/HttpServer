#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

extern "C" {
    #include "mongoose.h"
}


class HttpHandler
{
public: 
    static void HandleInfo(mg_connection* c, mg_http_message* hm);
    static void HandleLog(mg_connection* c, mg_http_message* hm);
    static void HandleUpload(mg_connection* conn, mg_http_message* hm);
    
    static void EventHandler(mg_connection* c, int ev, void* ev_data);

};

#endif // HTTPHANDLER_H