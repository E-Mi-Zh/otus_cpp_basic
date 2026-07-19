#pragma once
#include <cstddef>
#include <iostream>

struct NodeSingle {
    NodeSingle* next;
    int data;
};

class MyListSingle {
public:
    void push_back(int value);
    void insert(int value, size_t pos);
    void erase(size_t pos);
    size_t size();
    int get(size_t pos);
    MyListSingle();
    ~MyListSingle();
    MyListSingle(MyListSingle& other);
    MyListSingle(MyListSingle&& other);
    MyListSingle &operator=(MyListSingle &rhs);
    MyListSingle &operator=(MyListSingle &&rhs);

    NodeSingle* list_head();
    NodeSingle* list_tail();
    int operator[](size_t pos) {
        return get(pos);
    }

private:
    size_t listsize;            // данные
    NodeSingle* head;           // последний вставленный элемент (голова односвязного списка)
    NodeSingle* tail;           // первый вставленный элемент (хвост односвязного списка)
};

std::ostream &operator<<(std::ostream &os, MyListSingle& list);
