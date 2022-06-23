#include "API.h"
#include "type.h"
#include <vector>
#include <string>

extern ALGO_TYPE algorithm;

Status API::newCity(std::string name) {
    // TODO:
    // check if name exists
    // generate a new unique id for name
    int id = 0;
    // add city to data/city.json
    // if (algorithm == ALGO_DP)
    //     return DP::newCity(id);
    return Dijkstra::newCity(id);
}

Status API::delCity(int id) {
    // TODO:
    // delete city from data/city.json
    // if (algorithm == ALGO_DP)
    //     return DP::delCity(id);
    return Dijkstra::delCity(id);
}
