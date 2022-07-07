#include "API.h"
#include <iostream>

using namespace LSD;

typedef struct __node {
    Record data;
    struct __node *next;
} Node, *List;

Node *newNode(Record data) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->data = data;
    n->next = NULL;
    return n;
}

void allocate(List f[], List e[], Record *arr, int len, int key) {
    for (int i=0; i<len; i++) {
        int k = arr[i].val[key];
        if (f[k] == NULL)
            f[k] = e[k] = newNode(arr[i]);
        else {
            e[k]->next = newNode(arr[i]);
            e[k] = e[k]->next;
        }
    }
}

void collect(List f[], List e[], Record *arr, int len) {
    for (int i=0, l=0; i<VAL_RANGE+1 && l<len; i++) {
        while (f[i]) {
            arr[l++] = f[i]->data;
            f[i] = f[i]->next;
        }
    }
}

void LSD::radixSort(Record *arr, int len) {
    List f[VAL_RANGE+1], e[VAL_RANGE+1];
    for (int k=PRIORITY_NUM-1; k>=0; k--) {
        for (int i=0; i<VAL_RANGE+1; i++)
            f[i] = e[i] = NULL;
        allocate(f, e, arr, len, k);
        collect(f, e, arr, len);
    }
}

void LSD::internalSort(Record *arr, int len) {
    // TODO
}

// test
/*
int main() {
    Record arr[1000];
    int len = 13;
    arr[0] = {1, 2, 3, 5, 1};
    arr[1] = {1, 2, 3, 4, 2};
    arr[2] = {1, 2, 3, 3, 3};
    arr[3] = {1, 2, 3, 2, 4};
    arr[4] = {1, 2, 3, 1, 5};
    arr[5] = {2, 3, 1, 5, 1};
    arr[6] = {2, 3, 2, 4, 1};
    arr[7] = {2, 3, 3, 3, 1};
    arr[8] = {2, 3, 4, 2, 1};
    arr[9] = {2, 3, 5, 1, 1};
    arr[10] = {3, 4, 5, 1, 2};
    arr[11] = {4, 5, 1, 2, 3};
    arr[12] = {5, 1, 2, 3, 4};
    LSD::radixSort(arr, len);
    for (int i=0; i<len; i++) {
        std::cout << arr[i].val[0] << " " << arr[i].val[1] << " " << arr[i].val[2] << " " << arr[i].val[3] << " " << arr[i].val[4] << std::endl;
    }
    return 0;
}
*/
