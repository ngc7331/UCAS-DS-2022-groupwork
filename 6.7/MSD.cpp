#include "API.h"
#include<iostream>

extern bool DEBUGMODE;
// bool DEBUGMODE;

#define DEBUG(expr) \
    if (DEBUGMODE)  \
    {               \
        expr        \
    }

//基本策略是，按照key0排序，然后将key0相同的数据按照key1排序，以此类推
//考虑实际情况，同时使用优化的和简单的排序策略：
//在依据key0, key1两个关键字时，使用非递归的归并排序，因为此时需要排序的人数比较多
//而依据key2, key3, key4排序时，使用直接插入排序，因为此时待排序的人数比较少

int debugLen = 13;
Record debugArr[1000];

void Merge(Record *ARR, const int Priority, const int L, const int M, const int R)
{
    Record *arr = new Record[R - L + 1];
    int lIndex, rIndex, arrIndex;
    lIndex = L, rIndex = M + 1, arrIndex = 0;

    while ( lIndex <= M && rIndex <= R){
        if (ARR[lIndex].val[Priority] < ARR[rIndex].val[Priority])
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

    for (int i = 0; i < arrIndex;i++)
        ARR[L + i] = arr[i];

    delete arr;
    DEBUG(std::cout << "Merge() called with L = " << L << ", M = " << M << ", R = " << R << std::endl;)
}

// ARR: 大数组
// Priority：依据的优先级
// L: 最左侧的index
// R: 最右侧的index
void MergeSort(Record *ARR, const int Priority, const int L, const int R)
{
    DEBUG(std::cout << "MergeSort() called with Priority = " << Priority <<", L = " << L << ", R = " << R << std::endl;)

    int len = R - L + 1;

    for (int i = 1; i < len; i += i){
        int left = L;
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

void SimpleSelectSort(Record *ARR, const int Priority, const int L, const int R)
{
    DEBUG(std::cout << "SimpleSelectSort() called: Priority = " << Priority << ", L = " << L <<", R = "<<R<< std::endl;)

    for (int i = L; i <= R; i++)
    {
        int minIndex = i;
        int minKey = ARR[minIndex].val[Priority];
        for (int k = i + 1; k <= R; k++){
            if (ARR[k].val[Priority] < minKey)
            {
                minKey = ARR[k].val[Priority];
                minIndex = k;
            }
        }
        Record tmp = ARR[i];
        ARR[i] = ARR[minIndex];
        ARR[minIndex] = tmp;
    }
}

void MSDSort(Record *ARR, const int Priority, const int L, const int R)
{
    if (Priority < 0)
        MergeSort(ARR, Priority, L, R);
    else if (Priority < PRIORITY_NUM)
        SimpleSelectSort(ARR, Priority, L, R);
    else
        return;
};

// desp: internal sort
// args: Recoord record array
// args: int length of record array
void MSD::internalSort(Record *ARR, const int ArrLen)
{
    DEBUGMODE = true;
    DEBUG(std::cout << "internalSort() called \n";)
    std::cout << "internalSort() called \n";
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
            if (len != 0)
                MSDSort(ARR, priority + 1, i - len - 1, i - 1);
            i++;
            }
        }
}

// int main()
// {
//     debugArr[0] = {53, 2, 3, 5, 1};
//     debugArr[1] = {35, 2, 3, 4, 2};
//     debugArr[2] = {73, 2, 3, 3, 3};
//     debugArr[3] = {56, 2, 3, 2, 4};
//     debugArr[4] = {53, 2, 3, 1, 5};
//     debugArr[5] = {98, 3, 1, 5, 1};
//     debugArr[6] = {22, 3, 2, 4, 1};
//     debugArr[7] = {70, 3, 3, 3, 1};
//     debugArr[8] = {2, 3, 4, 2, 1};
//     debugArr[9] = {2, 3, 5, 1, 1};
//     debugArr[10] = {3, 4, 5, 1, 2};
//     debugArr[11] = {4, 5, 1, 2, 3};
//     debugArr[12] = {5, 1, 2, 3, 4};
//     MSD::internalSort(debugArr, debugLen);
//     for (int i = 0; i < debugLen; i++)
//     {
//         std::cout << debugArr[i].val[0] << " " << debugArr[i].val[1] << " " << debugArr[i].val[2] << " " << debugArr[i].val[3] << " " << debugArr[i].val[4] << std::endl;
//     }
//     return 0;
// }
