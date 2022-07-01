#include <algorithm>
using namespace std;

const int SIZE = 1100, VAL_RANGE = 100;

int valNumber; //number of values in a record

struct Record
{
    int val[5]; //ordered by priority
};

namespace LSD
{
    //desp: internal sort
    //args: Recoord record array
    //args: int length of record array
    void internalSort(Record*, int);

    //desp: radix sort
    //args: Record record array
    //args: int len
    void radixSort(Record*, int);
}

namespace MSD
{
    //desp: internal sort
    //args: Recoord record array
    //args: int length of record array
    void internalSort(Record*, int);

    //desp: radix sort
    //args: Record record array
    //args: int len
    void radixSort(Record*, int);
}