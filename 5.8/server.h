#ifndef __TQ_SERVER
#define __TQ_SERVER

#include "const.h"
#include "type.h"
#include <crow/app.h>
#include <string>

class Server {
private:
    unsigned PORT = DEFAULT_PORT;
    crow::SimpleApp app;
    static crow::response respond(Status, std::string, std::string, std::string);

public:
    Server();
    void run();
    void setPort(unsigned);
};

#endif
