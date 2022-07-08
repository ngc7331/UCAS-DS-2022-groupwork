#include "API.h"
#include <cstdio>
using namespace std;

int len, valNumber;
Record record[SIZE];

bool DEBUGMODE;

int priority[5];

int main() {
    FILE* data = fopen("data.txt", "r");
    FILE* result = fopen("result.txt", "w");
    printf("从data.txt读入数据，输出结果到result.txt\n");
    int tp;
    fscanf(data, "%d", &tp);
    printf("排序方式：");
    if (tp == 1)
        printf("LSD，内部排序\n");
    else if (tp == 2)
        printf("LSD，分配收集\n");
    else
        printf("MSD\n");
    fscanf(data, "%d", &valNumber);
    for (int i = 0; i < valNumber; ++i)
        fscanf(data, "%d", &priority[i]), --priority[i];
    fscanf(data, "%d", &len);
    for (int i = 0; i < len; ++i) {
        int tmp[5];
        for (int j = 0; j < valNumber; ++j)
            fscanf(data, "%d", &record[i].val[priority[j]]);
    }
    if (tp == 1)
        LSD ::internalSort(record, len);
    else if (tp == 2)
        LSD ::radixSort(record, len);
    else
        MSD ::internalSort(record, len);
    for (int i = 0; i < len; ++i) {
        int tmp[5];
        for (int j = 0; j < valNumber; ++j)
            tmp[j] = record[i].val[priority[j]];
        for (int j = 0; j < valNumber; ++j)
            fprintf(result, "%d ", tmp[j]);
        fprintf(result, "\n");
    }
    printf("排序完成\n");
}