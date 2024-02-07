//
// Created by 柠檬猪 on 2024/2/6.
//
//
// Created by 柠檬猪 on 2024/2/5.
//
#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>
#include "EnhancedTreeArray.h"
#define ll long long

using namespace std;


ll a[100005], s[100005];
int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    EnhancedTreeArray<ll> enhancedTreeArray(n);
    s[0] = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &a[i]);
        s[i] = s[i - 1] + a[i];
        // enhancedTreeArray.update(i, i, a[i]);
    }
    for (int i = 1; i <= m; i++) {
        int x, l, r;
        ll v;
        scanf("%d", &x);
        if (x == 1) {
            scanf("%d %d %lld", &l, &r, &v);
            enhancedTreeArray.update(l, r, v);
        } else {
            scanf("%d %d", &l, &r);
            printf("%lld\n", enhancedTreeArray.query(l, r) + s[r] - s[l - 1]);
        }
    }
    return 0;
}