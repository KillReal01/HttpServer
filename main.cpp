extern "C" {
    #include "http/mongoose.h"
}

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <mutex>

// Global log storage
std::vector<std::string> logs;
std::mutex log_mutex;

// Event handler function
// Remove the fn_data parameter
static void fn(struct mg_connection *c, int ev, void *ev_data)
{
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        // Extract URI from the HTTP message
        std::string uri(hm->uri.buf, hm->uri.len);
        
        // Log the accessed URI
        {
            std::lock_guard<std::mutex> lock(log_mutex);
            logs.emplace_back("Accessed: " + uri);
        }

        // Manually check the URI for "/info"
        if (uri == "/info") {
            mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Everything is OK");
        }
        // Manually check the URI for "/log"
        else if (uri == "/log") {
            std::string body;
            {
                // Lock the logs to safely read them
                std::lock_guard<std::mutex> lock(log_mutex);
                for (const auto& entry : logs) {
                    body += entry + "\n";
                }
            }
            mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "%s", body.c_str());
        }
        // Handle 404 for other URIs
        else {
            mg_http_reply(c, 404, "", "Not Found");
        }
    }
}

int main()
{
    // Check if the process is running as root
    if (geteuid() != 0) {
        printf("Process was started by a non-root user\n");
        return 1;
    }

    // Initialize Mongoose manager
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    // Start listening on port 1616
    mg_http_listen(&mgr, "http://0.0.0.0:1616", fn, NULL);

    printf("Server running at http://0.0.0.0:1616\n");

    // Main event loop
    while (true) {
        mg_mgr_poll(&mgr, 1000);
    }

    // Clean up Mongoose manager
    mg_mgr_free(&mgr);
    return 0;
}
