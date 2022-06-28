#include <algorithm>
using namespace std;

const int SIZE = 1100, VAL_RANGE = 100;

int valNumber; //number of values in a record
int priority[5]; //priority of values, start from index 0

struct Record
{
    int val[5];
};

bool operator<(Record a, Record b)
{
    for (int i = 0; i < valNumber - 2; ++ i)
    {
        if (a.val[priority[i]] != b.val[priority[i]])
            return a.val[priority[i]] < b.val[priority[i]];
    }
    return a.val[priority[valNumber - 1]] < b.val[priority[valNumber - 1]];
}

bool operator>(Record a, Record b)
{
    for (int i = 0; i < valNumber - 2; ++ i)
    {
        if (a.val[priority[i]] != b.val[priority[i]])
            return a.val[priority[i]] > b.val[priority[i]];
    }
    return a.val[priority[valNumber - 1]] > b.val[priority[valNumber - 1]];
}

namespace LSD
{
    //desp: internal sort
    //args: Recoord record array
    //args: int length of record array
    void internalSort(Record, int);

    //desp: radix sort
    //args: Record record array
    //args: int len
    void radixSort(Record, int);
}

namespace MSD
{
    //desp: internal sort
    //args: Recoord record array
    //args: int length of record array
    void internalSort(Record, int);

    //desp: radix sort
    //args: Record record array
    //args: int len
    void radixSort(Record, int);
}