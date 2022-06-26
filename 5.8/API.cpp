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
int newId(int type) {
    if (type == 0)      {__newId(city_list)}
    else if (type == 1) {__newId(train_list)}
    else                {__newId(plane_list)}
}

int API::getCityId(std::string name) {
    for (auto &iter : city_list) {
        if (iter.second == name)
            return iter.first;
    }
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

void API::saveFile(std::string path, std::map<int, std::string> src) {
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
}

bool API::isSameTrain(int a, int b) {
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
    // TODO: deleta related routes
    // delete city
    city_list.erase(id);
    API::saveFile(DATA_PATH "city.json", city_list);
    // call
    return Dijkstra::delCity(id)==OK /* && DP::delCity(id)==OK */ ? OK : ERR;
}
