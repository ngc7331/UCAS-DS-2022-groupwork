#include "API.h"
#include<iostream>

#define PRIORITY_NUM 5
bool DEBUGMODE;
#define DEBUG(expr) \
    if (DEBUGMODE)  \
    {               \
        expr        \
    }

//基本策略是，按照key0排序，然后将key0相同的数据按照key1排序，以此类推
//考虑实际情况，同时使用优化的和简单的排序策略：
//在依据key0, key1两个关键字时，使用非递归的归并排序，因为此时需要排序的人数比较多
//而依据key2, key3, key4排序时，使用直接插入排序，因为此时待排序的人数比较少

void Merge(Record *ARR, int Priority, int L, int M, int R)
{
    Record *arr = new Record[R - L + 1];
    int lIndex, rIndex, arrIndex;
    lIndex = L, rIndex = M + 1, arrIndex = 0;

    while ( lIndex <= M && rIndex <= R){
        if (ARR[lIndex].val[Priority] > ARR[rIndex].val[Priority])
            arr[arrIndex++] = ARR[lIndex++];
        else
            arr[arrIndex++] = ARR[rIndex++];
    }

    while (lIndex <= M){
        arr[arrIndex++] = ARR[lIndex++];
    }
    while (rIndex <= R){
        arr[arrIndex++] = ARR[rIndex++];
    }

    while(arrIndex >=0)
        ARR[R--] = arr[arrIndex--];
}

// ARR: 大数组
// Priority：依据的优先级
// L: 最左侧的index
// R: 最右侧的index
void MergeSort(Record *ARR, int Priority, int L, int R)
{
    DEBUG(std::cout << "MergeSort() called with  Priority " << Priority << std::endl;)

    int len = R - L + 1;

    for (int i = 1; i < len; i += i){
        int left = 0;
        int mid = left + i - 1;
        int right = mid + i;

        while (right < len)
        {
            Merge(ARR, Priority, left, mid, right);
            left = right + 1;
            mid = left + i - 1;
            right = mid + i;
        }
        if (left < len && mid < len)
        {
            Merge(ARR, Priority, left, mid, len - 1);
        }
    }
}

void SimpleSelectSort(Record *ARR, int Priority, int L, int R)
{
    DEBUG(std::cout << "SimpleSelectSort() called with  Priority " << Priority << std::endl;)

    for (int i = L; i <= R; i++)
    {
        int maxIndex = i;
        int maxKey = ARR[maxIndex].val[Priority];
        for (int k = i + 1; k <= R; k++){
            if (ARR[k].val[Priority] > maxKey)
            {
                maxKey = ARR[k].val[Priority];
                maxIndex = k;
            }
        }
        Record tmp = ARR[i];
        ARR[i] = ARR[maxIndex];
        ARR[maxIndex] = tmp;
    }
}

void MSDSort(Record *ARR, int Priority, int L, int R){
    if (Priority < 2)
        MergeSort(ARR, Priority, L, R);
    else if (Priority < PRIORITY_NUM)
        SimpleSelectSort(ARR, Priority, L, R);
    else
        return;
};

// desp: internal sort
// args: Recoord record array
// args: int length of record array
void MSD::internalSort(Record *ARR, int ArrLen)
{
    DEBUGMODE = false;

    DEBUG(std::cout << "internalSort() called \n";)

    int priority = 0;
    MSDSort(ARR, priority, 0, ArrLen - 1);

    for (; priority < PRIORITY_NUM - 1; priority++)
    {
        int i = 1;
        while (i < ArrLen){
            int len = 0;
            while (ARR[i - 1].val[priority] == ARR[i].val[priority])
            {
                len++;
                i++;
            }
            if(len!=0)
                MSDSort(ARR, priority + 1, i - len, i - 1);
            i++;
        }
    }
}
