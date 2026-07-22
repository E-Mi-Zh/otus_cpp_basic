#include "vector.h"
#include <iostream>

MyVector::MyVector(size_t size) {
    this->data = new int[static_cast<unsigned int>(size * MY_VECTOR_COEF)];
    this->vec_size = size;
    this->capacity = size * MY_VECTOR_COEF;
    this->pos = 0;
}

MyVector::~MyVector() {
    this->vec_size = 0;
    this->capacity = 0;
    this->pos = 0;
    delete [] this->data;
}

// Конструктор копирования
MyVector::MyVector(MyVector &other) {
    this->vec_size = other.vec_size;
    this->capacity = other.capacity;
    this->pos = other.pos;
    this->data = new int[this->capacity];

    for (size_t i = 0; i < this->pos; i++) {
        this->data[i] = other.data[i];
    }
}

// Конструктор перемещения
MyVector::MyVector(MyVector&& other) {
    this->data = other.data;
    this->vec_size = other.vec_size;
    this->capacity = other.capacity;
    this->pos = other.pos;
    other.data = nullptr;
    other.vec_size = 0;
    other.capacity = 0;
    other.pos = 0;
}

// Copy assignment operator
MyVector &MyVector::operator=(MyVector &rhs) {
    MyVector temp{rhs};

    int* tdata = this->data;
    this->data = temp.data;
    temp.data = tdata;

    size_t tvec_size = this->vec_size;
    this->vec_size = temp.vec_size;
    temp.vec_size = tvec_size;

    size_t tcapacity = this->capacity;
    this->capacity = temp.capacity;
    temp.capacity = tcapacity;

    size_t tpos = this->pos;
    this->pos = temp.pos;
    temp.pos = tpos;

    return *this;
}

// Move assignment operator
MyVector &MyVector::operator=(MyVector &&rhs) {
    MyVector tmp{std::move(rhs)};
    return *this = tmp;
}

size_t MyVector::size() {
    return this->pos;
}

void MyVector::push_back(int value) {
    // Если текущая позиция для вставки подошла к границе доступной памяти
    // увеличиваем вектор с учётом коэффициента расширения
    if (this->pos == this->capacity) {
        this->capacity = static_cast<size_t>(this->capacity * MY_VECTOR_COEF);
        int* new_region = new int[this->capacity];      // новая область памяти
        for (size_t i = 0; i < this->pos; i++) {
            new_region[i] = this->data[i];              // копирование элементов
        }
        delete [] this->data;                           // удаление старой области
        this->data = new_region;                        // сохранение новой в мембер
    }
    this->data[pos] = value;
    this->pos++;
    this->vec_size++;
}

void MyVector::insert(int value, size_t pos) {
    if (this->vec_size == 0) {
        this->push_back(value);
        return;
    }

    if (pos > this->pos) {
        // вставляем в конец
        pos = this->pos;
    }

    if (this->pos == this->capacity) {
        this->capacity = static_cast<size_t>(this->capacity * MY_VECTOR_COEF);
        int* new_region = new int[this->capacity];      // новая область памяти
        for (size_t i = 0; i < this->pos; i++) {
            new_region[i] = this->data[i];              // копирование элементов
        }
        delete [] this->data;                           // удаление старой области
        this->data = new_region;                        // сохранение новой в мембер
    }

    if (pos == 0) {
        // Т.к. мы оперируем порядковым номером элемента, а не его
        // индексом, то pos - это номер элемента, перед которым мы вставляем.
        // Для непустого вектора (пустой мы уже обработали) всегда будет первый элемент
        pos = 1;
    }

    // Пропускаем все элементы до вставляемой позиции
    // Затем идём с конца и до искомого индекса, меняя местами
    // последний (неинициализированный элемент и предшествующий)
    for (size_t i = this->pos; i > pos-1; i--) {
        this->data[i] = this->data[i-1];
    }
    // Можно сохранять новое значение
    this->data[pos-1] = value;
    this->vec_size++;
    this->pos++;
}

void MyVector::erase(size_t pos) {
    // Пропускаем все элементы до удаляемой позиции
    // Затем идём от искомого индекса и до конца, меняя местами
    // текущий элемент и следующий
    for (size_t i = pos-1; i < this->pos; i++) {
        this->data[i] = this->data[i+1];
    }
    this->vec_size--;
    this->pos--;
}

int MyVector::get(size_t pos) {
    return this->data[pos];
}

std::ostream &operator<<(std::ostream &os, MyVector& vec) {
    size_t i;
    if (vec.size() == 0) {
        // в пустом контейнере печатать нечего
        return os;
    }
    for (i = 0; i < (vec.size() - 1); i++) {
        os << vec.get(i) << ", ";
    }
    os << vec.get(i);
    return os;
}


MyVector::iterator::iterator(int* ptr) {
    this->cur = ptr;
}

MyVector::iterator MyVector::begin() {
  return iterator(this->data);
}

MyVector::iterator MyVector::end() {
  return iterator(this->data + this->pos);
}

int &MyVector::iterator::operator*() {
    return *(this->cur);
}

int MyVector::iterator::get() {
    return *(this->cur);
}
MyVector::iterator &MyVector::iterator::operator++() {
    this->cur++;

    return *this;
}

bool MyVector::iterator::operator!=(const iterator &other) {
    return this->cur != other.cur;
}
bool MyVector::iterator::operator==(const iterator &other) {
    return this->cur == other.cur;
}
