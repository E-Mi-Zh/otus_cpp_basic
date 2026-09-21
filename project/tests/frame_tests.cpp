#include <gtest/gtest.h>
#include <cstdint>
#include <vector>

#include "../tunnel_frame.h"

// Проверяем, что pack и pop не портят payload
TEST(frame, round_trip) { // круг: упаковал, разобрал, payload тот же
    const uint8_t payload[] = { 'h', 'i' }; // Arrange: короткая полезная нагрузка
    std::vector<uint8_t> packed; // сюда ляжет целый кадр
    ASSERT_TRUE(pack_frame(payload, 2, packed)); // Act: собираю кадр своими же функциями
    frame_parser parser; // парсер пустой
    ASSERT_TRUE(parser.consume(packed.data(), packed.size())); // скармливаю кадр целиком
    std::vector<uint8_t> out; // сюда pop положит кадр
    ASSERT_TRUE(parser.pop(out)); // один кадр должен быть готов
    ASSERT_EQ(out.size(), packed.size()); // Assert: размер не поехал
    ASSERT_EQ(out, packed); // байты кадра совпали, значит и payload внутри
    ASSERT_FALSE(parser.pop(out)); // второго кадра нет
} // конец round_trip

// Скармливает кадр двумя половинами.
// Зачем: заголовок часто приходит отдельно от тела.
TEST(frame, split_header) {
    // Arrange
    const uint8_t payload[] = { 'h', 'i' };
    std::vector<uint8_t> packed;
    ASSERT_TRUE(pack_frame(payload, 2, packed));

    // полкадра
    size_t half = packed.size() / 2;

    frame_parser parser;

    // Act
    // первая половина
    ASSERT_TRUE(parser.consume(packed.data(), half)); 
    std::vector<uint8_t> out;
    ASSERT_FALSE(parser.pop(out));              // кадра ещё нет
    // вторая половина
    ASSERT_TRUE(parser.consume(packed.data() + half, packed.size() - half));

    // Assert: теперь кадр собрался
    ASSERT_TRUE(parser.pop(out));
    // Кадр распакован правильно
    ASSERT_EQ(out, packed);
}

// Проверям, что мы можем корректно обработать приём нескольких кадров сразу
TEST(frame, two_in_one) {
    // Arrange
    // первый payload
    const uint8_t a[] = { 'a' };
    // второй
    const uint8_t b[] = { 'b', 'b' };
    std::vector<uint8_t> fa;
    std::vector<uint8_t> fb;

    ASSERT_TRUE(pack_frame(a, 1, fa));
    ASSERT_TRUE(pack_frame(b, 2, fb));

    std::vector<uint8_t> both = fa;
    both.insert(both.end(), fb.begin(), fb.end());
    frame_parser parser;
    // Act: одним куском
    ASSERT_TRUE(parser.consume(both.data(), both.size()));
    std::vector<uint8_t> out;

    // Assert
    // первый
    ASSERT_TRUE(parser.pop(out));
    ASSERT_EQ(out, fa);
    // второй
    ASSERT_TRUE(parser.pop(out));
    ASSERT_EQ(out, fb);
    ASSERT_FALSE(parser.pop(out));
}

// Портим заголовок
TEST(frame, bad_magic) {
    const uint8_t payload[] = { 'h', 'i' };
    std::vector<uint8_t> packed;

    ASSERT_TRUE(pack_frame(payload, 2, packed));
    packed[0] = 0x00;
    frame_parser parser;
    // Act
    // битый заголовок
    ASSERT_FALSE(parser.consume(packed.data(), packed.size()));
    std::vector<uint8_t> out;

    // Assert: кадра нет
    ASSERT_FALSE(parser.pop(out));
}
