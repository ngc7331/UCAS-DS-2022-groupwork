#include <iostream>
#include "server.h"

#define assertParam(key) \
if (!req.url_params.get(#key)) \
    return crow::response(400, "Error: missing parameter: " #key); \
else \

#define getParamString(key) assertParam(key) key = req.url_params.get(#key)
#define getParamInt(key) assertParam(key) key = atoi(req.url_params.get(#key))

#define STATIC_FILE_PATH "static/"

Server::Server() {
    CROW_ROUTE(app, "/")([]() {
        crow::response resp;
        resp.redirect("/user");
        return resp;
    });

    CROW_ROUTE(app, "/user")([]() {
        crow::response resp;
        resp.set_static_file_info(STATIC_FILE_PATH "user.html");
        return resp;
    });

    CROW_ROUTE(app, "/admin")([]() {
        crow::response resp;
        resp.set_static_file_info(STATIC_FILE_PATH "admin.html");
        return resp;
    });

    // DEMO: string arg
    CROW_ROUTE(app, "/hello")([](const crow::request& req) {
        std::string name;
        getParamString(name);
        return crow::response(200, "Hello " + name + "! ");
    });

    // DEMO: int arg
    CROW_ROUTE(app, "/add")([](const crow::request& req) {
        int a, b;
        getParamInt(a);
        getParamInt(b);
        return crow::response(200, std::to_string(a + b));
    });
}

void Server::run() {
    std::cout << "Running at server mode, press Ctrl+C to exit" << std::endl;
    app.port(PORT).multithreaded().run();
}

void Server::setPort(unsigned port) {
    if (port > 65535) return ;
    Server::PORT = port;
}
