#include "listsingle.h"

MyListSingle::MyListSingle() {
    this->listsize = 0;
    this->head = nullptr;
    this->tail = nullptr;
}

size_t MyListSingle::size() {
    return this->listsize;
}

void MyListSingle::push_back(int value) {
    NodeSingle* node = new NodeSingle{};    // создание нового узла

    if (this->tail == nullptr) {
        // сохраняем указатель на хвост только для первого вставленного элемента
        this->tail = node;
        this->head = node;
    }
    node->data = value;
    this->head->next = node;
    this->head = node;
    this->listsize++;
}

void MyListSingle::insert(int value, size_t pos) {
    NodeSingle* node = this->tail;
    NodeSingle* new_node = new NodeSingle{};
    size_t ins_pos = 1;

    while ((node != nullptr) && (ins_pos != pos)) {
        ins_pos++;
        node = node->next;
    }

    if (node == nullptr) {
        // вставляем в самый конец
        new_node->data = value;
        new_node->next = nullptr;
        this->head->next = new_node;
        this->head = new_node;
    } else {
        // Нам надо вставить новый элемент перед существующим
        // ссылки на предыдущий у нас нет
        // делаем финт: копируем в новый элемент существующий
        // а в текущий записываем искомое значение
        new_node->data = node->data;
        new_node->next = node->next;
        node->data = value;
        node->next = new_node;
        if (this->head == node) {
            this->head = new_node;
        }
    }
    this->listsize++;
}

void MyListSingle::erase(size_t pos) {
    NodeSingle* node = this->tail;
    NodeSingle* prev_node = this->tail;
    size_t del_pos = 1;

    while ((node != nullptr) && (del_pos != pos)) {
        del_pos++;
        prev_node = node;
        node = node->next;
    }
    prev_node->next = node->next;
    if (node == this->head) {
        this->head = prev_node;
    }
    node->next = nullptr;
    delete [] node;
    this->listsize--;
}

int MyListSingle::get(size_t pos) {
    NodeSingle* node;
    size_t cur_pos = 0;

    node = this->tail;
    while (node != nullptr) {
        if (cur_pos == pos) {
            return node->data;
        }
        node = node->next;
        cur_pos++;
    }
    return this->head->data;
}

NodeSingle* MyListSingle::list_head(){
    return this->head;
}

NodeSingle* MyListSingle::list_tail(){
    return this->tail;
}

std::ostream &operator<<(std::ostream &os, MyListSingle& list) {
    NodeSingle* node = list.list_tail();

    while (node != nullptr) {
        os << node->data;
        node = node->next;
        if (node != nullptr) {
             os << ", ";
        }
    }
    os << std::endl;

    return os;
}