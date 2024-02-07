//
// Created by 柠檬猪 on 2024/2/7.
//
#include <cstdio>
#include <iostream>

#include "TreeArray.h"

int main() {
    int n, m;
    int eax, ebx;
    scanf("%d %d", &n, &m);

    TreeArray<long long> treeArray(n);

    for (int i = 1; i <= n; i++) {
        scanf("%d", &eax);
        treeArray.update(i, eax);
    }
    for (int i = 1; i <= m; i++) {
        int x;
        scanf("%d", &x);
        scanf("%d %d", &eax, &ebx);
        if (x == 1) {
            treeArray.update(eax, ebx);
        } else {
            printf("%lld\n", treeArray.query(eax, ebx));
        }
    }
    return 0;
}