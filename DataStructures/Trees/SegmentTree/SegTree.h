//
// Created by 柠檬猪 on 2024/2/7.
//

#ifndef PLAYGROUND_SEGTREE_H
#define PLAYGROUND_SEGTREE_H

int lc(const int &x);

int rc(const int &x);

template<typename T>
struct TreeNode {
    T val, lazy;
};

template<typename T>
class SegTree {
private:
    int n;
    TreeNode<T> *tree;
public:
    explicit SegTree(int _size);

    ~SegTree();

    void build(T *a);

    void build(int _pos, int l, int r, T *a);

    void spread(int _pos, int _real_l, int real_r);

    void update(int update_l, int update_r, T v, int _pos, int _pos_l, int _pos_r);

    void update(int l, int r, T v);

    T query(int l, int r);

    T query(int query_l, int query_r, int _pos, int _pos_l, int _pos_r);
};

#include "SegTree.tpp"

#endif //PLAYGROUND_SEGTREE_H
