//
// Created by 柠檬猪 on 2024/2/7.
//

#ifndef PLAYGROUND_FFT_H
#define PLAYGROUND_FFT_H

#include <complex>
#include <vector>

const double PI = acos(-1);
typedef std::complex<double> Complex;

void FFT(std::vector<Complex> &a, bool invert) {
    unsigned int n = a.size();
    if (n <= 1) {
        return;
    }
    std::vector<Complex> a0(n/2), a1(n/2);
    for (unsigned int i = 0; i < n/2; i++) {
        a0[i] = a[i*2];
        a1[i] = a[i*2+1];
    }
    FFT(a0, invert);
    FFT(a1, invert);
    double ang = 2*PI/n * (invert ? -1 : 1);
    Complex w(1), wn(cos(ang), sin(ang));

    for (unsigned int i = 0; i < n/2; i++) {
        a[i] = a0[i] + w*a1[i];
        a[i+n/2] = a0[i] - w*a1[i];
        if (invert) {
            a[i] /= 2;
            a[i+n/2] /= 2;
        }
        w *= wn;
    }
}

void resize_to_pow(std::vector<Complex> &a, unsigned int k) {
    unsigned int n = a.size();
    if (n < k) {
        a.resize(k);
    }
}

#endif //PLAYGROUND_FFT_H
