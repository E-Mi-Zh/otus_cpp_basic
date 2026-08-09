#include "../src/listsingle.hpp"
#include <gtest/gtest.h>

// создание контейнера
TEST(list, create) {
    // Arrange

    // Act

    // Assert
    EXPECT_NO_THROW({
        MyListSingle<int> l;
    });

}

// получение размера контейнера (фактическое количество элементов)
TEST(list, get_size) {
    // Arrange
    MyListSingle<int> l;
    const size_t expected = 0;

    // Act

    // Assert
    ASSERT_EQ(l.size(), expected);
}

// получение элементов из контейнера
TEST(list, get_value) {
    // Arrange
    MyListSingle<int> l;
    const int value = 7;

    // Act
    l.push_back(value);

    // Assert
    ASSERT_EQ(l.get(0), value);
}

// вставка элементов в конец
TEST(list, push_back) {
    // Arrange
    MyListSingle<int> l;
    const size_t expected = 1;

    // Act
    l.push_back(10);

    // Assert
    ASSERT_EQ(l.size(), expected);
}

// получение размера контейнера (фактическое количество элементов)
TEST(list, get_size_after_push_back) {
    // Arrange
    MyListSingle<int> l;
    const size_t expected = 1;

    // Act
    l.push_back(10);

    // Assert
    ASSERT_EQ(l.size(), expected);
}

// вставка элементов в начало
TEST(list, insert_begin) {
    // Arrange
    MyListSingle<int> l;
    const size_t expected = 6;
    const int value = 7;
    const size_t pos = 0;

    for (size_t i = 0; i < (expected-1); i++) {
        l.push_back(i);
    }

    // Act
    l.insert(value, pos);

    // Assert
    ASSERT_EQ(l.size(), expected);
    ASSERT_EQ(l.get(pos), value);
}

// вставка элементов в середину
TEST(list, insert_middle) {
    // Arrange
    MyListSingle<int> l;
    const size_t expected = 6;
    const int value = 7;
    const size_t pos = 3;

    for (size_t i = 0; i < (expected-1); i++) {
        l.push_back(i);
    }

    // Act
    l.insert(value, pos);

    // Assert
    ASSERT_EQ(l.size(), expected);
    ASSERT_EQ(l.get(pos), value);
}

// вставка элементов в конец
TEST(list, insert_end) {
    // Arrange
    MyListSingle<int> l;
    const size_t expected = 6;
    const int value = 7;
    const size_t pos = 5;

    for (size_t i = 0; i < (expected-1); i++) {
        l.push_back(i);
    }

    // Act
    l.insert(value, pos);

    // Assert
    ASSERT_EQ(l.size(), expected);
    ASSERT_EQ(l.get(pos), value);
}

// удаление элементов из конца
TEST(list, delete_end) {
    // Arrange
    MyListSingle<int> l;
    const size_t expected = 5;

    for (size_t i = 0; i < (expected+1); i++) {
        l.push_back(i);
    }

    // Act
    l.erase(expected);

    // Assert
    ASSERT_EQ(l.size(), expected);
}

// удаление элементов из начала
TEST(list, delete_begin) {
    // Arrange
    MyListSingle<int> l;
    const size_t expected = 5;

    for (size_t i = 0; i < (expected+1); i++) {
        l.push_back(i);
    }

    // Act
    l.erase(0);

    // Assert
    ASSERT_EQ(l.size(), expected);
}

// удаление элементов из середины
TEST(list, delete_middle) {
    // Arrange
    MyListSingle<int> l;
    const size_t expected = 5;
    const size_t pos = 3;

    for (size_t i = 0; i < (expected+1); i++) {
        l.push_back(i);
    }

    // Act
    l.erase(pos);

    // Assert
    ASSERT_EQ(l.size(), expected);
}

// копирование контейнера
TEST(list, copy_constructor) {
    // Arrange
    MyListSingle<int> l1;
    const size_t sz = 5;
    // MyVector v2(sz);

    for (size_t i = 0; i < sz; i++) {
        l1.push_back(i);
    }

    // Act
    MyListSingle<int> l2 = l1;

    // Assert
    ASSERT_EQ(l1.size(), l2.size());
    for (size_t i = 0; i < sz; i++) {
        ASSERT_EQ(l1.get(i), l2.get(i));
    }
}

// удаление пустого контейнера
TEST(list, delete_constructor_empty) {
    EXPECT_NO_THROW({
        // Arrange
        MyListSingle<int> l;

        // Act

        // Assert
        // l.~MyListSingle<int>();
    });
}

// удаление контейнера с одним элементом
TEST(list, delete_constructor_one) {
    EXPECT_NO_THROW({
        // Arrange
        MyListSingle<int> l;

        // Act
        l.push_back(7);

        // Assert
        // l.~MyListSingle<int>();
    });
}

// удаление контейнера с несколькими элементами
TEST(list, delete_constructor_many) {
    EXPECT_NO_THROW({
        // Arrange
        MyListSingle<int> l;

        // Act
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        l.push_back(4);
        l.push_back(5);

        // Assert
        // l.~MyListSingle<int>();
    });
}

// Чтобы проверить фактическое удаление элементов наш контейнер должен
// поддерживать работу с чем-то сложнее чем int.
// Переписывать свои контейнеры не хочу, поэтому покажу на примере стандартного.

// удаление контейнера с проверкой удаления элементов
class CounterL {
    public:
        static int counter;
        CounterL() = default;
        ~CounterL() {
            counter++;
        }
};
int CounterL::counter;

TEST(list, delete_constructor_check_elements) {
    const int expected = 5;

    CounterL::counter = 0;
    {
        // Arrange
        MyListSingle<CounterL> l;

        // Act
        for (int i = 0; i < expected; i++) {
            l.push_back(CounterL());
        }

        // Assert
    }
    // умножаем на два, т.к. при использовании push_back создаётся временный объект
    ASSERT_EQ(CounterL::counter, expected * 2);
}

// перемещение контейнера
TEST(list, move_assignment) {
    // Arrange
    MyListSingle<int> l1;
    MyListSingle<int> l2;
    const int sz = 5;

    for (int i = 0; i < sz; i++) {
        l1.push_back(i);
    }

    for (int i = 0; i < (sz * 2); i++) {
        l2.push_back(i*10);
    }

    // Act
    l2 = std::move(l1);

    // Assert
    ASSERT_EQ(l2.size(), sz);
    for (int i = 0; i < sz; i++) {
        ASSERT_EQ(l2.get(i), i);
    }
    ASSERT_EQ(l1.size(), 0);
}