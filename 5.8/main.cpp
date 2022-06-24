#include <iostream>
#include "API.h"
#include "const.h"
#include "server.h"
#include "type.h"

void help();

// use "extern" in other files
ALGO_TYPE algorithm;
std::map<int, std::string> city_list;
std::map<int, std::string> train_list;
std::map<int, std::string> plane_list;

int main(int argc, char **argv) {
    // init data
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

    // arg parse
    while (argc --) {
        std::string arg = *argv;
        if (arg == "-p" || arg == "--port") {
            argc --;
            argv ++;
            server.setPort(atoi(*argv));
        };
        if (arg == "-t" || arg == "--terminal") intr_mode = TERMINAL;
        if (arg == "-s" || arg == "--server") intr_mode = SERVER;
        if (arg == "-h" || arg == "--help") {
            help();
            exit(0);
        };
        argv ++;
    }

    switch (intr_mode) {
    case TERMINAL:
        break;
    case SERVER:
        server.run();
        break;
    }

    return 0;
}

void help() {
    // TODO: print help message
}
