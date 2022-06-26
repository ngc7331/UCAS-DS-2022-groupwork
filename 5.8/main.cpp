#include <crow/json.h>
#include <iostream>
#include "API.h"
#include "const.h"
#include "server.h"
#include "type.h"

void help();
std::string EXE_NAME;

// use "extern" in other files
ALGO_TYPE algorithm;
std::map<int, std::string> city_list;
std::map<int, std::string> train_list;
std::map<int, std::string> plane_list;

int main(int argc, char **argv) {
    // init data
    EXE_NAME = *argv;
    enum {TERMINAL, SERVER} intr_mode = TERMINAL;
    Server server;
    algorithm = ALGO_DP;
    city_list = API::loadFile(DATA_PATH "city.json");
    train_list = API::loadFile(DATA_PATH "train.json");
    plane_list = API::loadFile(DATA_PATH "plane.json");
    for (auto &iter : city_list) {
        // DP::newCity(iter.first);
        Dijkstra::newCity(iter.first);
    }
    for (auto &iter : train_list) {
        crow::json::rvalue r = crow::json::load(iter.second);
        int a = r[1].i(), b = r[2].i(), t = r[3].i(), d = r[4].i(), c = r[5].i();
        // DP::newRoute(iter.first, TRAIN, a, b, t, d, c);
        Dijkstra::newRoute(iter.first, TRAIN, a, b, t, d, c);
    }
    for (auto &iter : plane_list) {
        crow::json::rvalue r = crow::json::load(iter.second);
        int a = r[1].i(), b = r[2].i(), t = r[3].i(), d = r[4].i(), c = r[5].i();
        // DP::newRoute(iter.first, TRAIN, a, b, t, d, c);
        Dijkstra::newRoute(iter.first, TRAIN, a, b, t, d, c);
    }

    // arg parse
    while (++argv, --argc) {
        std::string arg = *argv;
        if (arg == "-p" || arg == "--port") {
            argc --;
            argv ++;
            server.setPort(atoi(*argv));
        }
        else if (arg == "-t" || arg == "--terminal") intr_mode = TERMINAL;
        else if (arg == "-s" || arg == "--server") intr_mode = SERVER;
        else if (arg == "-h" || arg == "--help") {
            help();
            exit(0);
        }
        else {
            std::cout << "参数无效: "<< arg << std::endl;
            help();
            exit(1);
        }
    }

    std::vector<int> a;
    switch (intr_mode) {
    case TERMINAL:
        std::cout << "你启用了尚未完成的终端模式orz" << std::endl;
        // DEBUG:
        algorithm = ALGO_DIJK;
        a = API::search(0, 2, PLANE, COST);
        for (auto iter : a) {
            std::cout << iter << std::endl;
        }
        break;
    case SERVER:
        server.run();
        break;
    }

    return 0;
}

void help() {
    std::cout << "usage: '" << EXE_NAME << " [--terminal] [--server [--port] <port>] [--help]'" << std::endl;
    std::cout << "  --terminal 或无参数的情况下启用终端模式" << std::endl;
    std::cout << "  --server   启用服务器 + webui 模式，默认情况下使用8080端口" << std::endl;
    std::cout << "  --port     指定服务器端口，紧跟<port>为端口号，仅在使用--server时有效" << std::endl;
    std::cout << "  --help     显示帮助信息并退出" << std::endl;
    std::cout << "如参数有冲突，例如同时使用--server和--terminal，将以最后的为准" << std::endl;
    std::cout << "支持使用首字母缩写，例如-s等价于--server" << std::endl;
}
