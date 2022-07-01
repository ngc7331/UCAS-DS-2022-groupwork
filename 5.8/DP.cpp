#include <set>
#include <vector>
#include <algorithm>
#include "API.h"
using namespace DP;
using namespace std;

struct Edge
{
    int id;
    int s, t;
    int sTime, tTime;
    int cost;
    
    Edge() {}
    Edge(int _id, int _s, int _t, int _sTime, int _tTime, int _cost) : id(id), s(_s), t(_t), sTime(_sTime), tTime(_tTime), cost(_cost) {}
};
bool operator<(Edge a, Edge b) { return a.tTime < b.tTime; }
set <Edge> edge[2];

struct PreEdge
{
    int id, tTime;

    PreEdge() {}
    PreEdge(int _id, int _tTime) : id(_id), tTime(_tTime) {}
};
bool operator<(PreEdge a, PreEdge b) { return a.tTime == b.tTime ? a.tTime < b.tTime : a.id < b.id; }
vector <pair <PreEdge, int>> f[MAX_CITY];
vector <int> suffixMin[MAX_CITY];
bool exis[MAX_CITY];

Status newCity(int x)
{
    if (exis[x]) return ERR_VALUE;
    exis[x] = 1;
    return OK;
}

Status delCity(int x)
{
    if (!exis[x]) return ERR_VALUE;
    exis[x] = 0;
    return OK;
}

Status newRoute(int id, ROUTE_TYPE type, int s, int t, int sTime, int duration, int cost)
{
    Edge tmp = Edge(id, s, t, sTime, sTime + duration, cost);
    if (edge[type].find(tmp) != edge[type].end()) return ERR_VALUE;
    edge[type].insert(tmp);
    return OK;
}

// desp:   delete a existing route
// args:   int route_id
// args:   ROUTE_TYPE route_type
// return: Status code
Status delRoute(int id, ROUTE_TYPE type) //maybe need some more information
{

}

void init()
{

}

vector <int> search(int S, int T, ROUTE_TYPE routeType, POLICY_TYPE policyType)
{
    init();
    for (auto i : edge[routeType])
    {
        int pos = upper_bound(f[i.s].begin(), f[i.s].end(), make_pair(PreEdge(0, i.sTime), 0)) - f[i.s].begin();
        int cost = suffixMin[i.s][pos] + i.cost;
        f[i.t].push_back(make_pair(PreEdge(i.id, i.tTime), cost));
        suffixMin[i.t].push_back(min(suffixMin[i.t][pos - 1], cost));
    }
}