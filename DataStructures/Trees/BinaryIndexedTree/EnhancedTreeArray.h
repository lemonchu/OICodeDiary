//
// Created by 柠檬猪 on 2024/2/7.
//

// EnhancedTreeArray.h
#ifndef PLAYGROUND_ENHANCEDTREEARRAY_H
#define PLAYGROUND_ENHANCEDTREEARRAY_H

#include "TreeArray.h"

template<typename T>
class EnhancedTreeArray {
private:
    int n;
    TreeArray<T> *treeArray_a, *treeArray_b;
public:
    explicit EnhancedTreeArray(unsigned int _size);

    ~EnhancedTreeArray();

    void update(int l, int r, T v);

    T query(int p);

    T query(int l, int r);
};

#include "EnhancedTreeArray.tpp"

#endif //PLAYGROUND_ENHANCEDTREEARRAY_H