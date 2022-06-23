#ifndef __TQ_SERVER
#define __TQ_SERVER

#include <crow/app.h>
#include <string>
#include "type.h"

class Server {
    private:
        unsigned PORT=8080;
        crow::SimpleApp app;
        static crow::response respond(Status code, std::string msg, std::string err);
    public:
        Server();
        void run();
        void setPort(unsigned);
};

#endif
