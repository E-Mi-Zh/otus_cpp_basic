#pragma once
#include <cstddef>
#include <iostream>

// Коэффициент расширения вектора при нехватке памяти
#define MY_VECTOR_COEF 1.5
// Размер по умолчанию
#define MY_VECTOR_INITIAL_SZ 10

class MyVector {
public:
    void push_back(int value);
    void insert(int value, size_t pos);
    void erase(size_t pos);
    size_t size();
    int get(size_t pos);
    MyVector(size_t size = MY_VECTOR_INITIAL_SZ);
    ~MyVector();
    MyVector(MyVector& other);
    MyVector(MyVector&& other);
    MyVector &operator=(MyVector &rhs);
    MyVector &operator=(MyVector &&rhs);

    int operator[](size_t pos) {
        return get(pos);
    };

    struct iterator {
        iterator(int* ptr);
        int &operator*();
        int get();
        iterator& operator++();
        bool operator!=(const iterator &other);
        bool operator==(const iterator &other);
    private:
        int* cur;
    };

    iterator begin();
    iterator end();

private:
    int* data;            // данные
    size_t vec_size;      // размер вектора, каким он был указан при создании
    size_t capacity;      // фактический размер области памяти для хранения данных
    size_t pos;           // текущая позиция для вставки элемента, "хвост"
};

std::ostream &operator<<(std::ostream &os, MyVector& vec);