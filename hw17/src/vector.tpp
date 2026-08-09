template<typename T>
MyVector<T>::MyVector(size_t size) {
    this->vec_size = size;
    this->capacity = static_cast<unsigned int>(size * MY_VECTOR_COEF);
    this->data = new T[capacity];
    this->pos = 0;
}

template<typename T>
MyVector<T>::~MyVector() {
    this->vec_size = 0;
    this->capacity = 0;
    this->pos = 0;
    delete [] this->data;
}

// Конструктор копирования
template<typename T>
MyVector<T>::MyVector(const MyVector &other) {
    this->vec_size = other.vec_size;
    this->capacity = other.capacity;
    this->pos = other.pos;
    this->data = new T[this->capacity];

    for (size_t i = 0; i < this->pos; i++) {
        this->data[i] = other.data[i];
    }
}

// Конструктор перемещения
template<typename T>
MyVector<T>::MyVector(MyVector<T>&& other) {
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
template<typename T>
MyVector<T> &MyVector<T>::operator=(const MyVector<T> &rhs) {
    MyVector<T> temp{rhs};

    std::swap(this->data, temp.data);
    std::swap(this->vec_size, temp.vec_size);
    std::swap(this->capacity, temp.capacity);
    std::swap(this->pos, temp.pos);

    return *this;
}


// Move assignment operator
template<typename T>
MyVector<T> &MyVector<T>::operator=(MyVector<T> &&rhs) {
    if (this != &rhs) {
        delete [] this->data;
        this->data = rhs.data;
        this->vec_size = rhs.vec_size;
        this->capacity = rhs.capacity;
        this->pos = rhs.pos;

        rhs.data = nullptr;
        rhs.capacity = 0;
        rhs.pos = 0;
        rhs.vec_size = 0;
    }
    return *this;
}

template<typename T>
size_t MyVector<T>::size() {
    return this->pos;
}

template<typename T>
void MyVector<T>::push_back(T value) {
    // Если текущая позиция для вставки подошла к границе доступной памяти
    // увеличиваем вектор с учётом коэффициента расширения
    if (this->pos == this->capacity) {
        std::cout << "realloc!\n";
        this->capacity = static_cast<size_t>(this->capacity * MY_VECTOR_COEF);
        T* new_region = new T[this->capacity];      // новая область памяти
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

template<typename T>
void MyVector<T>::insert(T value, size_t pos) {
    // В векторе ещё не было ни одного элемента, просто добавляем новый в конец
    if (this->pos == 0) {
        this->push_back(value);
        return;
    }

    if (pos > this->pos) {
        // вставляем в конец
        pos = this->pos;
    }

    if (this->pos == this->capacity) {
        this->capacity = static_cast<size_t>(this->capacity * MY_VECTOR_COEF);
        T* new_region = new T[this->capacity];      // новая область памяти
        for (size_t i = 0; i < this->pos; i++) {
            new_region[i] = this->data[i];              // копирование элементов
        }
        delete [] this->data;                           // удаление старой области
        this->data = new_region;                        // сохранение новой в мембер
    }

    // Пропускаем все элементы до вставляемой позиции
    // Затем идём с конца и до искомого индекса, меняя местами
    // последний (неинициализированный элемент и предшествующий)
    for (size_t i = this->pos; i > pos; i--) {
        this->data[i] = this->data[i-1];
    }
    // Можно сохранять новое значение
    this->data[pos] = value;
    this->vec_size++;
    this->pos++;
}

template<typename T>
void MyVector<T>::erase(size_t pos) {
    // Пропускаем все элементы до удаляемой позиции
    // Затем идём от искомого индекса и до конца, меняя местами
    // текущий элемент и следующий
    for (size_t i = pos; i < this->pos; i++) {
        this->data[i] = this->data[i+1];
    }
    this->vec_size--;
    this->pos--;
}

template<typename T>
T MyVector<T>::get(size_t pos) {
    return this->data[pos];
}

template<typename T>
std::ostream &operator<<(std::ostream &os, MyVector<T>& vec) {
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

template<typename T>
MyVector<T>::iterator::iterator(T* ptr) {
    this->cur = ptr;
}

template<typename T>
typename MyVector<T>::iterator MyVector<T>::begin() {
  return iterator(this->data);
}

template<typename T>
typename MyVector<T>::iterator MyVector<T>::end() {
  return iterator(this->data + this->pos);
}

template<typename T>
T &MyVector<T>::iterator::operator*() {
    return *(this->cur);
}

template<typename T>
T MyVector<T>::iterator::get() {
    return *(this->cur);
}

template<typename T>
typename MyVector<T>::iterator &MyVector<T>::iterator::operator++() {
    this->cur++;

    return *this;
}

template<typename T>
bool MyVector<T>::iterator::operator!=(const iterator &other) {
    return this->cur != other.cur;
}

template<typename T>
bool MyVector<T>::iterator::operator==(const iterator &other) {
    return this->cur == other.cur;
}
