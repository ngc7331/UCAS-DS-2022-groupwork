#include <cstdio>
#include "API.h"
using namespace std;

int len;
Record record[SIZE];

bool DEBUGMODE;

int priority[5];

int main()
{
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
            scanf("%d", &tmp[j]);
        for (int j = 0; j < valNumber; ++ j)
            record[i].val[j] = tmp[priority[j]];
    }
    LSD :: internalSort(record, len);
    for (int i = 0; i < len; ++ i)
    {
        int tmp[5];
        for (int j = 0; j < valNumber; ++ j)
            printf("%d ", tmp[priority[j]] = record[i].val[j]);
        for (int j = 0; j < valNumber; ++ j)
            printf("%d ", tmp[j]);
        printf("\n");
    }
}