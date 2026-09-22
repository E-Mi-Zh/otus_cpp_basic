#include <gtest/gtest.h>
#include <cstdint>
#include <cstdlib>          // srand
#include <vector>

#include "../codec.h"

// encode/decode одного кадра
TEST(padding, round_trip) {
    std::srand(1);
    // Arrange
    const uint8_t payload[] = { 'h', 'i' };
    std::vector<uint8_t> frame;
    ASSERT_TRUE(pack_frame(payload, 2, frame));
    padding_codec enc;
    std::vector<uint8_t> wire;

    // Act
    ASSERT_TRUE(enc.encode(frame.data(), frame.size(), wire));
    ASSERT_GE(wire.size(), frame.size() + 2);
    padding_codec dec;
    ASSERT_TRUE(dec.decode(wire.data(), wire.size()));

    // Assert
    std::vector<uint8_t> out;
    ASSERT_TRUE(dec.pop_frame(out));
    ASSERT_EQ(out, frame);
    ASSERT_FALSE(dec.pop_frame(out));
}

// два кадра подряд.
// декодер обязан вернуться в WAIT_N
TEST(padding, two_frames) {
    std::srand(2);
    // Arrange
    const uint8_t a[] = { 'a' };
    const uint8_t b[] = { 'b', 'b' };
    std::vector<uint8_t> fa;
    std::vector<uint8_t> fb;
    ASSERT_TRUE(pack_frame(a, 1, fa));
    ASSERT_TRUE(pack_frame(b, 2, fb));
    padding_codec enc;
    std::vector<uint8_t> w1;
    std::vector<uint8_t> w2;
    ASSERT_TRUE(enc.encode(fa.data(), fa.size(), w1));
    ASSERT_TRUE(enc.encode(fb.data(), fb.size(), w2));
    std::vector<uint8_t> wire = w1;
    wire.insert(wire.end(), w2.begin(), w2.end());

    // Act
    padding_codec dec;
    ASSERT_TRUE(dec.decode(wire.data(), wire.size()));

    // Assert
    std::vector<uint8_t> out;
    ASSERT_TRUE(dec.pop_frame(out));
    ASSERT_EQ(out, fa);
    ASSERT_TRUE(dec.pop_frame(out));
    ASSERT_EQ(out, fb);
    ASSERT_FALSE(dec.pop_frame(out));
}

// Подаём частями по три байта
TEST(padding, by_three_bytes) {
    std::srand(3);

    // Arrange
    const uint8_t payload[] = { 'p', 'i', 'n', 'g' };
    std::vector<uint8_t> frame;
    ASSERT_TRUE(pack_frame(payload, 4, frame));
    padding_codec enc;
    std::vector<uint8_t> wire;
    ASSERT_TRUE(enc.encode(frame.data(), frame.size(), wire));

    // Act
    padding_codec dec;
    // Режем по 3 байта
    for (size_t i = 0; i < wire.size(); i += 3) {
        size_t n = 3;
        if (i + n > wire.size()) {
            n = wire.size() - i;
        }
        // состояние не сбрасывается
        ASSERT_TRUE(dec.decode(wire.data() + i, n));
    }

    // Assert
    // кадр собран целиком
    std::vector<uint8_t> out;
    ASSERT_TRUE(dec.pop_frame(out));
    ASSERT_EQ(out, frame);
}
