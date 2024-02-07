//
// Created by 柠檬猪 on 2024/2/7.
//
#include <cstdio>
#include <complex>
#include <vector>
#include "FFT.h"
using namespace std;

int main() {
    unsigned int n,m;
    scanf("%d %d", &n, &m);
    vector<Complex> a(n+1), b(m+1);
    for (int i = 0; i <= n; i++) {
        double x;
        scanf("%lf", &x);
        a[i] = Complex(x, 0);
    }
    for (int i = 0; i <= m; i++) {
        double x;
        scanf("%lf", &x);
        b[i] = Complex(x, 0);
    }
    unsigned int final_size = n+m+1;
    unsigned int final_pow = 0, tmp = final_size - 1;
    while (tmp) {
        tmp >>= 1;
        final_pow++;
    }
    final_pow = 1 << final_pow;
    resize_to_pow(a, final_pow);
    resize_to_pow(b, final_pow);

    FFT(a, false);
    FFT(b, false);
    for (unsigned int i = 0; i < final_pow; i++) {
        a[i] *= b[i];
    }

    FFT(a, true);
    for (unsigned int i = 0; i < final_size; i++) {
        printf("%d ", (int)(a[i].real() + 0.5));
    }

    return 0;
}
