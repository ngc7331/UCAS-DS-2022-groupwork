#include <cstdio>
#include "API.h"
using namespace std;

int len;
Record record[SIZE];

int main()
{
    scanf("%d", &valNumber);
    for (int i = 0; i < valNumber; ++ i)
        scanf("%d", &priority[i]), -- priority[i];
    scanf("%d", &len);
    for (int i = 0; i < len; ++ i)
        for (int j = 0; j < valNumber; ++ j)
            scanf("%d", &record[i].val[j]);
}