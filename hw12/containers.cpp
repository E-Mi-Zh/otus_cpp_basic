#include <iostream>
#include "list.h"
#include "vector.h"

int main() {
    MyVector values;

    for (int i = 0; i < 10; i++) {
        values.push_back(i);
    }

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

    return 0;
}