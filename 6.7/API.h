#include <algorithm>
using namespace std;

#define SIZE 1100
#define VAL_RANGE 100
#define PRIORITY_NUM 5

extern int valNumber; // number of values in a record

struct Record {
    int val[5]; // ordered by priority
};

namespace LSD {
// desp: internal sort
// args: Recoord record array
// args: int length of record array
void internalSort(Record*, int);

// desp: radix sort
// args: Record record array
// args: int len
void radixSort(Record*, int);
} // namespace LSD

namespace MSD {
// desp: internal sort
// args: Recoord record array
// args: int length of record array
void internalSort(Record*, int);
} // namespace MSD
