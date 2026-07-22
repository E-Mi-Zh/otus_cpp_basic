#include "listdouble.h"

MyListDouble::MyListDouble() {
    this->listsize = 0;
    this->head = nullptr;
    this->tail = nullptr;
}

MyListDouble::~MyListDouble() {
    NodeDouble* node = this->tail;
    NodeDouble* prev_node;

    while (node != nullptr) {
        node->data = 0;
        prev_node = node;
        node = node->next;
        prev_node->next = nullptr;
        prev_node->prev = nullptr;
        delete prev_node;
    }

    this->listsize = 0;
    this->head = nullptr;
    this->tail = nullptr;
}

// Конструктор копирования
MyListDouble::MyListDouble(MyListDouble &other) {
    this->head = nullptr;
    this->tail = nullptr;
    this->listsize = 0;

    NodeDouble* node = other.tail;

    while (node != nullptr) {
        this->push_back(node->data);
        node = node->next;
    }
}

// Конструктор перемещения
MyListDouble::MyListDouble(MyListDouble&& other) {
    this->head = other.head;
    this->tail = other.tail;
    this->listsize = other.listsize;
    other.head = nullptr;
    other.tail = nullptr;
    other.listsize = 0;
}

// Copy assignment operator
MyListDouble &MyListDouble::operator=(MyListDouble &rhs) {
    MyListDouble temp{rhs};

    NodeDouble* thead = this->head;
    this->head = temp.head;
    temp.head = thead;

    NodeDouble* ttail = this->tail;
    this->tail = temp.tail;
    temp.tail = ttail;

    size_t tlistsize = this->listsize;
    this->listsize = temp.listsize;
    temp.listsize = tlistsize;

    return *this;
}

// Move assignment operator
MyListDouble &MyListDouble::operator=(MyListDouble &&rhs) {
    MyListDouble tmp{std::move(rhs)};
    return *this = tmp;
}

size_t MyListDouble::size() {
    return this->listsize;
}

void MyListDouble::push_back(int value) {
    NodeDouble* node = new NodeDouble{};    // создание нового узла

    node->data = value;
    node->next = nullptr;
    node->prev = this->head;

    if (this->tail == nullptr) {
        // сохраняем указатель на хвост только для первого вставленного элемента
        this->tail = node;
        // this->head = node;
    } else {
        // если в списке уже есть узлы, устанавливаем ссылку у предыдущего
        // узла на новый, только что созданный (последний)
        this->head->next = node;
    }

    this->head = node;
    this->listsize++;
}

void MyListDouble::insert(int value, size_t pos) {
    NodeDouble* node = this->tail;
    NodeDouble* new_node = new NodeDouble{};
    size_t ins_pos = 1;

    while ((node != nullptr) && (ins_pos != pos)) {
        ins_pos++;
        node = node->next;
    }

    if (node == nullptr) {
        // вставляем в самый конец
        new_node->data = value;
        new_node->next = nullptr;
        new_node->prev = this->head;
        this->head->next = new_node;
        this->head = new_node;
    } else {
        // Нам надо вставить новый элемент перед существующим
        new_node->data = value;
        new_node->next = node;
        new_node->prev = node->prev;
        if (this->tail == node) {
            this->tail = new_node;
            new_node->prev = nullptr;
        } else {
            node->prev->next = new_node;
        }
        node->prev = new_node;
        if (this->head == node) {
            this->head = new_node;
        }
    }
    this->listsize++;
}

void MyListDouble::erase(size_t pos) {
    NodeDouble* node = this->tail;
    size_t del_pos = 1;

    while ((node != nullptr) && (del_pos != pos)) {
        del_pos++;
        node = node->next;
    }
    if (node == this->head) {
        this->head = node->prev;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = nullptr;
    node->prev = nullptr;
    delete node;
    this->listsize--;
}

int MyListDouble::get(size_t pos) {
    NodeDouble* node;
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

NodeDouble* MyListDouble::list_head(){
    return this->head;
}

NodeDouble* MyListDouble::list_tail(){
    return this->tail;
}

std::ostream &operator<<(std::ostream &os, MyListDouble& list) {
    NodeDouble* node = list.list_tail();

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

MyListDouble::iterator::iterator(NodeDouble* ptr) {
    this->cur = ptr;
}

MyListDouble::iterator MyListDouble::begin() {
  return iterator(this->tail);
}

MyListDouble::iterator MyListDouble::end() {
  return iterator(nullptr);
}

NodeDouble &MyListDouble::iterator::operator*() {
    return *(this->cur);
}

NodeDouble MyListDouble::iterator::get() {
    return *(this->cur);
}
MyListDouble::iterator &MyListDouble::iterator::operator++() {
    this->cur = this->cur->next;

    return *this;
}

bool MyListDouble::iterator::operator!=(const iterator &other) {
    return this->cur != other.cur;
}
bool MyListDouble::iterator::operator==(const iterator &other) {
    return this->cur == other.cur;
}