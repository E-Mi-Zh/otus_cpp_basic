#include "../src/listsingle.h"
#include <gtest/gtest.h>

// создание контейнера
TEST(list, create) {
    // Arrange

    // Act

    // Assert
    EXPECT_NO_THROW({
        MyListSingle l;
    });

}

// получение размера контейнера (фактическое количество элементов)
TEST(list, get_size) {
    // Arrange
    MyListSingle l;
    const size_t expected = 0;

    // Act

    // Assert
    ASSERT_EQ(l.size(), expected);
}

// получение элементов из контейнера
TEST(list, get_value) {
    // Arrange
    MyListSingle l;
    const int value = 7;

    // Act
    l.push_back(value);

    // Assert
    ASSERT_EQ(l.get(0), value);
}

// вставка элементов в конец
TEST(list, push_back) {
    // Arrange
    MyListSingle l;
    const size_t expected = 1;

    // Act
    l.push_back(10);

    // Assert
    ASSERT_EQ(l.size(), expected);
}

// получение размера контейнера (фактическое количество элементов)
TEST(list, get_size_after_push_back) {
    // Arrange
    MyListSingle l;
    const size_t expected = 1;

    // Act
    l.push_back(10);

    // Assert
    ASSERT_EQ(l.size(), expected);
}

// вставка элементов в начало
TEST(list, insert_begin) {
    // Arrange
    MyListSingle l;
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
    MyListSingle l;
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
    MyListSingle l;
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
    MyListSingle l;
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
    MyListSingle l;
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
    MyListSingle l;
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
