#include <set>
#include <vector>
#include <algorithm>
#include "API.h"
#include "const.h"
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
bool operator<(Edge a, Edge b) { return a.tTime == b.tTime ? a.tTime < b.tTime : a.id < b.id; }
set <Edge> edge[2];
int tTime[MAX_CITY];

struct ArrEdge
{
    int id, tTime;

    ArrEdge() {}
    ArrEdge(int _id, int _tTime) : id(_id), tTime(_tTime) {}
};
bool operator<(ArrEdge a, ArrEdge b) { return a.tTime == b.tTime ? a.tTime < b.tTime : a.id < b.id; }
struct State
{
    ArrEdge arr;
    int cost, pre, index;

    State() {}
    State(ArrEdge _arr, int _cost, int _pre, int _index) : arr(_arr), cost(_cost), pre(_pre), index(_index) {}
};
bool operator<(State a, State b) { return a.arr < b.arr; }
vector <State> f[MAX_CITY];
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
    tTime[id] = sTime + duration;
    return OK;
}

Status delRoute(int id, ROUTE_TYPE type) //maybe need some more information
{
    Edge tmp = Edge(id, 0, 0, 0, tTime[id], 0);
    if (edge[type].find(tmp) != edge[type].end()) return ERR_VALUE;
    edge[type].erase(tmp);
    return OK;
}

vector <int> sequence;

void getRoute(int x, int index, int S)
{
    if (x != S)
        getRoute(f[x][index].pre, f[x][index].index, S);
    sequence.push_back(x);
}

vector <int> search(int S, int T, ROUTE_TYPE routeType, POLICY_TYPE policyType)
{
    for (auto i : edge[routeType])
        f[i.t].clear();

    f[S].push_back(State(ArrEdge(0, 0), 0, 0, 0));

    for (auto i : edge[routeType])
    {
        int index = upper_bound(f[i.s].begin(), f[i.s].end(), make_pair(ArrEdge(0, i.sTime), 0)) - f[i.s].begin();
        int cost = f[i.s][index].cost;
        auto last = f[i.t].end() - 1;
        if (cost < last->cost)
            f[i.t].push_back(State(ArrEdge(i.id, i.tTime), cost, i.s, index));
    }

    sequence.clear();
    getRoute(T, f[T].size() - 1, S);
}
