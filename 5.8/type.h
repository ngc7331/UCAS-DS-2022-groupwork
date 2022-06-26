#ifndef __TQ_TYPE
#define __TQ_TYPE

typedef enum {
    ALGO_DP,     // 动态规划
    ALGO_DIJK    // Dijkstra
} ALGO_TYPE;     // 算法类型

typedef enum {
    TRAIN,       // 火车
    PLANE        // 飞机
} ROUTE_TYPE;    // 路线类型

typedef enum {
    COST,        // 最少花费
    TIME,        // 最短时间
    INTERCHANGE  // 最少换乘
} POLICY_TYPE;   // 搜索策略类型

typedef enum {
    OK,            // 成功
    ERR_VALUE,     // 传入无效的参数
    ERR_ASSERTION, // 断言失败
    ERR            // 其余错误
} Status;          // 函数状态码（参考Python的Exception设计）

#endif