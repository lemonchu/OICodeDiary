//
// Created by 柠檬猪 on 2024/2/7.
//

template<typename T>
EnhancedTreeArray<T>::EnhancedTreeArray(unsigned int _size) {
    n = _size;
    treeArray_a = new TreeArray<T>(n);
    treeArray_b = new TreeArray<T>(n);
}

template<typename T>
EnhancedTreeArray<T>::~EnhancedTreeArray() {
    delete treeArray_a;
    delete treeArray_b;
}

template<typename T>
void EnhancedTreeArray<T>::update(int l, int r, T v) {
    treeArray_a->update(l, v);
    treeArray_a->update(r + 1, -v);
    treeArray_b->update(l, v * l);
    treeArray_b->update(r + 1, -v * (r + 1));
}

template<typename T>
T EnhancedTreeArray<T>::query(int p) {
    return treeArray_a->query(p) * (p + 1) - treeArray_b->query(p);
}

template<typename T>
T EnhancedTreeArray<T>::query(int l, int r) {
    return query(r) - query(l - 1);
}