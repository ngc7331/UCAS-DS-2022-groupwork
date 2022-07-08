#include "server.h"
#include "API.h"
#include "const.h"
#include <crow/app.h>
#include <iostream>

#define assertParam(key)                                               \
    if (!req.url_params.get(#key))                                     \
        return crow::response(400, "Error: missing parameter: " #key); \
    else

#define getParamString(key) assertParam(key) key = req.url_params.get(#key)
#define getParamInt(key) assertParam(key) key = atoi(req.url_params.get(#key))

extern std::map<int, std::string> city_list;
extern std::map<int, std::string> train_list;
extern std::map<int, std::string> plane_list;

Server::Server() {
    /* --- static file --- */
    CROW_ROUTE(app, "/")
    ([]() {
        crow::response resp;
        resp.set_static_file_info(STATIC_FILE_PATH "index.html");
        return resp;
    });

    CROW_ROUTE(app, "/css/<path>")
    ([](std::string f) {
        crow::response resp;
        resp.set_static_file_info(STATIC_FILE_PATH "css/" + f);
        return resp;
    });

    CROW_ROUTE(app, "/js/<path>")
    ([](std::string f) {
        crow::response resp;
        resp.set_static_file_info(STATIC_FILE_PATH "js/" + f);
        return resp;
    });

    /* --- data --- */
    CROW_ROUTE(app, "/data/<string>")
    ([](std::string f) {
        crow::response resp;
        resp.set_static_file_info(DATA_PATH + f);
        return resp;
    });

    /* --- API --- */
    CROW_ROUTE(app, "/api/v1/newCity")
    ([](const crow::request& req) {
        std::string name;
        getParamString(name);
        Status code = API::newCity(name);
        return Server::respond(code, "", "City Created",
                               code == ERR_VALUE ? "City " + name + " already existed"
                                                 : "Internal Server Error, see log");
    });

    CROW_ROUTE(app, "/api/v1/delCity")
    ([](const crow::request& req) {
        int id;
        getParamInt(id);
        Status code = API::delCity(id);
        return Server::respond(code, "", "City Deleted", "No such city");
    });

    CROW_ROUTE(app, "/api/v1/newRoute")
    ([](const crow::request& req) {
        std::string name;
        getParamString(name);
        int tp, a, b, t, d, c;
        getParamInt(tp);
        getParamInt(a);
        getParamInt(b);
        getParamInt(t);
        getParamInt(d);
        getParamInt(c);
        Status code = API::newRoute(name, tp, a, b, t, d, c);
        return Server::respond(code, "", "Route created", "");
    });

    CROW_ROUTE(app, "/api/v1/delRoute")
    ([](const crow::request& req) {
        int id, tp;
        getParamInt(id);
        getParamInt(tp);
        Status code = API::delRoute(id, tp);
        return Server::respond(code, "", "Route deleted", "");
    });

    CROW_ROUTE(app, "/api/v1/setAlgo")
    ([](const crow::request& req) {
        int tp;
        getParamInt(tp);
        Status code = API::setAlgo(tp);
        return Server::respond(code, "", "Success", "");
    });

    CROW_ROUTE(app, "/api/v1/search")
    ([](const crow::request& req) {
        int a, b, tp, policy;
        getParamInt(a);
        getParamInt(b);
        getParamInt(tp);
        getParamInt(policy);
        std::vector<int> res = API::search(a, b, tp, policy);
        std::string msg, err;
        crow::json::wvalue data;
        Status code = OK;
        if (res.size() == 0) {
            err = "没有找到符合要求的路径";
            code = ERR_VALUE;
        } else if (res[0] == -1) {
            err = "输入无效或内部错误";
            code = ERR;
        } else {
            int cost = 0, duration = 0, trip_duration = 0, interchange = 0;
            std::map<int, std::string>* list = tp == TRAIN ? &train_list : &plane_list;
            std::string lastname;
            crow::json::rvalue r;
            int laststart, d;
            for (int i = 0; i < res.size(); i++) {
                r = crow::json::load((*list)[res[i]]);
                cost += r[5].i();
                trip_duration += r[4].i();
                if (i != 0) {
                    d = r[3].i() - laststart;
                    if (d <= 0)
                        msg = "本路径假设每天都有相同的车次，可能需要在某城市等到次日";
                    while (d <= 0)
                        d += 1440; // the next day
                    duration += d;
                }
                if (i != 0 && r[0].s() != lastname)
                    interchange++;
                lastname = r[0].s();
                laststart = r[3].i();
            }
            data["path"] = res;
            data["cost"] = cost;
            data["duration"] = duration + r[4].i();
            data["trip_duration"] = trip_duration;
            data["interchange"] = interchange;
            data["tp"] = tp;
        }
        return Server::respond(code, data.dump(), msg, err);
    });
}

void Server::run() {
    std::cout << "=== WebUI 模式启动 ===" << std::endl;
    std::cout << "请用浏览器访问 http://127.0.0.1:" << Server::PORT << std::endl;
    std::cout << "请按 Ctrl + C 退出程序" << std::endl;
    std::cout << "=== 服务器日志 ===" << std::endl;
    Server::app.port(Server::PORT).multithreaded().run();
}

void Server::setPort(unsigned port) {
    if (port > 65535)
        return;
    Server::PORT = port;
}

crow::response Server::respond(Status code, std::string data, std::string msg, std::string err) {
    crow::response resp;

    switch (code) {
    case OK:
        resp.code = 200;
        break;
    case ERR_VALUE:
    case ERR_ASSERTION:
        resp.code = 400;
        break;
    default:
        resp.code = 500;
        err += "\n===\n程序内部错误，可能出现数据不一致的问题，请重启服务端\n";
        break;
    }

    crow::json::wvalue body;
    body["code"] = code;
    body["data"] = data;
    body["msg"] = msg;
    body["err"] = err;
    resp.body = body.dump();

    resp.set_header("content-type", "application/json");

    return resp;
}
