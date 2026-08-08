#include "../src/vector.h"
#include <gtest/gtest.h>

// создание контейнера
TEST(vector, create) {
    // Arrange

    // Act

    // Assert
    EXPECT_NO_THROW({
        MyVector v;
    });

}

// получение размера контейнера (фактическое количество элементов)
TEST(vector, get_size) {
    // Arrange
    MyVector v;
    const size_t expected = 0;

    // Act

    // Assert
    ASSERT_EQ(v.size(), expected);
}

// получение элементов из контейнера
TEST(vector, get_value) {
    // Arrange
    MyVector v;
    const int value = 7;

    // Act
    v.push_back(value);

    // Assert
    ASSERT_EQ(v.get(0), value);
}

// вставка элементов в конец
TEST(vector, push_back) {
    // Arrange
    MyVector v;
    const size_t expected = 1;

    // Act
    v.push_back(10);

    // Assert
    ASSERT_EQ(v.size(), expected);
}

// получение размера контейнера (фактическое количество элементов)
TEST(vector, get_size_after_push_back) {
    // Arrange
    MyVector v;
    const size_t expected = 1;

    // Act
    v.push_back(10);

    // Assert
    ASSERT_EQ(v.size(), expected);
}

// вставка элементов в начало
TEST(vector, insert_begin) {
    // Arrange
    const size_t expected = 6;
    MyVector v(expected-1);
    const int value = 7;
    const size_t pos = 0;

    for (size_t i = 0; i < (expected-1); i++) {
        v.push_back(i);
    }

    // Act
    v.insert(value, pos);

    // Assert
    ASSERT_EQ(v.size(), expected);
    ASSERT_EQ(v.get(pos), value);
}

// вставка элементов в середину
TEST(vector, insert_middle) {
    // Arrange
    const size_t expected = 6;
    MyVector v(expected-1);
    const int value = 7;
    const size_t pos = 3;

    for (size_t i = 0; i < (expected-1); i++) {
        v.push_back(i);
    }

    // Act
    v.insert(value, pos);

    // Assert
    ASSERT_EQ(v.size(), expected);
    ASSERT_EQ(v.get(pos), value);
}

// вставка элементов в конец
TEST(vector, insert_end) {
    // Arrange
    const size_t expected = 6;
    MyVector v(expected-1);
    const int value = 7;
    const size_t pos = 5;

    for (size_t i = 0; i < (expected-1); i++) {
        v.push_back(i);
    }

    // Act
    v.insert(value, pos);

    // Assert
    ASSERT_EQ(v.size(), expected);
    ASSERT_EQ(v.get(pos), value);
}

// удаление элементов из конца
TEST(vector, delete_end) {
    // Arrange
    const size_t expected = 5;
    MyVector v(expected+1);

    for (size_t i = 0; i < (expected+1); i++) {
        v.push_back(i);
    }

    // Act
    v.erase(expected);

    // Assert
    ASSERT_EQ(v.size(), expected);
}

// удаление элементов из начала
TEST(vector, delete_begin) {
    // Arrange
    const size_t expected = 5;
    MyVector v(expected+1);

    for (size_t i = 0; i < (expected+1); i++) {
        v.push_back(i);
    }

    // Act
    v.erase(0);

    // Assert
    ASSERT_EQ(v.size(), expected);
}

// удаление элементов из середины
TEST(vector, delete_middle) {
    // Arrange
    const size_t expected = 5;
    MyVector v(expected+1);
    const size_t pos = 3;

    for (size_t i = 0; i < (expected+1); i++) {
        v.push_back(i);
    }

    // Act
    v.erase(pos);

    // Assert
    ASSERT_EQ(v.size(), expected);
}

// копирование контейнера
TEST(vector, copy_constructor) {
    // Arrange
    const size_t sz = 5;
    MyVector v1(sz);

    for (size_t i = 0; i < sz; i++) {
        v1.push_back(i);
    }

    // Act
    MyVector v2 = v1;

    // Assert
    ASSERT_EQ(v1.size(), v2.size());
    for (size_t i = 0; i < sz; i++) {
        ASSERT_EQ(v1.get(i), v2.get(i));
    }
}

// удаление пустого контейнера
TEST(vector, delete_constructor_empty) {
    EXPECT_NO_THROW({
        // Arrange
        MyVector v;

        // Act

        // Assert
        // v.~MyVector();
    });
}

// удаление контейнера с одним элементом
TEST(vector, delete_constructor_one) {
    EXPECT_NO_THROW({
        // Arrange
        MyVector v;

        // Act
        v.push_back(7);

        // Assert
        // v.~MyVector();
    });
}

// удаление контейнера с несколькими элементами
TEST(vector, delete_constructor_many) {
    EXPECT_NO_THROW({
        // Arrange
        MyVector v;

        // Act
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);

        // Assert
        // v.~MyVector();
    });
}
