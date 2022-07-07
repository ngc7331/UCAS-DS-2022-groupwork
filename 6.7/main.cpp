#include <cstdio>
#include "API.h"
using namespace std;

int len, valNumber;
Record record[SIZE];

bool DEBUGMODE;

int priority[5];

int main()
{
    freopen("data.txt", "r", stdin);
    freopen("result.txt", "w", stdout);
    int tp;
    scanf("%d", &tp);
    scanf("%d", &valNumber);
    for (int i = 0; i < valNumber; ++ i)
        scanf("%d", &priority[i]), -- priority[i];
    scanf("%d", &len);
    for (int i = 0; i < len; ++ i)
    {
        int tmp[5];
        for (int j = 0; j < valNumber; ++ j)
            scanf("%d", &record[i].val[priority[j]]);
    }
    if (tp == 1) LSD :: internalSort(record, len);
    else if (tp == 2) LSD :: radixSort(record, len);
    else MSD :: internalSort(record, len);
    for (int i = 0; i < len; ++ i)
    {
        int tmp[5];
        for (int j = 0; j < valNumber; ++ j)
            tmp[j] = record[i].val[priority[j]];
        for (int j = 0; j < valNumber; ++ j)
            printf("%d ", tmp[j]);
        printf("\n");
    }
}