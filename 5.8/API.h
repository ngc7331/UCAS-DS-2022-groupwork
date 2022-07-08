#ifndef __TQ_API
#define __TQ_API

#include "type.h"
#include <map>
#include <string>
#include <vector>

namespace API {
// desp:   get city id by its name
// args:   std::string name
// return: int city id
int getCityId(std::string);

// desp:   load a json file
// args:   std::string path
// return: std::map<int, std::string> json object
std::map<int, std::string> loadFile(std::string);

// desp:   save a json file
// args:   std::string path
// args:   std::map<int, std::string> json object
// return: Status code
Status saveFile(std::string, std::map<int, std::string>);

// desp:   set algorithm
// args:   ALGO_TYPE type
// return: Status code
Status setAlgo(int);

// desp:   check if a and b are the same train
// note:   should be called only if ROUTE_TYPE is TRAIN
// args:   int route_id a
// args:   int route_id b
// return: bool
bool isSameTrain(int, int);

// desp:   create a new city
// args:   std::string name
// return: Status code
Status newCity(std::string);

// desp:   delete a existing city
// args:   int city_id
// return: Status code
Status delCity(int);

// desp:   create a new route
// args:   std::string name
// args:   ROUTE_TYPE route_type
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   int departure_time
// args:   int duration
// args:   int cost
// return: Status code
Status newRoute(std::string, int, int, int, int, int, int);

// desp:   delete a existing route
// args:   int route_id
// args:   ROUTE_TYPE route_type
// return: Status code
Status delRoute(int, int);

// desp:   find path that matches policy
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   ROUTE_TYPE route_type
// args:   POLICY_TYPE policy
// return: std::vector<int> sequence of [route_id] in the path
std::vector<int> search(int, int, int, int);
} // namespace API

namespace Dijkstra {
// desp:   create a new city
// args:   int city_id
// return: Status code
Status newCity(int);

// desp:   delete a existing city
// args:   int city_id
// return: Status code
Status delCity(int);

// desp:   create a new route
// args:   int route_id
// args:   ROUTE_TYPE route_type
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   int departure_time
// args:   int duration
// args:   int cost
// return: Status code
Status newRoute(int, ROUTE_TYPE, int, int, int, int, int);

// desp:   delete a existing route
// args:   int route_id
// args:   ROUTE_TYPE route_type
// return: Status code
Status delRoute(int, ROUTE_TYPE);

// desp:   find path that matches policy
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   ROUTE_TYPE route_type
// args:   POLICY_TYPE policy
// return: std::vector<int> sequence of [route_id] in the path
std::vector<int> search(int, int, ROUTE_TYPE, POLICY_TYPE);
} // namespace Dijkstra

namespace DP {
// desp:   create a new city
// args:   int city_id
// return: Status code
Status newCity(int);

// desp:   delete a existing city
// args:   int city_id
// return: Status code
Status delCity(int);

// desp:   create a new route
// args:   int route_id
// args:   ROUTE_TYPE route_type
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   int departure_time
// args:   int duration
// args:   int cost
// return: Status code
Status newRoute(int, ROUTE_TYPE, int, int, int, int, int);

// desp:   delete a existing route
// args:   int route_id
// args:   ROUTE_TYPE route_type
// return: Status code
Status delRoute(int, ROUTE_TYPE);

// desp:   find path that matches policy
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   ROUTE_TYPE route_type
// args:   POLICY_TYPE policy
// return: std::vector<int> sequence of [route_id] in the path
std::vector<int> search(int, int, ROUTE_TYPE, POLICY_TYPE);
} // namespace DP

#endif
