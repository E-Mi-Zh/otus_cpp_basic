#include <gtest/gtest.h>
#include <cstdint>
#include <string>
#include <vector>

#include "../base64.h"
#include "../codec.h"

// Проверяем дополнение '='
TEST(base64, known) {
    // Act
    std::string out;
    const uint8_t abc[] = { 'A', 'B', 'C' };
    ASSERT_TRUE(base64_encode(abc, 3, out)); 
    ASSERT_EQ(out, "QUJD"); // echo -n ABC | base64

    const uint8_t ab[] = { 'A', 'B' };
    ASSERT_TRUE(base64_encode(ab, 2, out));
    ASSERT_EQ(out, "QUI=");

    const uint8_t a[] = { 'A' };
    ASSERT_TRUE(base64_encode(a, 1, out));
    ASSERT_EQ(out, "QQ==");

    ASSERT_TRUE(base64_encode(0, 0, out));
    ASSERT_EQ(out, "");
}

// Проверяем на массивах от 0 до 100, включая байты больше 127
// decode(encode(x)) == x
TEST(base64, round_lengths) {
    for (size_t n = 0; n <= 100; n++) {
        std::vector<uint8_t> in(n);

        for (size_t i = 0; i < n; i++) {
            in[i] = (uint8_t)(i * 17 + 3);
        }

        std::string enc;
        ASSERT_TRUE(base64_encode(in.data(), in.size(), enc));
        std::vector<uint8_t> back;
        ASSERT_TRUE(base64_decode(enc, back));
        ASSERT_EQ(back, in);
    }
}

// Короткий кадр - одна строка, seq растёт
TEST(irc, short_round_trip) {
    const uint8_t payload[] = { 'h', 'i' };
    std::vector<uint8_t> frame;
    ASSERT_TRUE(pack_frame(payload, 2, frame));

    irc_codec enc;
    std::vector<uint8_t> wire;
    ASSERT_TRUE(enc.encode(frame.data(), frame.size(), wire));

    std::string text(wire.begin(), wire.end());
    ASSERT_NE(text.find("PRIVMSG #tunnel :FT:1:1/1:"), std::string::npos);
    ASSERT_EQ(text.size() >= 2 && text[text.size() - 2] == '\r' && text[text.size() - 1] == '\n', true);

    std::vector<uint8_t> wire2;
    ASSERT_TRUE(enc.encode(frame.data(), frame.size(), wire2));
    std::string text2(wire2.begin(), wire2.end());
    ASSERT_NE(text2.find("PRIVMSG #tunnel :FT:2:1/1:"), std::string::npos);

    irc_codec dec;
    ASSERT_TRUE(dec.decode(wire.data(), wire.size()));

    std::vector<uint8_t> out;
    ASSERT_TRUE(dec.pop_frame(out));
    ASSERT_EQ(out, frame);
    ASSERT_FALSE(dec.pop_frame(out));
}

// Длинный кадр режем на несколько строк и собираем обратно
TEST(irc, long_round_trip) {
    std::vector<uint8_t> payload(500);

    for (size_t i = 0; i < payload.size(); i++) {
        payload[i] = (uint8_t)(i & 0xFF);
    }

    std::vector<uint8_t> frame;
    ASSERT_TRUE(pack_frame(payload.data(), payload.size(), frame));

    irc_codec enc;
    std::vector<uint8_t> wire;
    ASSERT_TRUE(enc.encode(frame.data(), frame.size(), wire));

    std::string text(wire.begin(), wire.end());
    ASSERT_NE(text.find(":1/2:"), std::string::npos); // первый кусок из двух
    ASSERT_NE(text.find(":2/2:"), std::string::npos); // второй кусок, total

    irc_codec dec;
    for (size_t i = 0; i < wire.size(); i = i + 3) {
        size_t n = 3;
        if (i + n > wire.size()) {
            n = wire.size() - i;
        }
        ASSERT_TRUE(dec.decode(wire.data() + i, n));
    }

    std::vector<uint8_t> out;
    ASSERT_TRUE(dec.pop_frame(out));
    ASSERT_EQ(out, frame);
    ASSERT_FALSE(dec.pop_frame(out));
}

// Битая и посторонняя строки не ломают следующий нормальный кадр
TEST(irc, bad_and_foreign) {
    const uint8_t payload[] = { 'o', 'k' };
    std::vector<uint8_t> frame;
    ASSERT_TRUE(pack_frame(payload, 2, frame));

    irc_codec enc;
    std::vector<uint8_t> wire;
    ASSERT_TRUE(enc.encode(frame.data(), frame.size(), wire));

    std::string junk = "PRIVMSG #tunnel :FT:nope\r\nPING :server\r\n";
    std::vector<uint8_t> mix(junk.begin(), junk.end());
    mix.insert(mix.end(), wire.begin(), wire.end());

    irc_codec dec;
    ASSERT_TRUE(dec.decode(mix.data(), mix.size()));

    std::vector<uint8_t> out;
    ASSERT_TRUE(dec.pop_frame(out));
    ASSERT_EQ(out, frame);
    ASSERT_FALSE(dec.pop_frame(out));
}
