template<typename T>
MyListSingle<T>::MyListSingle() {
    this->listsize = 0;
    this->tail = nullptr;
    this->head = nullptr;
}

template<typename T>
MyListSingle<T>::~MyListSingle() {
    NodeSingle<T>* node = this->head;
    NodeSingle<T>* prev_node;

    while (node != nullptr) {
        prev_node = node;
        node = node->next;
        prev_node->next = nullptr;
        delete prev_node;
    }

    this->listsize = 0;
    this->tail = nullptr;
    this->head = nullptr;
}

// Конструктор копирования
template<typename T>
MyListSingle<T>::MyListSingle(const MyListSingle<T> &other) {
    this->tail = nullptr;
    this->head = nullptr;
    this->listsize = 0;

    NodeSingle<T>* node = other.head;

    while (node != nullptr) {
        this->push_back(node->data);
        node = node->next;
    }
}

// Конструктор перемещения
template<typename T>
MyListSingle<T>::MyListSingle(MyListSingle<T>&& other) {
    this->tail = other.tail;
    this->head = other.head;
    this->listsize = other.listsize;
    other.tail = nullptr;
    other.head = nullptr;
    other.listsize = 0;
}

// Copy assignment operator
template<typename T>
MyListSingle<T> &MyListSingle<T>::operator=(const MyListSingle<T> &rhs) {
    MyListSingle<T> temp{rhs};

    std::swap(this->tail, temp.tail);
    std::swap(this->head, temp.head);
    std::swap(this->listsize, temp.listsize);

    return *this;
}

// Move assignment operator
template<typename T>
MyListSingle<T> &MyListSingle<T>::operator=(MyListSingle<T> &&rhs) {
    NodeSingle<T>* node = this->head;
    NodeSingle<T>* prev_node;

    if (this != &rhs) {
        while (node != nullptr) {
            prev_node = node;
            node = node->next;
            delete prev_node;
        }

        this->tail = rhs.tail;
        this->head = rhs.head;
        this->listsize = rhs.listsize;

        rhs.tail = nullptr;
        rhs.head = nullptr;
        rhs.listsize = 0;
    }
    return *this;
}

template<typename T>
size_t MyListSingle<T>::size() {
    return this->listsize;
}

template<typename T>
void MyListSingle<T>::push_back(T value) {
    NodeSingle<T>* node = new NodeSingle<T>{};    // создание нового узла

    if (this->head == nullptr) {
        // сохраняем указатель на хвост только для первого вставленного элемента
        this->head = node;
        this->tail = node;
    } else {
        this->tail->next = node;
        this->tail = node;
    }
    node->data = value;
    this->listsize++;
}

template<typename T>
void MyListSingle<T>::insert(T value, size_t pos) {
    NodeSingle<T>* node = this->head;
    NodeSingle<T>* new_node = new NodeSingle<T>{};
    size_t ins_pos = 0;

    while ((node != nullptr) && (ins_pos != pos)) {
        node = node->next;
        ins_pos++;
    }

    if (node == nullptr) {
        // вставляем в самый конец
        new_node->data = value;
        new_node->next = nullptr;
        this->tail->next = new_node;
        this->tail = new_node;
    } else {
        // Нам надо вставить новый элемент перед существующим
        // ссылки на предыдущий у нас нет
        // делаем финт: копируем в новый элемент существующий
        // а в текущий записываем искомое значение
        new_node->data = node->data;
        new_node->next = node->next;
        node->data = value;
        node->next = new_node;
        if (this->tail == node) {
            this->tail = new_node;
        }
    }
    this->listsize++;
}

template<typename T>
void MyListSingle<T>::erase(size_t pos) {
    NodeSingle<T>* node = this->head;
    NodeSingle<T>* prev_node = this->head;
    size_t del_pos = 0;

    if (this->listsize == 0) {
        // в пустом списке и удалять нечего
        return;
    }

    if (pos > (this->listsize - 1)) {
        pos = this->listsize - 1;
    }
    while ((node != nullptr) && (del_pos != pos)) {
        prev_node = node;
        node = node->next;
        del_pos++;
    }

    if (node == this->tail) {
        this->tail = prev_node;
        this->tail->next = nullptr;
    } else {
        prev_node->next = node->next;
    }
    if (node == this->head) {
        this->head = node->next;
    }
    node->next = nullptr;
    delete node;
    this->listsize--;
}

template<typename T>
T MyListSingle<T>::get(size_t pos) {
    NodeSingle<T>* node;
    size_t cur_pos = 0;

    node = this->head;
    while (node != nullptr) {
        if (cur_pos == pos) {
            return node->data;
        }
        node = node->next;
        cur_pos++;
    }
    return this->tail->data;
}

template<typename T>
NodeSingle<T>* MyListSingle<T>::list_head(){
    return this->tail;
}

template<typename T>
NodeSingle<T>* MyListSingle<T>::list_tail(){
    return this->head;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, MyListSingle<T>& list) {
    NodeSingle<T>* node = list.list_tail();

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

template<typename T>
MyListSingle<T>::iterator::iterator(NodeSingle<T>* ptr) {
    this->cur = ptr;
}

template<typename T>
typename MyListSingle<T>::iterator MyListSingle<T>::begin() {
    return iterator(this->head);
}

template<typename T>
typename MyListSingle<T>::iterator MyListSingle<T>::end() {
    return iterator(nullptr);
}

template<typename T>
T &MyListSingle<T>::iterator::operator*() {
    if (this->cur == nullptr) {
        std::cout << "Ошибка при работе с итератором: доступ по нулевому указателю!" << std::endl;
        std::cout << "Завершаем программу!" << std::endl;
        exit(-1);
    } else {
        return this->cur->data;
    }
}

template<typename T>
T MyListSingle<T>::iterator::get() {
    if (this->cur == nullptr) {
        std::cout << "Ошибка при работе с итератором: доступ по нулевому указателю!" << std::endl;
        std::cout << "Возвращаем ноль!" << std::endl;
        return 0;
    } else {
        return this->cur->data;
    }
}

template<typename T>
typename MyListSingle<T>::iterator &MyListSingle<T>::iterator::operator++() {
    this->cur = this->cur->next;

    return *this;
}

template<typename T>
bool MyListSingle<T>::iterator::operator!=(const iterator &other) {
    return this->cur != other.cur;
}

template<typename T>
bool MyListSingle<T>::iterator::operator==(const iterator &other) {
    return this->cur == other.cur;
}