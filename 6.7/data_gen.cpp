#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

int random(int l, int r) { return rand() % (r - l + 1) + l; }

int main()
{
    freopen("data.txt", "w", stdout);
    srand(time(0));
    int tp = random(1, 3);
    int valNumber = random(1, 5);
    int priority[5] = {1, 2, 3, 4, 5};
    random_shuffle(priority, priority + valNumber);
    printf("%d\n%d\n", tp, valNumber);
    for (int i = 0; i < valNumber; ++ i)
        printf("%d ", priority[i]);
    printf("\n");
    int len = random(1, 1000);
    printf("%d\n", len);
    for (int i = 0; i < len; ++ i)
    {
        for (int j = 0; j < valNumber; ++ j)
            printf("%d ", random(1, 100));
        printf("\n");
    }
}