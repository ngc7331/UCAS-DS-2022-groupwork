#ifndef __TQ_API
#define __TQ_API

#include <vector>
#include "type.h"

namespace Dijkstra {
    // desp:   create a new city
    // args:   int city_id
    void newCity(int);

    // desp:   delete a existing city
    // args:   int city_id
    void delCity(int);

    // desp:   create a new route
    // args:   int route_id
    // args:   ROUTE_TYPE route_type
    // args:   int departure_city_id
    // args:   int arrival_city_id
    // args:   int departure_time
    // args:   int duration
    // args:   int cost
    void newRoute(int, ROUTE_TYPE, int, int, int, int, int);

    // desp:   delete a existing route
    // args:   int route_id
    void delRoute(int);

    // desp:   find path that matches policy
    // args:   int departure_city_id
    // args:   int arrival_city_id
    // args:   ROUTE_TYPE route_type
    // args:   POLICY_TYPE policy
    // return: std::vector<int> sequence of [route_id] in the path
    std::vector<int> search(int, int, ROUTE_TYPE, POLICY_TYPE);
}

namespace DP {
    // desp:   create a new city
    // args:   int city_id
    void newCity(int);

    // desp:   delete a existing city
    // args:   int city_id
    void delCity(int);

    // desp:   create a new route
    // args:   int route_id
    // args:   ROUTE_TYPE route_type
    // args:   int departure_city_id
    // args:   int arrival_city_id
    // args:   int departure_time
    // args:   int duration
    // args:   int cost
    void newRoute(int, ROUTE_TYPE, int, int, int, int, int);

    // desp:   delete a existing route
    // args:   int route_id
    void delRoute(int);

    // desp:   find path that matches policy
    // args:   int departure_city_id
    // args:   int arrival_city_id
    // args:   ROUTE_TYPE route_type
    // args:   POLICY_TYPE policy
    // return: std::vector<int> sequence of [route_id] in the path
    std::vector<int> search(int, int, ROUTE_TYPE, POLICY_TYPE);
}

#endif
