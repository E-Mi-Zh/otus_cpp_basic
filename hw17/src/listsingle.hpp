#pragma once
#include <cstddef>
#include <iostream>

template<typename T>
struct NodeSingle {
    NodeSingle<T>* next;
    T data;
};

template<typename T>
class MyListSingle {
public:
    void push_back(T value);
    void insert(T value, size_t pos);
    void erase(size_t pos);
    size_t size();
    T get(size_t pos);
    MyListSingle();
    ~MyListSingle();
    MyListSingle(const MyListSingle<T>& other);
    MyListSingle(MyListSingle<T>&& other);
    MyListSingle<T> &operator=(const MyListSingle<T> &rhs);
    MyListSingle<T> &operator=(MyListSingle<T> &&rhs);

    NodeSingle<T>* list_head();
    NodeSingle<T>* list_tail();
    T operator[](size_t pos) {
        return get(pos);
    }

    struct iterator {
        iterator(NodeSingle<T>* ptr);
        T &operator*();
        T get();
        iterator& operator++();
        bool operator!=(const iterator &other);
        bool operator==(const iterator &other);
    private:
        NodeSingle<T>* cur;
    };

    iterator begin();
    iterator end();

private:
    size_t listsize;            // данные
    NodeSingle<T>* head;           // последний вставленный элемент (голова односвязного списка)
    NodeSingle<T>* tail;           // первый вставленный элемент (хвост односвязного списка)
};

template<typename T>
std::ostream &operator<<(std::ostream &os, MyListSingle<T>& list);

#include "listsingle.tpp"
