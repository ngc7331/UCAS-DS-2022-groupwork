#include "API.h"
#include "const.h"
#include "type.h"
#include <crow/json.h>
#include <fstream>
#include <string>
#include <map>

#define __newId(ls) if (ls.size()) return ls.rbegin()->first + 1; else return 0;

extern ALGO_TYPE algorithm;
extern std::map<int, std::string> city_list;
extern std::map<int, std::string> train_list;
extern std::map<int, std::string> plane_list;

// desp:   generate a new id
// args:   int id_type
// return: int id
int newId(int tp) {
    switch (tp) {
    case 0: __newId(city_list);
    case 1: __newId(train_list);
    case 2: __newId(plane_list);
    default: return 0;
    }
}

int API::getCityId(std::string name) {
    for (auto &iter : city_list)
        if (iter.second == name)
            return iter.first;
    return -1;
}

std::map<int, std::string> API::loadFile(std::string path) {
    std::ifstream rf(path);
    std::stringstream buffer;
    buffer << rf.rdbuf();
    crow::json::rvalue r = crow::json::load(buffer.str());
    std::map<int, std::string> dst;
    for (auto key : r.keys()) {
        crow::json::wvalue w = r[key];
        std::string s = w.dump();
        if (s[0] == '"')
            s = s.substr(1, s.length()-2);
        dst[atoi(key.c_str())] = s;
    }
    return dst;
}

Status API::saveFile(std::string path, std::map<int, std::string> src) {
    std::ofstream wf(path);
    std::stringstream buffer;
    buffer << "{" << std::endl;
    for (auto iter=src.begin(); iter!=src.end(); ) {
        buffer << "    \"" << iter->first << "\": ";
        if (iter->second[0] == '[')
            buffer << crow::json::load(iter->second);
        else
            buffer << "\"" << iter->second << "\"";
        if (++iter != src.end())
            buffer << ",";
        buffer << std::endl;
    }
    buffer << "}" << std::endl;
    wf << buffer.str();
    return OK;
}

Status API::setAlgo(int tp) {
    switch (tp) {
    case 0: algorithm = ALGO_DP; break;
    case 1: algorithm = ALGO_DIJK; break;
    default: return ERR_VALUE;
    }
    return OK;
}

bool API::isSameTrain(int a, int b) {
    if (!train_list.size() || a > train_list.rbegin()->first || b > train_list.rbegin()->first) {
        std::cout << "API::isSameTrain() Warning: out of range(a=" << a << ", b=" << b << ", last=" << (train_list.size() ? train_list.rbegin()->first : 0) << ")";
        std::cout << "this may be caused by a program error or invalid data" << std::endl;
        return false;
    }
    crow::json::rvalue ra = crow::json::load(train_list[a]);
    crow::json::rvalue rb = crow::json::load(train_list[b]);
    return ra[0].s() == rb[0].s();
}

Status API::newCity(std::string name) {
    // check if name exists
    if (API::getCityId(name) != -1) return ERR_VALUE;
    // generate a new unique id for city
    int id = newId(0);
    // add city
    city_list[id] = name;
    API::saveFile(DATA_PATH "city.json", city_list);
    // call
    return Dijkstra::newCity(id)==OK /* && DP::newCity(id)==OK */ ? OK : ERR;
}

Status API::delCity(int id) {
    // check if city exists
    if (!city_list.count(id)) return ERR_VALUE;
    // delete related routes
    std::map<int, std::string> tmp;
    tmp = train_list;
    for (auto &iter : tmp) {
        crow::json::rvalue r = crow::json::load(iter.second);
        if (r[1].i() == id || r[2].i() == id)
            API::delRoute(iter.first, TRAIN);
    }
    tmp = plane_list;
    for (auto &iter : tmp) {
        crow::json::rvalue r = crow::json::load(iter.second);
        if (r[1].i() == id || r[2].i() == id)
            API::delRoute(iter.first, PLANE);
    }
    // delete city
    city_list.erase(id);
    API::saveFile(DATA_PATH "city.json", city_list);
    // call
    return Dijkstra::delCity(id)==OK /* && DP::delCity(id)==OK */ ? OK : ERR;
}

Status API::newRoute(std::string name, int tp, int a, int b, int t, int d, int c) {
    // assert a & b are existing city id
    if (city_list.find(a) == city_list.end() || city_list.find(b) == city_list.end())
            return ERR_ASSERTION;
    ROUTE_TYPE route_type;
    std::stringstream buffer;
    d -= t;  // caculate duration
    buffer << "[\"" << name << "\"," << a << "," << b << "," << t << "," << d << "," << c << "]";
    int id;
    switch(tp) {
    case TRAIN:
        route_type = TRAIN;
        id = newId(1);
        train_list[id] = buffer.str();
        API::saveFile(DATA_PATH "train.json", train_list);
        break;
    case PLANE:
        route_type = PLANE;
        id = newId(2);
        plane_list[id] = buffer.str();
        API::saveFile(DATA_PATH "plane.json", plane_list);
        break;
    default: return ERR_VALUE;
    }
    // call
    return Dijkstra::newRoute(id, route_type, a, b, t, d, c)==OK /* && DP::newRoute(id, route_type, a, b, t, d, c)==OK */ ? OK : ERR;
}

Status API::delRoute(int id, int tp) {
    ROUTE_TYPE route_type;
    switch(tp) {
    case TRAIN:
        route_type = TRAIN;
        if (!train_list.count(id)) return ERR_VALUE;
        train_list.erase(id);
        API::saveFile(DATA_PATH "train.json", train_list);
        break;
    case PLANE:
        route_type = PLANE;
        if (!plane_list.count(id)) return ERR_VALUE;
        plane_list.erase(id);
        API::saveFile(DATA_PATH "plane.json", plane_list);
        break;
    default: return ERR_VALUE;
    }
    // call
    return Dijkstra::delRoute(id, route_type)==OK /* && DP::delRoute(id, route_type)==OK */ ? OK : ERR;
}

std::vector<int> API::search(int a, int b, int r, int p) {
    // call
    ROUTE_TYPE route_type;
    switch (r) {
    case TRAIN: route_type = TRAIN; break;
    case PLANE: route_type = PLANE; break;
    default: return {-1};
    }
    POLICY_TYPE policy;
    switch (p) {
    case COST: policy = COST; break;
    case TIME: policy = TIME; break;
    case INTERCHANGE: policy = INTERCHANGE; break;
    default: return {-1};
    }
    switch (algorithm) {
    // case ALGO_DP: return DP::search(a, b, route_type, policy);
    case ALGO_DIJK: return Dijkstra::search(a, b, route_type, policy);
    }
    return {-1};
}
