#include <iostream>
#include "type.h"
#include "server.h"

// use "extern ALGO_TYPE algorithm" in other files
ALGO_TYPE algorithm;

int main(int argc, char **argv) {
    void help();
    Server server;

    // default settings
    enum {TERMINAL, SERVER} intr_mode = TERMINAL;
    algorithm = ALGO_DP;

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
