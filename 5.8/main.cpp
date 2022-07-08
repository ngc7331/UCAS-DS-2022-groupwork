#include <crow/json.h>
#include <iostream>
#include <map>
#include <string>
#include "API.h"
#include "const.h"
#include "server.h"
#include "terminal.h"
#include "type.h"

std::string EXE_NAME;

// use "extern" in other files
ALGO_TYPE algorithm;
std::map<int, std::string> city_list;
std::map<int, std::string> train_list;
std::map<int, std::string> plane_list;
bool DEBUGMODE;

int main(int argc, char **argv) {
    // init config
    EXE_NAME = *argv;
    enum {TERMINAL, SERVER} intr_mode = TERMINAL;
    Server server;
    algorithm = ALGO_DP;
    DEBUGMODE = false;

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
        else if (arg == "-d" || arg == "--debug") DEBUGMODE = true;
        else if (arg == "-h" || arg == "--help") {
            Terminal::help();
            exit(0);
        }
        else {
            std::cout << "参数无效: "<< arg << std::endl;
            Terminal::help();
            exit(1);
        }
    }

    // load data
    city_list = API::loadFile(DATA_PATH "city.json");
    train_list = API::loadFile(DATA_PATH "train.json");
    plane_list = API::loadFile(DATA_PATH "plane.json");
    for (auto &iter : city_list) {
        DP::newCity(iter.first);
        Dijkstra::newCity(iter.first);
    }
    for (auto &iter : train_list) {
        crow::json::rvalue r = crow::json::load(iter.second);
        int a = r[1].i(), b = r[2].i(), t = r[3].i(), d = r[4].i(), c = r[5].i();
        DP::newRoute(iter.first, TRAIN, a, b, t, d, c);
        Dijkstra::newRoute(iter.first, TRAIN, a, b, t, d, c);
    }
    for (auto &iter : plane_list) {
        crow::json::rvalue r = crow::json::load(iter.second);
        int a = r[1].i(), b = r[2].i(), t = r[3].i(), d = r[4].i(), c = r[5].i();
        DP::newRoute(iter.first, PLANE, a, b, t, d, c);
        Dijkstra::newRoute(iter.first, PLANE, a, b, t, d, c);
    }

    switch (intr_mode) {
    case TERMINAL:
        Terminal::run();
        break;
    case SERVER:
        server.run();
        break;
    }

    std::cout << "感谢您的使用，再见" << std::endl;

    return 0;
}
