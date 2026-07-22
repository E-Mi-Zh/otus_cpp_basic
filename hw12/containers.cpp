#include <iostream>
#include "listsingle.h"
#include "listdouble.h"
#include "vector.h"

void test_vector() {
    MyVector values;

    for (int i = 0; i < 10; i++) {
        values.push_back(i);
    }

    std::cout << "<----------+= Проверяем контейнер вектор =+---------->" << std::endl;
    std::cout << "1. создание объекта контейнера для хранения объектов типа int" << std::endl;
    std::cout << "2. добавление в контейнер десяти элементов (0, 1 ... 9)" << std::endl;
    std::cout << "3. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t0, 1, 2, 3, 4, 5, 6, 7, 8, 9" << std::endl;
    std::cout << "\t\t\t" << values << std::endl;
    std::cout << "4. вывод размера контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат: 10" << std::endl;
    std::cout << "\t\t\t" << values.size() << std::endl;
    std::cout << "5. удаление третьего (по счёту), пятого и седьмого элементов" << std::endl;
    std::cout << "6. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t0, 1, 3, 5, 7, 8, 9" << std::endl;
    values.erase(3);
    values.erase(5);
    values.erase(7);
    std::cout << "\t\t\t" << values << std::endl;
    std::cout << "Такой результат получился вследствие того, что индексы при удалении пересчитываются." << std::endl;
    std::cout << "Как вариант можно удалять с конца, тогда результат совпадёт." << std::endl;
    MyVector values2;
    for (int i = 0; i < 10; i++) {
        values2.push_back(i);
    }
    values2.erase(7);
    values2.erase(5);
    values2.erase(3);
    std::cout << "   ожидаемый результат:\t0, 1, 3, 5, 7, 8, 9" << std::endl;
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "7. добавление элемента 10 в начало контейнера" << std::endl;
    std::cout << "8. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t10, 0, 1, 3, 5, 7, 8, 9" << std::endl;
    values2.insert(10, 1);
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "9. добавление элемента 20 в середину контейнера" << std::endl;
    std::cout << "10. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t10, 0, 1, 3, 20, 5, 7, 8, 9" << std::endl;
    values2.insert(20, 5);
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "11. добавление элемента 30 в конец контейнера" << std::endl;
    std::cout << "12. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t10, 0, 1, 3, 20, 5, 7, 8, 9, 30" << std::endl;
    values2.insert(30, 10);
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "13. вывод элемента контейнера с индексом 7 (container[7])" << std::endl;
    std::cout << "   ожидаемый результат:\t8" << std::endl;
    std::cout << "\t\t\t" << values2[7] << std::endl;
}

void test_list_single() {
    MyListSingle values;

    for (int i = 0; i < 10; i++) {
        values.push_back(i);
    }

    std::cout << "<----------+= Проверяем контейнер односвязный список =+---------->" << std::endl;
    std::cout << "1. создание объекта контейнера для хранения объектов типа int" << std::endl;
    std::cout << "2. добавление в контейнер десяти элементов (0, 1 ... 9)" << std::endl;
    std::cout << "3. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t0, 1, 2, 3, 4, 5, 6, 7, 8, 9" << std::endl;
    std::cout << "\t\t\t" << values << std::endl;
    std::cout << "4. вывод размера контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат: 10" << std::endl;
    std::cout << "\t\t\t" << values.size() << std::endl;
    std::cout << "5. удаление третьего (по счёту), пятого и седьмого элементов" << std::endl;
    std::cout << "6. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t0, 1, 3, 5, 7, 8, 9" << std::endl;
    values.erase(3);
    values.erase(5);
    values.erase(7);
    std::cout << "\t\t\t" << values << std::endl;
    std::cout << "Такой результат получился вследствие того, что индексы при удалении пересчитываются." << std::endl;
    std::cout << "Как вариант можно удалять с конца, тогда результат совпадёт." << std::endl;
    MyListSingle values2;
    for (int i = 0; i < 10; i++) {
        values2.push_back(i);
    }
    values2.erase(7);
    values2.erase(5);
    values2.erase(3);
    std::cout << "   ожидаемый результат:\t0, 1, 3, 5, 7, 8, 9" << std::endl;
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "7. добавление элемента 10 в начало контейнера" << std::endl;
    std::cout << "8. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t10, 0, 1, 3, 5, 7, 8, 9" << std::endl;
    values2.insert(10, 1);
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "9. добавление элемента 20 в середину контейнера" << std::endl;
    std::cout << "10. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t10, 0, 1, 3, 20, 5, 7, 8, 9" << std::endl;
    values2.insert(20, 5);
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "11. добавление элемента 30 в конец контейнера" << std::endl;
    std::cout << "12. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t10, 0, 1, 3, 20, 5, 7, 8, 9, 30" << std::endl;
    values2.insert(30, 10);
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "13. вывод элемента контейнера с индексом 7 (container[7])" << std::endl;
    std::cout << "   ожидаемый результат:\t8" << std::endl;
    std::cout << "\t\t\t" << values2[7] << std::endl;
}

void test_list_double() {
    MyListDouble values;

    for (int i = 0; i < 10; i++) {
        values.push_back(i);
    }

    std::cout << "<----------+= Проверяем контейнер двусвязный список =+---------->" << std::endl;
    std::cout << "1. создание объекта контейнера для хранения объектов типа int" << std::endl;
    std::cout << "2. добавление в контейнер десяти элементов (0, 1 ... 9)" << std::endl;
    std::cout << "3. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t0, 1, 2, 3, 4, 5, 6, 7, 8, 9" << std::endl;
    std::cout << "\t\t\t" << values << std::endl;
    std::cout << "4. вывод размера контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат: 10" << std::endl;
    std::cout << "\t\t\t" << values.size() << std::endl;
    std::cout << "5. удаление третьего (по счёту), пятого и седьмого элементов" << std::endl;
    std::cout << "6. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t0, 1, 3, 5, 7, 8, 9" << std::endl;
    values.erase(3);
    values.erase(5);
    values.erase(7);
    std::cout << "\t\t\t" << values << std::endl;
    std::cout << "Такой результат получился вследствие того, что индексы при удалении пересчитываются." << std::endl;
    std::cout << "Как вариант можно удалять с конца, тогда результат совпадёт." << std::endl;
    MyListDouble values2;
    for (int i = 0; i < 10; i++) {
        values2.push_back(i);
    }
    values2.erase(7);
    values2.erase(5);
    values2.erase(3);
    std::cout << "   ожидаемый результат:\t0, 1, 3, 5, 7, 8, 9" << std::endl;
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "7. добавление элемента 10 в начало контейнера" << std::endl;
    std::cout << "8. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t10, 0, 1, 3, 5, 7, 8, 9" << std::endl;
    values2.insert(10, 1);
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "9. добавление элемента 20 в середину контейнера" << std::endl;
    std::cout << "10. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t10, 0, 1, 3, 20, 5, 7, 8, 9" << std::endl;
    values2.insert(20, 5);
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "11. добавление элемента 30 в конец контейнера" << std::endl;
    std::cout << "12. вывод содержимого контейнера на экран" << std::endl;
    std::cout << "   ожидаемый результат:\t10, 0, 1, 3, 20, 5, 7, 8, 9, 30" << std::endl;
    values2.insert(30, 10);
    std::cout << "\t\t\t" << values2 << std::endl;
    std::cout << "13. вывод элемента контейнера с индексом 7 (container[7])" << std::endl;
    std::cout << "   ожидаемый результат:\t8" << std::endl;
    std::cout << "\t\t\t" << values2[7] << std::endl;
}

void test_vector_move() {
    MyVector vec1;

    for (int i = 0; i < 10; i++) {
        vec1.push_back(i);
    }

    std::cout << "<----------+= Проверяем работу перемещения для вектора =+---------->" << std::endl;

    std::cout << "1. Проверка перемещающего конструктора:" << std::endl;
    std::cout << "\tvec1: " << vec1 << std::endl;
    std::cout << "\tvec1 size: " << vec1.size() << std::endl;

    // Создаём через перемещающий конструктор
    MyVector vec2 = std::move(vec1);
    std::cout << "\tvec2 (после перемещения из vec1): " << vec2 << std::endl;
    std::cout << "\tvec2 size: " << vec2.size() << std::endl;
    std::cout << "\tvec1 size (после перемещения): " << vec1.size() << " (должен быть 0)" << std::endl;

    // ===== 2. Проверка перемещающего оператора присваивания =====
    std::cout << "2. Проверка перемещающего оператора присваивания:" << std::endl;

    MyVector vec3;
    for (int i = 0; i < 10; i++) {
        vec3.push_back(i);
    }

    MyVector vec4;
    for (int i = 10; i < 20; i++) {
        vec4.push_back(i);
    }
    std::cout << "\tvec3: " << vec3 << std::endl;
    std::cout << "\tvec4: " << vec4 << std::endl;

    // Перемещаем через оператор присваивания
    vec4 = std::move(vec3);
    std::cout << "Перемещаем оператором присваивания" << std::endl;
    std::cout << "\tvec4: " << vec4 << std::endl;
    std::cout << "\tvec3 size: " << vec3.size() << " (должен быть 0)" << std::endl;

    // ===== 3. Проверка работы с r-value ссылками (временные объекты) =====
    std::cout << "3. Проверка работы с r-value:" << std::endl;

    MyVector vec5;
    vec5 = MyVector(10);  // Присваиваем временный объект (r-value)

    for (int i = 20; i < 30; i++) {
        vec5.push_back(i);
    }
    std::cout << "\tпосле присваивания временного объекта: " << vec5 << std::endl;
}

void test_list_single_move() {
    MyListSingle lst1;

    for (int i = 0; i < 10; i++) {
        lst1.push_back(i);
    }

    std::cout << "<----------+= Проверяем работу перемещения для односвязного списка =+---------->" << std::endl;

    std::cout << "1. Проверка перемещающего конструктора:" << std::endl;
    std::cout << "\tlst1: " << lst1 << std::endl;
    std::cout << "\tlst1 size: " << lst1.size() << std::endl;

    // Создаём через перемещающий конструктор
    MyListSingle lst2 = std::move(lst1);
    std::cout << "\tlst2 (после перемещения из lst1): " << lst2 << std::endl;
    std::cout << "\tlst2 size: " << lst2.size() << std::endl;
    std::cout << "\tlst1 size (после перемещения): " << lst1.size() << " (должен быть 0)" << std::endl;

    // ===== 2. Проверка перемещающего оператора присваивания =====
    std::cout << "2. Проверка перемещающего оператора присваивания:" << std::endl;

    MyListSingle lst3;
    for (int i = 0; i < 10; i++) {
        lst3.push_back(i);
    }

    MyListSingle lst4;
    for (int i = 10; i < 20; i++) {
        lst4.push_back(i);
    }
    std::cout << "\tlst3: " << lst3 << std::endl;
    std::cout << "\tlst4: " << lst4 << std::endl;

    // Перемещаем через оператор присваивания
    lst4 = std::move(lst3);
    std::cout << "Перемещаем оператором присваивания" << std::endl;
    std::cout << "\tlst4: " << lst4 << std::endl;
    std::cout << "\tlst3 size: " << lst3.size() << " (должен быть 0)" << std::endl;

    // ===== 3. Проверка работы с r-value ссылками (временные объекты) =====
    std::cout << "3. Проверка работы с r-value:" << std::endl;

    MyListSingle lst5;
    lst5 = MyListSingle();  // Присваиваем временный объект (r-value)

    for (int i = 20; i < 30; i++) {
        lst5.push_back(i);
    }
    std::cout << "\tпосле присваивания временного объекта: " << lst5 << std::endl;
}

void test_list_double_move() {
    MyListDouble lst1;

    for (int i = 0; i < 10; i++) {
        lst1.push_back(i);
    }

    std::cout << "<----------+= Проверяем работу перемещения для двусвязного списка =+---------->" << std::endl;

    std::cout << "1. Проверка перемещающего конструктора:" << std::endl;
    std::cout << "\tlst1: " << lst1 << std::endl;
    std::cout << "\tlst1 size: " << lst1.size() << std::endl;

    // Создаём через перемещающий конструктор
    MyListDouble lst2 = std::move(lst1);
    std::cout << "\tlst2 (после перемещения из lst1): " << lst2 << std::endl;
    std::cout << "\tlst2 size: " << lst2.size() << std::endl;
    std::cout << "\tlst1 size (после перемещения): " << lst1.size() << " (должен быть 0)" << std::endl;

    // ===== 2. Проверка перемещающего оператора присваивания =====
    std::cout << "2. Проверка перемещающего оператора присваивания:" << std::endl;

    MyListDouble lst3;
    for (int i = 0; i < 10; i++) {
        lst3.push_back(i);
    }

    MyListDouble lst4;
    for (int i = 10; i < 20; i++) {
        lst4.push_back(i);
    }
    std::cout << "\tlst3: " << lst3 << std::endl;
    std::cout << "\tlst4: " << lst4 << std::endl;

    // Перемещаем через оператор присваивания
    lst4 = std::move(lst3);
    std::cout << "Перемещаем оператором присваивания" << std::endl;
    std::cout << "\tlst4: " << lst4 << std::endl;
    std::cout << "\tlst3 size: " << lst3.size() << " (должен быть 0)" << std::endl;

    // ===== 3. Проверка работы с r-value ссылками (временные объекты) =====
    std::cout << "3. Проверка работы с r-value:" << std::endl;

    MyListDouble lst5;
    lst5 = MyListDouble();  // Присваиваем временный объект (r-value)

    for (int i = 20; i < 30; i++) {
        lst5.push_back(i);
    }
    std::cout << "\tпосле присваивания временного объекта: " << lst5 << std::endl;
}

void test_vector_iterator() {
    MyVector values;
    // заполнение контейнера элементами
    for (int i = 0; i < 10; i++) {
        values.push_back(i);
    }
    std::cout << "<----------+= Проверяем работу итератора для вектора =+---------->" << std::endl;
    for (auto iter = values.begin(); iter != values.end(); ++iter) {
        std::cout << *iter << " ";
    // или:
    // std::cout << iter.get() << std::endl;
    }
    std::cout << std::endl;
}

void test_list_single_iterator() {
    MyListSingle values;
    // заполнение контейнера элементами
    for (int i = 0; i < 10; i++) {
        values.push_back(i);
    }
    std::cout << "<----------+= Проверяем работу итератора для односвязного списка =+---------->" << std::endl;
    for (auto iter = values.begin(); iter != values.end(); ++iter) {
        std::cout << *iter << " ";
    // или:
    // std::cout << iter.get() << std::endl;
    }
    std::cout << std::endl;
}

void test_list_double_iterator() {
    MyListDouble values;
    // заполнение контейнера элементами
    for (int i = 0; i < 10; i++) {
        values.push_back(i);
    }
    std::cout << "<----------+= Проверяем работу итератора для двусвязного списка =+---------->" << std::endl;
    for (auto iter = values.begin(); iter != values.end(); ++iter) {
        std::cout << *iter << " ";
    // или:
    // std::cout << iter.get() << std::endl;
    }
    std::cout << std::endl;
}

// Проверяем удаление граничных элементов в списках
void test_fix_2() {
    MyListSingle values;

    for (int i = 0; i < 10; i++) {
        values.push_back(i);
    }
    // Проверяем удаление первого элемента
    std::cout << "Проверяем односвязный список" << std::endl;
    std::cout << "Удаляем первый элемент" << std::endl;
    std::cout << "\tДо удаления: " << values << std::endl;
    values.erase(1);
    std::cout << "\tПосле удаления: " << values << std::endl;
    std::cout << "Удаляем последний элемент" << std::endl;
    std::cout << "\tДо удаления: " << values << std::endl;
    values.erase(9);
    std::cout << "\tПосле удаления: " << values << std::endl;


    MyListDouble values2;

    for (int i = 0; i < 10; i++) {
        values2.push_back(i);
    }
    // Проверяем удаление первого элемента
    std::cout << "Проверяем двусвязный список" << std::endl;
    std::cout << "Удаляем первый элемент" << std::endl;
    std::cout << "\tДо удаления: " << values2 << std::endl;
    values2.erase(1);
    std::cout << "\tПосле удаления: " << values2 << std::endl;
    std::cout << "Удаляем последний элемент" << std::endl;
    std::cout << "\tДо удаления: " << values2 << std::endl;
    values2.erase(9);
    std::cout << "\tПосле удаления: " << values2 << std::endl;
}

void test_fix_3() {
    MyVector values;

    for (int i = 0; i < 10; i++) {
        values.push_back(i);
    }
    std::cout << "Проверяем вставку в вектор" << std::endl;
    std::cout << "Вставляем перед первым элементом" << std::endl;
    std::cout << "\tДо вставки: " << values << std::endl;
    values.insert(77, 1);
    std::cout << "\tПосле вставки: " << values << std::endl;
    std::cout << "\tТеперь с pos=0: " << values << std::endl;
    values.insert(88, 0);
    std::cout << "\tПосле вставки: " << values << std::endl;

    MyVector values2;

    std::cout << "Проверяем вставку в пустой вектор" << std::endl;
    std::cout << "Вставляем перед первым элементом" << std::endl;
    std::cout << "\tДо вставки c pos=0: " << values2 << std::endl;
    values2.insert(77, 0);
    std::cout << "\tПосле вставки: " << values2 << std::endl;
    std::cout << "\tТеперь с pos=1: " << values2 << std::endl;
    values2.insert(88, 0);
    std::cout << "\tПосле вставки: " << values2 << std::endl;


}

int main() {
    test_vector();
    test_list_single();
    test_list_double();
    test_vector_move();
    test_list_single_move();
    test_list_double_move();
    test_vector_iterator();
    test_list_single_iterator();
    test_list_double_iterator();

    test_fix_2();
    test_fix_3();
    return 0;
}