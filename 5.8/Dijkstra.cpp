#include "API.h"
#include <queue>
#include <iostream>
using namespace Dijkstra;

#define MAXWEIGHT 0XFFFF

//说明：
//删除完城市与路径时，整个体系的合法性需要由前端保证

//数据结构部分
//变量采用小驼峰命名法，数据结构采用大驼峰命名法
typedef struct RouteInfo
{
    ROUTE_TYPE routeType; // route类型
    int routeID;          // route编号，用于识别同Type下唯一的route
    int cost;             // route花费
    int startCityIndex;   // route的起始城市的标号，对应数组下标
    int startTime;        // route的出发时间，精确到分钟，由前端处理
    int endCityIndex;     // route终点城市的标号，对应数组下标
    int endTime;          // route的到达时间，为前端处理后的，精确到分钟的时间

    friend bool operator<(RouteInfo a, RouteInfo b)
    {
        return a.routeID < b.routeID; //重载小于号，以routeID判断
    }

} RouteInfo;              // route相关信息

typedef struct NodeLink
{
    RouteInfo routeInfo;
    struct NodeLink *next;
} NodeLink; //邻接表中的结点

typedef struct
{
    bool existence;  //采用数组记录相关城市，用此判断城市是否已经存在
    NodeLink *first; //在existence变化时进行初始化
} CityType;          //城市相关信息

typedef struct
{
    int cityNum, edgeNum; // 图的顶点数，边数。
    std::vector<CityType> city;
} AGraph;

//全局变量
AGraph Map;

//本文件的内部函数
int searchDirectRoute(int startCity, int endCity, ROUTE_TYPE routeType, POLICY_TYPE decisionKind, int startTime);

// desp:   create a new city
// args:   int city_id
// return: Status code
Status Dijkstra::newCity(int cityID)
{
    std::cout << "newCity() called: " << cityID << std::endl;
    if (cityID >= 0)
    {
        if (cityID >= Map.city.size())
            Map.city.resize(cityID + 10); //扩大vector
        else if (Map.city[cityID].existence)
            return ERR_VALUE; //该城市已经存在

        //初始化该城市，并维护全局变量
        Map.city[cityID].existence = true;
        Map.city[cityID].first = nullptr;  
        Map.cityNum++;
        return OK;
    }
    return ERR_VALUE; //输入ID非法
}

// desp:   delete a existing city
// args:   int city_id
// return: Status code
Status Dijkstra::delCity(int cityID)
{ //删除该点的相应边由前端实现
    if (cityID >= 0 && cityID < Map.city.size() && Map.city[cityID].existence)
    {
        Map.city[cityID].existence = false;
        Map.city[cityID].first = nullptr; //初始化该城市的指针
        Map.cityNum--;
        return OK;
    }
    return ERR_VALUE;
}

// desp:   create a new route
// args:   int route_id
// args:   ROUTE_TYPE route_type
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   int departure_time
// args:   int duration
// args:   int cost
// return: Status code
Status Dijkstra::newRoute(int routeID, ROUTE_TYPE routeType, int startCityID, int endCityID, int startTime, int duration, int cost)
{
    std::cout << "newRoute() called: " << routeID << " " << routeType << " " << startCityID << " " << endCityID << " " << startTime << " " << duration << " " << cost << std::endl;
    if (startCityID < 0 || endCityID < 0)
        return ERR_VALUE; //输入边不合法

    if (startCityID >= Map.city.size() || Map.city[startCityID].existence == false)
        newCity(startCityID);
    if (endCityID >= Map.city.size() || Map.city[endCityID].existence == false)
        newCity(endCityID);

    //找到并创建需要插入的位置p
    NodeLink *p;
    if (!Map.city[startCityID].first)
    {
        Map.city[startCityID].first = new NodeLink;
        p = Map.city[startCityID].first;
    }
    else
    {
        for (p = Map.city[startCityID].first; p->next; p = p->next)
        {
            if (p->routeInfo.routeID == routeID && p->routeInfo.routeType == routeType)
                return ERR_VALUE; //边已经存在
        }
        p->next = new NodeLink;
        p = p->next;
    } //找到并创建需要插入的位置p

    //为p赋值
    Map.edgeNum++;
    p->routeInfo.routeID = routeID;
    p->routeInfo.routeType = routeType;
    p->routeInfo.startCityIndex = startCityID;
    p->routeInfo.startTime = startTime;
    p->routeInfo.endCityIndex = endCityID;
    p->routeInfo.endTime = duration + startTime;
    p->routeInfo.cost = cost;
    p->next = nullptr;
    return OK;
}

// desp:   delete a existing route
// args:   int route_id
// args:   ROUTE_TYPE route_type
// return: Status code
Status Dijkstra::delRoute(int routeID, ROUTE_TYPE routeType)
{//删除指定的边
    NodeLink *bp, *p;
    for (auto startCity : Map.city) //遍历Map中的city
    {
        if (startCity.existence && startCity.first)
        {
            bp = p = startCity.first;
            while (p)
            {
                if (p->routeInfo.routeID == routeID && p->routeInfo.routeType == routeType)
                { //删除需要删除的边，并更新全局计数器
                    bp->next = p->next;
                    delete p;
                    Map.edgeNum--;
                    return OK;
                }
                bp = p;
                p = p->next;
            }
        }
    }
    return ERR_VALUE;
}

// desp:   find path that matches policy
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   ROUTE_TYPE route_type
// args:   POLICY_TYPE policy
// return: std::vector<int> sequence of [route_id] in the path
std::vector<int> Dijkstra::search(int startCityID, int endCityID, ROUTE_TYPE routeType, POLICY_TYPE decisionKind)
{ //采用堆优化的Dijkstra算法，计算三种情况下的值
    std::cout << "DijkstraSearch() called: "
              << "startCityID:" << startCityID << ", endCityID: " << endCityID << ", routeType: " << (routeType == PLANE ? "PLANE" : "TRAIN")
              << ", decisionKind: " << ((decisionKind == COST) ? "COST" 
              : (decisionKind == TIME) ? "TIME" : "INTERCHANGE")
              << std::endl; // Debug

    if (startCityID < 0 || startCityID >= Map.city.size() || endCityID < 0 || endCityID >= Map.city.size()) {
        std::cout << "err: Overflow" << std::endl; // Debug
        return std::vector<int>(0); //合法性检查
    }
    if (!Map.city[startCityID].existence || !Map.city[endCityID].existence || !Map.city[startCityID].first) {
        std::cout << "err: not existent" << std::endl; // Debug
        return std::vector<int>(0); //不存在城市或起点城市不存在任何线路
    }

    // 使用优先队列(堆)，内容是pair
    // first 是一个相应的开销， second 是边的全部信息
    //使用小顶堆，使用greater模板，优先比较相应的开销信息
    // RouteInfo的比较函数已经重载过了，在结构体定义之中
    std::priority_queue<std::pair<int, RouteInfo>,
                        std::vector<std::pair<int, RouteInfo>>,
                        std::greater<std::pair<int, RouteInfo>>>
        q;
    
    //初始化变量
    std::vector<int> minCost(Map.city.size(),MAXWEIGHT); //记录最小开销
    std::vector<bool> visited(Map.city.size(), false); //初始化visited数组
    minCost[startCityID] = 0;

    std::vector<RouteInfo> routeRecord; //用于记录Dijkstra算法所容纳的所有边
    std::vector<int> pathRecord;        //用于记录需要返回给前端的边
    std::vector<int> tmpRecord;         //模拟堆，将routeRecord里的信息转移到pathRecord中
    int beforeCityID = endCityID;       //记录之前的城市，在将routeRecord里的信息转移到tmpRecord时需要使用

    RouteInfo null;                     //用于填充第一个route，无意义
    null.startCityIndex = null.endCityIndex = startCityID;
    null.cost = null.startTime = null.endTime = 0;
    q.push(std::make_pair(minCost[startCityID], null)); //将起始点放入堆
    while (!q.empty())
    {
        if (visited[endCityID])
            break;

        //队列首是目前所有线路中开销最小的线路comeRoute
        //相应的，lCity是开销最小就可到达的新城市
        RouteInfo comeRoute = q.top().second;
        q.pop();
        int lCity = comeRoute.endCityIndex;
        if (visited[lCity])
            continue;                                                  //重复到达该点，跳过
        routeRecord.push_back(comeRoute);                              //第一次到达该城市，说明相应边开销最小
        std::cout << "\tadd City " << lCity << " into Set" << std::endl; // Debug
        visited[lCity] = true;                                         //更新：到达过该城市

        //更新以该城市为出发点的其他城市的花费
        for (NodeLink *p = Map.city[lCity].first; p; p = p->next)
        {
            if (p->routeInfo.routeType == routeType)
            {
                int i = p->routeInfo.endCityIndex;
                if (decisionKind == TIME)
                { //在时间函数中，返回值是线路的到达时间，所以不需要累加
                    if (searchDirectRoute(lCity, i, routeType, decisionKind, comeRoute.endTime) < minCost[i])
                    {
                        minCost[i] = searchDirectRoute(lCity, i, routeType, decisionKind, comeRoute.endTime);
                        std::cout << "\t\tupdata minCost[" << i << "] = " << minCost[i] << std::endl; // Debug
                        q.push(std::make_pair(minCost[i], p->routeInfo));
                    }
                }
                else if(lCity == startCityID && decisionKind == INTERCHANGE)
                {//特殊处理：在始发站的直达站，换乘均为0
                        int i = p->routeInfo.endCityIndex;
                        minCost[i] = 0;
                        std::cout << "\t\tupdata minCost[" << i << "] = " << minCost[i] << std::endl; // Debug
                        q.push(std::make_pair(minCost[i], p->routeInfo));
                }
                else if (minCost[lCity] + searchDirectRoute(lCity, i, routeType, decisionKind, comeRoute.endTime) < minCost[i])
                { //在花费和换乘计算中，返回值是增量，所以需要累加
                    minCost[i] = minCost[lCity] + searchDirectRoute(lCity, i, routeType, decisionKind, comeRoute.endTime);
                    std::cout << "\t\tupdata minCost[" << i << "] = " << minCost[i] << std::endl; // Debug
                    q.push(std::make_pair(minCost[i], p->routeInfo));
                }
            }
        }
    } //更新其他城市的花费


    //如果目标城市还没有被访问过，说明没有路径
    if (!visited[endCityID])
        return std::vector<int>(0);

    //从routeRecord中读取合适的pathRecord
    //利用tmpRecord模拟栈进行中转
    // routeRecord的0号位置永远是无意义路径null，所以用i>0规避
    for (int i = routeRecord.size() - 1; i > 0; i--)
    {
        if (routeRecord[i].endCityIndex == beforeCityID)
        {
            beforeCityID = routeRecord[i].startCityIndex;
            tmpRecord.push_back(routeRecord[i].routeID);
        }
    }
    for (int i = tmpRecord.size() - 1; i >= 0; i--)
        pathRecord.push_back(tmpRecord[i]);
    return pathRecord;
}

int searchDirectRoute(int startCity, int endCity, ROUTE_TYPE routeType, POLICY_TYPE decisionKind, int startTime)
{
    int res = MAXWEIGHT;
    if (startCity < 0 || startCity >= Map.city.size() || endCity < 0 || endCity >= Map.city.size() || !Map.city[startCity].existence || !Map.city[endCity].existence)
        return MAXWEIGHT; //合法性检查，非法时，直接视为无穷大开销

    if (startCity == endCity)
        return 0;

    for (NodeLink *p = Map.city[startCity].first; p; p = p->next)
    {
        if (p->routeInfo.endCityIndex == endCity && p->routeInfo.routeType == routeType)
        {
            switch (decisionKind)
            {
                case COST:
                {
                    res = (p->routeInfo.cost < res) ? p->routeInfo.cost : res;
                    break;
                } 
                case TIME:
                {
                    if (p->routeInfo.startTime >= startTime)
                        res = (p->routeInfo.endTime < res) ? p->routeInfo.endTime : res;
                    break;
                }
                case INTERCHANGE:
                { //不能计算startCity本事就是起点站的例子，需要外部函数额外计算
                    if (p->routeInfo.startTime >= startTime)
                    { //找到了startCity到endCity的符合要求的路径
                        res = 1; //默认新路径需要一次额外中转
                        for(auto city: Map.city)
                        {
                            if(city.existence)
                            {
                                //遍历所有的边
                                for (NodeLink *bp = city.first; bp; bp = bp->next)
                                {
                                    if (bp->routeInfo.endCityIndex == startCity)
                                        if (API::isSameTrain(p->routeInfo.routeID, bp->routeInfo.routeID))
                                            return 0; 
                                            //如果在到达startCity的路径中存在同一列火车，说明从startCity到达endCity不需要换乘，返回0
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
    return res;
}