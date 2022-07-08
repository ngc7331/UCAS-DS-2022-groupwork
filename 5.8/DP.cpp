#include <set>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "API.h"
#include "const.h"
using namespace DP;
using namespace std;
using API :: isSameTrain;

const int INF = 1e9;

struct Edge
{
    int id;
    int s, t;
    int sTime, tTime;
    int cost;
    
    Edge() {}
    Edge(int _id, int _s, int _t, int _sTime, int _tTime, int _cost) : id(_id), s(_s), t(_t), sTime(_sTime), tTime(_tTime), cost(_cost) {}
};
bool operator<(Edge a, Edge b) { return a.tTime != b.tTime ? a.tTime < b.tTime : a.id < b.id; }
set <Edge> edge[2];
int tTime[MAX_CITY];

struct ArrEdge
{
    int id, tTime;

    ArrEdge() {}
    ArrEdge(int _id, int _tTime) : id(_id), tTime(_tTime) {}
};
bool operator<(ArrEdge a, ArrEdge b) { return a.tTime != b.tTime ? a.tTime < b.tTime : a.id < b.id; }
struct State
{
    ArrEdge arr;
    int cost, pre, index;

    State() {}
    State(ArrEdge _arr, int _cost, int _pre, int _index) : arr(_arr), cost(_cost), pre(_pre), index(_index) {}
};
bool operator<(State a, State b) { return a.arr < b.arr; }
vector <State> f[MAX_CITY];
unordered_set <int> city;

Status DP :: newCity(int x)
{
    if (city.find(x) != city.end()) return ERR_VALUE;
    city.insert(x);
    return OK;
}

Status DP :: delCity(int x)
{
    if (city.find(x) == city.end()) return ERR_VALUE;
    city.erase(x);
    return OK;
}

Status DP :: newRoute(int id, ROUTE_TYPE type, int s, int t, int sTime, int duration, int cost)
{
    Edge tmp = Edge(id, s, t, sTime, sTime + duration, cost);
    if (edge[type].find(tmp) != edge[type].end()) return ERR_VALUE;
    edge[type].insert(tmp);
    tTime[id] = sTime + duration;
    return OK;
}

Status DP :: delRoute(int id, ROUTE_TYPE type)
{
    Edge tmp = Edge(id, 0, 0, 0, tTime[id], 0);
    if (edge[type].find(tmp) != edge[type].end()) return ERR_VALUE;
    edge[type].erase(tmp);
    return OK;
}

vector <int> sequence;

void getRoute(int x, int index, int S)
{
    if (x == S) return;
    getRoute(f[x][index].pre, f[x][index].index, S);
    sequence.push_back(f[x][index].arr.id);
}

void clear()
{
    for (auto i : city)
        f[i].clear();
}

void work(int S, int T, ROUTE_TYPE routeType, POLICY_TYPE policyType)
{
    for (auto i : edge[routeType])
    {
        int index = upper_bound(f[i.s].begin(), f[i.s].end(), State(ArrEdge(0, i.sTime), 0, 0, 0)) - f[i.s].begin() - 1;
        if (index < 0 || f[i.s][index].arr.tTime > i.sTime) continue;
        int cost = f[i.s][index].cost;
        if (policyType == COST) cost += i.cost;
        else
        {
            ++ cost;
            for (int tmp = index; tmp >= 0 && f[i.s][tmp].arr.tTime >= i.sTime - 25; -- tmp)
            {
                if (isSameTrain(f[i.s][tmp].arr.id, i.id) && f[i.s][tmp].cost < cost)
                {
                    cost = f[i.s][tmp].cost;
                    index = tmp;
                }
            }
        }
        if (f[i.t].empty()) f[i.t].push_back(State(ArrEdge(i.id, i.tTime), cost, i.s, index));
        else
        {
            auto last = f[i.t].end() - 1;
            if (cost < last->cost)
                f[i.t].push_back(State(ArrEdge(i.id, i.tTime), cost, i.s, index));
        }
    }
}

vector <int> DP :: search(int S, int T, ROUTE_TYPE routeType, POLICY_TYPE policyType)
{
    if (policyType == TIME)
    {
        int time = INF, cost = INF;
        for (auto i : edge[routeType])
        {
            if (i.s == S)
            {
                clear();
                f[S].push_back(State(ArrEdge(0, i.sTime), 0, 0, 0));
                work(S, T, routeType, COST);

                if (f[T].empty()) continue;
                int tmp = 0;
                while (tmp + 1 <f[T].size() && f[T][tmp + 1].arr.tTime == f[T][0].arr.tTime) ++ tmp;
                if (f[T][tmp].arr.tTime - i.sTime < time || (f[T][tmp].arr.tTime - i.sTime == time && f[T][tmp].cost < cost))
                {
                    time = f[T][tmp].arr.tTime - i.sTime;
                    cost = f[T][tmp].cost;
                    sequence.clear();
                    getRoute(T, tmp, S);
                }
            }
        }
    }
    else
    {
        clear();
        f[S].push_back(State(ArrEdge(0, 0), 0, 0, 0));
        work(S, T, routeType, policyType);

        sequence.clear();
        if (!f[T].empty()) getRoute(T, f[T].size() - 1, S);
    }

    return sequence;
}

#ifdef debug
int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= m; ++ i)
    {
        int s, t, sTime, duration, cost;
        scanf("%d %d %d %d %d", &s, &t, &sTime, &duration, &cost);
        newRoute(i, PLANE, s, t, sTime, duration, cost);
    }
    int S, T;
    scanf("%d %d", &S, &T);
    vector <int> result = search(S, T, PLANE, INTERCHANGE);
    for (auto i : result)
        printf("%d\n", i);
}
#endif