#ifndef __TQ_SERVER
#define __TQ_SERVER

#include <crow/app.h>

class Server {
    private:
        unsigned PORT=8080;
        crow::SimpleApp app;
    public:
        Server();
        void run();
        void setPort(unsigned);
};

#endif
