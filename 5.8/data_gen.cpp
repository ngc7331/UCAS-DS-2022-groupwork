#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

int random(int l, int r) { return rand() % (r - l + 1) + l; }

int main() {
    // FILE *city = fopen("city.json", "w");
    FILE* plane = fopen("plane.json", "w");
    FILE* train = fopen("train.json", "w");

    int n = 24;
    // fprintf(city, "{\n");
    // for (int i = 1; i <= n - 1; ++ i)
    //     fprintf(city, "\t\"%d\": \"%d\",\n", i, i);
    // fprintf(city, "\t\"%d\": \"%d\"\n", n, n);
    // fprintf(city, "}\n");

    int m1 = 20, planeTime = 180;
    fprintf(plane, "{\n");
    for (int i = 1; i <= m1; ++i) {
        int sTime = random(1, 1440);
        int s = random(1, n), t = random(1, n);
        while (s == t)
            t = random(1, n);
        fprintf(plane, "    \"%d\": [\"%d\",%d,%d,%d,%d,%d]", i, i, s, t, sTime,
                random(1, planeTime), random(1, 2000));
        if (i != m1)
            fputc(',', plane);
        fputc('\n', plane);
    }
    fprintf(plane, "}\n");

    int m2 = 20, id = 0, middle[20], trainTime = 480;
    fprintf(train, "{\n");
    for (int i = 1; i <= m2; ++i) {
        int num = random(2, 10);
        for (int j = 1; j <= num; ++j)
            middle[j] = random(1, n);
        sort(middle + 1, middle + num + 1);
        num = unique(middle + 1, middle + num + 1) - middle - 1;
        random_shuffle(middle + 1, middle + num + 1);
        int sTime = random(1, 1440);
        for (int j = 2; j <= num; ++j) {
            int duration = random(1, trainTime);
            ++id;
            fprintf(train, "\t\"%d\": [\"%d\",%d,%d,%d,%d]", id, id, middle[j - 1], middle[j],
                    sTime, duration);
            if (i != m2 || j != num)
                fputc(',', train);
            fputc('\n', train);
            sTime += duration;
        }
    }
    fprintf(train, "}\n");
}