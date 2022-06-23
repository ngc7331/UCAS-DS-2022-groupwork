#include"API.h"
using namespace Dijkstra;

//状态：
//实现了搜索之外的内容
//使用vector处理城市，要求城市ID尽可能是小的整数。ID对应唯一的城市，相当于最平凡的hash映射。如果后期需要修改，应该用map等类型
//使用全局变量，不确定是不是需要额外初始化！

//数据结构
//变量采用小驼峰命名法，数据结构采用大驼峰命名法
typedef struct{
    ROUTE_TYPE routeType;
    int routeID;        //编号，用于识别转乘次数
    int cost;           //花费
    int startCityIndex; //起始城市的标号,作为邻接表的边而言是冗余信息
    int startTime;      //出发时间，为前端处理后的，精确到分钟的时间
    int endCityIndex;   //终点城市的ID
    int endTime;        //到达时间，为前端处理后的，精确到分钟的时间
} EdgeInfo;

typedef struct NodeLink{
    EdgeInfo edgeInfo;
    struct NodeLink *next;
} NodeLink;

typedef struct{
    bool existence; //判断城市是否已经存在，因为有的城市可能没有线路连接，但已经存在，虽然没有意义。
    NodeLink *first;
} CityType;

typedef struct{
    int cityNum, edgeNum; // 图的顶点数，边数
    std::vector<CityType> city;
} AGraph;


//全局变量
AGraph Map; 

//本文件特定函数
std::vector<int> searchMinCost(int startCity, int endCity, ROUTE_TYPE routeType);
std::vector<int> searchQuickest(int startCity, int endCity, ROUTE_TYPE routeType);
std::vector<int> searchMinInterchange(int startCity, int endCity, ROUTE_TYPE routeType);

// desp:   create a new city
// args:   int city_id
// return: Status code
Status Dijkstra::newCity(int cityID)
{
    if (cityID >= 0)
    {
        if (cityID >= Map.city.size())
            Map.city.resize(cityID + 10);
        Map.city[cityID].existence = true;
        Map.cityNum++; 
        return OK;
    }
    return ERR_VALUE;
}

// desp:   delete a existing city
// args:   int city_id
// return: Status code
Status Dijkstra::delCity(int cityID)
{
    if (cityID >= 0 && cityID < Map.city.size())
    {
        Map.city[cityID].existence = false;
        Map.cityNum--;
        //边没有被删除，在后续遍历时，如果遇到非法边，再删除。
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
Status Dijkstra::newRoute(int routeID, ROUTE_TYPE routeType, int startCityID, int endCityID, int stratTime, int endTime, int cost)
{
    if (startCityID >= Map.city.size() || Map.city[startCityID].existence == false)
        newCity(startCityID);
    if (endCityID >= Map.city.size() || Map.city[endCityID].existence == false)
        newCity(endCityID);

    NodeLink *p;
    if (!Map.city[startCityID].first)
    {
        Map.city[startCityID].first = new NodeLink;
        p = Map.city[startCityID].first;
    }
    else{
        for (p = Map.city[startCityID].first; p->next; p = p->next)
        {
            if (p->edgeInfo.routeID == routeID && p->edgeInfo.routeType == routeType)
                return ERR_VALUE; //边已经存在
        }
        p->next = new NodeLink;
        p = p->next;
    }//找到需要插入的位置p

    p->edgeInfo.routeID = routeID;
    p->edgeInfo.routeType = routeType;
    p->edgeInfo.startCityIndex = startCityID;
    p->edgeInfo.startTime = stratTime;
    p->edgeInfo.endCityIndex = endCityID;
    p->edgeInfo.endTime = endTime;
    p->edgeInfo.cost = cost;
    p->next = nullptr;
    return OK;
}

// desp:   delete a existing route
// args:   int route_id
// args:   ROUTE_TYPE route_type
// return: Status code
Status Dijkstra::delRoute(int routeID, ROUTE_TYPE routeType)
{
    NodeLink *bp, *p;
    for (auto startCity : Map.city) //遍历Map中的city
    {
        if (startCity.existence){
            if (startCity.first)
            {
                bp = p = startCity.first;
                while(p)
                {
                    if (p->edgeInfo.routeID == routeID && p->edgeInfo.routeType == routeType)
                    {
                        bp->next = p->next;
                        delete p;
                        return OK;
                    }
                    bp = p;
                    p = p->next;
                }
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
std::vector<int> Dijkstra::search(int startCity, int endCity, ROUTE_TYPE routeType, POLICY_TYPE decisionKind)
{
    switch (decisionKind)
    {
        case COST:{
            return searchMinCost(startCity, endCity, routeType);
        }
        case TIME:{
            return searchQuickest(startCity, endCity, routeType);
        }
        case INTERCHANGE:{
            return searchMinInterchange(startCity, endCity, routeType);
        }
    }
}

// desp:   find path that cost least
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   ROUTE_TYPE route_type
// return: std::vector<int> sequence of [route_id] in the path
std::vector<int> searchMinCost(int startCity, int endCity, ROUTE_TYPE routeType)
{
    
}

// desp:   find path that arrive quickliest
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   ROUTE_TYPE route_type
// return: std::vector<int> sequence of [route_id] in the path
std::vector<int> searchQuickest(int startCity, int endCity, ROUTE_TYPE routeType)
{

}

// desp:   find path that cost least
// args:   int departure_city_id
// args:   int arrival_city_id
// args:   ROUTE_TYPE route_type
// return: std::vector<int> sequence of [route_id] in the path
std::vector<int> searchMinInterchange(int startCity, int endCity, ROUTE_TYPE routeType)
{

}
