#pragma once
#include <cstddef>
#include <iostream>

struct NodeDouble {
    NodeDouble* next;
    NodeDouble* prev;
    int data;
};

class MyListDouble {
public:
    void push_back(int value);
    void insert(int value, size_t pos);
    void erase(size_t pos);
    size_t size();
    int get(size_t pos);
    MyListDouble();
    NodeDouble* list_head();
    NodeDouble* list_tail();
    int operator[](size_t pos) {
        return get(pos);
    }

private:
    size_t listsize;            // данные
    NodeDouble* head;           // последний вставленный элемент (голова двусвязного списка)
    // первый вставленный элемент (хвост двусвязного списка)
    // в принципе для двусвязного списка можно обойтись и без него
    // тогда в случаях когда нам нужен обход от начала можно было бы пробегать от головы
    // и затем печатать (перебирать) от хвоста
    // оставил чтобы меньше переделывать код от односвязного
    NodeDouble* tail;
};

std::ostream &operator<<(std::ostream &os, MyListDouble& list);
