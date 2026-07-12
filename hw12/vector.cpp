#include "vector.h"
#include <iostream>

MyVector::MyVector(size_t size) {
    this->data = new int[static_cast<unsigned int>(size * MY_VECTOR_COEF)];
    this->vec_size = size;
    this->capacity = size * MY_VECTOR_COEF;
    this->pos = 0;
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
    // size_t new_size = this->pos++;
    if (this->pos == this->capacity) {
        this->capacity = static_cast<size_t>(this->capacity * MY_VECTOR_COEF);
        int* new_region = new int[this->capacity];      // новая область памяти
        for (size_t i = 0; i < this->pos; i++) {
            new_region[i] = this->data[i];              // копирование элементов
        }
        delete [] this->data;                           // удаление старой области
        this->data = new_region;                        // сохранение новой в мембер
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
    for (i = 0; i < (vec.size() - 1); i++) {
        os << vec.get(i) << ", ";
    }
    os << vec.get(i);
    return os;
}
