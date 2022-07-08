#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

int random(int l, int r) { return rand() % (r - l + 1) + l; }

int main()
{
    FILE *data = fopen("data.txt", "w");
    srand(time(0));

    printf("请输入排序类型：\n0、随机\n1、LSD，内部排序\n2、LSD，分配收集\n3、MSD\n");
    int tp;
    scanf("%d", &tp);
    if (tp == 0) tp = random(1, 3);

    printf("请输入关键字个数，输入0则随机生成\n");
    int valNumber;
    scanf("%d", &valNumber);
    if (valNumber == 0) valNumber = random(1, 5), printf("生成的关键字个数：%d\n", valNumber);

    printf("请输入关键字优先级，输入0则随机生成\n");
    int tmp;
    scanf("%d", &tmp);
    int priority[5] = {1, 2, 3, 4, 5};
    if (tmp)
    {
        priority[0] = tmp;
        for (int i = 1; i < valNumber; ++ i)
            scanf("%d", &priority[i]);
    }
    else random_shuffle(priority, priority + valNumber);

    fprintf(data, "%d\n%d\n", tp, valNumber);
    for (int i = 0; i < valNumber; ++ i)
        fprintf(data, "%d ", priority[i]);
    fprintf(data, "\n");

    printf("请输入记录个数，输入0则随机生成\n");
    int len;
    scanf("%d", &len);
    if (!len) len = random(1, 1000);
    
    printf("开始生成数据\n");
    fprintf(data, "%d\n", len);
    for (int i = 0; i < len; ++ i)
    {
        for (int j = 0; j < valNumber; ++ j)
            fprintf(data, "%d ", random(1, 100));
        fprintf(data, "\n");
    }
    printf("数据生成完成\n");
}