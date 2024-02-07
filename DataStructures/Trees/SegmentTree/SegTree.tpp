//
// Created by 柠檬猪 on 2024/2/6.
//

int lc(const int &x) {
    return x << 1;
}

int rc(const int &x) {
    return x << 1 | 1;
}

template<typename T>
SegTree<T>::SegTree(int _size) : n(_size) {
    tree = new TreeNode<T>[4 * n];
}

template<typename T>
SegTree<T>::~SegTree() {
    delete[] tree;
}

template<typename T>
void SegTree<T>::build(T *a) {
    build(1, 1, n, a);
}

template<typename T>
void SegTree<T>::build(int _pos, int l, int r, T *a) {
    if (l == r) {
        tree[_pos].val = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(lc(_pos), l, mid, a);
    build(rc(_pos), mid + 1, r, a);
    tree[_pos].val = tree[lc(_pos)].val + tree[rc(_pos)].val;
}

template<typename T>
void SegTree<T>::spread(int _pos, int _real_l, int real_r) {
    if (tree[_pos].lazy != 0) {
        tree[lc(_pos)].lazy += tree[_pos].lazy;
        tree[rc(_pos)].lazy += tree[_pos].lazy;
        int mid = (_real_l + real_r) >> 1;
        tree[lc(_pos)].val += tree[_pos].lazy * (mid - _real_l + 1);
        tree[rc(_pos)].val += tree[_pos].lazy * (real_r - mid);
        tree[_pos].lazy = 0;
    }
}

template<typename T>
void SegTree<T>::update(int update_l, int update_r, T v, int _pos, int _pos_l, int _pos_r) {
    if (update_l > _pos_r || update_r < _pos_l) return;

    if (update_l <= _pos_l && _pos_r <= update_r) {
        tree[_pos].val += v * (_pos_r - _pos_l + 1);
        tree[_pos].lazy += v;
        return;
    }

    int mid = (_pos_l + _pos_r) >> 1;
    spread(_pos, _pos_l, _pos_r);

    update(update_l, update_r, v, lc(_pos), _pos_l, mid);
    update(update_l, update_r, v, rc(_pos), mid + 1, _pos_r);

    tree[_pos].val = tree[lc(_pos)].val + tree[rc(_pos)].val;
}

template<typename T>
void SegTree<T>::update(int l, int r, T v) {
    update(l, r, v, 1, 1, n);
}

template<typename T>
T SegTree<T>::query(int l, int r) {
    return query(l, r, 1, 1, n);
}

template<typename T>
T SegTree<T>::query(int query_l, int query_r, int _pos, int _pos_l, int _pos_r) {
    if (query_l > _pos_r || query_r < _pos_l) return 0;
    if (query_l <= _pos_l && _pos_r <= query_r) return tree[_pos].val;
    int mid = (_pos_l + _pos_r) >> 1;
    spread(_pos, _pos_l, _pos_r);
    return query(query_l, query_r, lc(_pos), _pos_l, mid) + query(query_l, query_r, rc(_pos), mid + 1, _pos_r);
}