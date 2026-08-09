#pragma once
#include <cstddef>
#include <iostream>

// Коэффициент расширения вектора при нехватке памяти
#define MY_VECTOR_COEF 1.5
// Размер по умолчанию
#define MY_VECTOR_INITIAL_SZ 10

template<typename T>
class MyVector {
public:
    void push_back(T value);
    void insert(T value, size_t pos);
    void erase(size_t pos);
    size_t size();
    T get(size_t pos);
    MyVector(size_t size = MY_VECTOR_INITIAL_SZ);
    ~MyVector();
    MyVector(const MyVector<T>& other);
    MyVector(MyVector<T>&& other);
    MyVector<T> &operator=(const MyVector<T> &rhs);
    MyVector<T> &operator=(MyVector<T> &&rhs);

    T operator[](size_t pos) {
        return get(pos);
    };

    struct iterator {
        iterator(T* ptr);
        T &operator*();
        T get();
        iterator& operator++();
        bool operator!=(const iterator &other);
        bool operator==(const iterator &other);
    private:
        T* cur;
    };

    iterator begin();
    iterator end();

private:
    T* data;            // данные
    size_t vec_size;      // размер вектора, каким он был указан при создании
    size_t capacity;      // фактический размер области памяти для хранения данных
    size_t pos;           // текущая позиция для вставки элемента, "хвост"
};

template<typename T>
std::ostream &operator<<(std::ostream &os, MyVector<T>& vec);

#include "vector.tpp"