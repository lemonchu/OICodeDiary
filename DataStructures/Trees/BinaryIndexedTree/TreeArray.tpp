//
// Created by 柠檬猪 on 2024/2/7.
//

template<typename T>
TreeArray<T>::TreeArray(unsigned int n): n(n){
    tree = new T[n];
}

template<typename T>
TreeArray<T>::~TreeArray() {
    delete[] tree;
}

template<typename T>
void TreeArray<T>::update(int i, T v) {
    for (; i <= n; i += i & -i) {
        tree[i] += v;
    }
}

template<typename T>
T TreeArray<T>::query(int i) {
    T sum = 0;
    for (; i > 0; i -= i & -i) {
        sum += tree[i];
    }
    return sum;
}

template<typename T>
T TreeArray<T>::query(int l, int r) {
    return query(r) - query(l - 1);
}