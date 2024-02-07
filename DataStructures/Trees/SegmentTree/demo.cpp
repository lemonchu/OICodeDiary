//
// Created by 柠檬猪 on 2024/2/6.
//

#include <cstdio>
#include <vector>
#include <iostream>
#include <cmath>

#include "SegTree.h"

typedef long long ll;

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    SegTree<ll> seg_tree(n);
    int a, b, c, d;

    ll *arr = new ll[n + 1];
    for (int i = 1; i <= n; i++)
        scanf("%lld", &arr[i]);
    seg_tree.build(arr);
    for (int i = 1; i <= m; i++) {
        scanf("%d", &a);
        if (a == 1) {
            scanf("%d%d%d", &b, &c, &d);
            seg_tree.update(b, c, d);
        } else {
            scanf("%d%d", &b, &c);
            printf("%lld\n", seg_tree.query(b, c));
        }
    }
    delete[] arr;
    return 0;
}
