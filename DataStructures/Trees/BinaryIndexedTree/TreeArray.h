//
// Created by 柠檬猪 on 2024/2/6.
//

#ifndef PLAYGROUND_TREEARRAY_H
#define PLAYGROUND_TREEARRAY_H

template<typename T>
class TreeArray {
private:
    int n;
    T *tree;
public:
    explicit TreeArray(unsigned int n);

    ~TreeArray();

    void update(int i, T v);

    T query(int i);

    T query(int l, int r);
};

#include "TreeArray.tpp"

#endif //PLAYGROUND_TREEARRAY_H