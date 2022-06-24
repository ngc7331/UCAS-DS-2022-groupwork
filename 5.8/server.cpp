#include <crow/app.h>
#include <iostream>
#include "API.h"
#include "const.h"
#include "server.h"

#define assertParam(key) \
if (!req.url_params.get(#key)) \
    return crow::response(400, "Error: missing parameter: " #key); \
else \

#define getParamString(key) assertParam(key) key = req.url_params.get(#key)
#define getParamInt(key) assertParam(key) key = atoi(req.url_params.get(#key))

Server::Server() {
    /* --- root --- */
    CROW_ROUTE(app, "/")([]() {
        crow::response resp;
        resp.redirect("/user");
        return resp;
    });

    /* --- static page --- */
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

    /* --- data --- */
    CROW_ROUTE(app, "/" DATA_PATH "<string>")([](std::string f) {
        crow::response resp;
        resp.set_static_file_info(DATA_PATH + f);
        return resp;
    });

    /* --- API --- */
    CROW_ROUTE(app, "/api/v1/newCity")([](const crow::request& req) {
        std::string name;
        getParamString(name);
        Status code = API::newCity(name);
        return Server::respond(
            code,
            "City Created",
            code==ERR_VALUE ? "City" + name + "already existed" : "Internal Server Error, see log"
        );
    });

    CROW_ROUTE(app, "/api/v1/delCity")([](const crow::request& req) {
        int id;
        getParamInt(id);
        Status code = API::delCity(id);
        return Server::respond(code, "City Deleted", "No such city");
    });

    /* --- DEMO --- */
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
    Server::app.port(Server::PORT).multithreaded().run();
}

void Server::setPort(unsigned port) {
    if (port > 65535) return ;
    Server::PORT = port;
}

crow::response Server::respond(Status code, std::string msg, std::string err) {
    crow::response resp;

    switch (code) {
    case OK:
        resp.code = 200; break;
    case ERR_VALUE: case ERR_ASSERTION:
        resp.code = 400; break;
    default:
        resp.code = 500;
    }

    crow::json::wvalue body;
    body["code"] = code;
    body["msg"] = code == OK ? msg : "";
    body["err"] = code == OK ? "" : err;
    resp.body = body.dump();

    resp.set_header("content-type", "application/json");

    return resp;
}
