#include <gtest/gtest.h>
#include <cstdint>
#include <vector>

#include "../codec.h"

TEST(identity, round_trip)
{
    // Arrange
    // payload
    const uint8_t payload[] = { 'h', 'i' }; 
    std::vector<uint8_t> frame;
    // Собираю кадр
    ASSERT_TRUE(pack_frame(payload, 2, frame));

    // Act
    identity_codec codec;
    std::vector<uint8_t> wire;
    ASSERT_TRUE(codec.encode(frame.data(), frame.size(), wire));
    // identity ничего не добавил
    ASSERT_EQ(wire, frame);
    ASSERT_TRUE(codec.decode(wire.data(), wire.size()));

    // Assert:
    std::vector<uint8_t> out; // кадр с приёма
    // один кадр (не два), байт в байт
    ASSERT_TRUE(codec.pop_frame(out));
    ASSERT_EQ(out, frame);
    ASSERT_FALSE(codec.pop_frame(out));
}
