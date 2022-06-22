#ifndef __TQ_TYPE
#define __TQ_TYPE

typedef enum {
    ALGO_DP,     // 动态规划
    ALGO_DIJK    // Dijkstra
} ALGO_TYPE;     // 算法类型

typedef enum {
    PLANE,       // 飞机
    TRAIN        // 火车
} ROUTE_TYPE;    // 路线类型

typedef enum {
    COST,        // 最少花费
    TIME,        // 最短时间
    INTERCHANGE  // 最少换乘
} POLICY_TYPE;   // 搜索策略类型

#endif