#include <iostream>

#include "base64.h"

// Алфавит из 64 символов - 2 в 6-й
const char B64_ALPH[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Преобразуем символ алфавита в число от 0 до 63
// -1 - лишний символ
int b64_val(char c)
{
    if ((c >= 'A') && (c <= 'Z')) {
        return (c - 'A');                 // 0..25
    }

    if ((c >= 'a') && (c <= 'z')) {
        return (c - 'a' + 26);            // 26..51
    }

    if ((c >= '0') && (c <= '9')) {
        return (c - '0' + 52);            // 52..61
    }

    if (c == '+') {
        return 62;
    }

    if (c == '/') {
        return 63;
    }

    return -1;
}

// кодируем три байта в четыре символа
// если есть хвост - дописываем '='
// На выходе нет \r и \n, строку IRC можно резать по ним
bool base64_encode(const uint8_t* in, size_t len, std::string& out)
{
    if ((in == 0) && (len != 0)) {
        std::cout << "base64 encode: null" << std::endl;
        return false;
    }

    out.clear();
    size_t i = 0;
    while ((i + 3) <= len) {
        // uint8_t в uint32 до сдвига: байт > 127 на char уехал бы в знак.
        uint32_t n = ((uint32_t)in[i] << 16) | ((uint32_t)in[i + 1] << 8) | (uint32_t)in[i + 2]; // 24 бита
        out.push_back(B64_ALPH[(n >> 18) & 63]); // старшие 6
        out.push_back(B64_ALPH[(n >> 12) & 63]); // следующие
        out.push_back(B64_ALPH[(n >> 6) & 63]); // следующие
        out.push_back(B64_ALPH[n & 63]); // младшие 6
        i = i + 3;
    }
    // в хвост пишем '='
    if (len - i == 1) {
        uint32_t n = ((uint32_t)in[i] << 16);
        out.push_back(B64_ALPH[(n >> 18) & 63]);
        out.push_back(B64_ALPH[(n >> 12) & 63]);
        out.push_back('=');
        out.push_back('=');
    } else if (len - i == 2) {
        uint32_t n = ((uint32_t)in[i] << 16) | ((uint32_t)in[i + 1] << 8);
        out.push_back(B64_ALPH[(n >> 18) & 63]);
        out.push_back(B64_ALPH[(n >> 12) & 63]);
        out.push_back(B64_ALPH[(n >> 6) & 63]);
        out.push_back('=');
    }

    return true;
}

// Декодируем четыре символа в байты от одного до трёх
bool base64_decode(const std::string& in, std::vector<uint8_t>& out)
{
    out.clear();

    if (in.empty()) {
        return true;
    }

    if (in.size() % 4 != 0) {
        std::cout << "base64 decode: length not multiple of 4" << std::endl;
        return false;
    }

    for (size_t i = 0; i < in.size(); i = i + 4) {
        int v[4];
        // счётчик '='
        int pad = 0;
        for (int k = 0; k < 4; k++) {
            char c = in[i + (size_t)k];
            if (c == '=') {
                if (k < 2) {
                    // '=' не должно быть в начале
                    std::cout << "base64 decode: pad too early" << std::endl;
                    return false;
                }
                v[k] = 0;
                pad = pad + 1;
            } else {
                // символ алфавита
                if (pad > 0) {
                    // после '=' символов быть не должно
                    std::cout << "base64 decode: data after pad" << std::endl;
                    return false;
                }

                int d = b64_val(c);

                if (d < 0) {
                    std::cout << "base64 decode: bad char" << std::endl;
                    return false;
                }

                // шесть бит
                v[k] = d;
            }
        }
        uint32_t n = ((uint32_t)v[0] << 18) | ((uint32_t)v[1] << 12) | ((uint32_t)v[2] << 6) | (uint32_t)v[3]; // 24 бита

        out.push_back((uint8_t)((n >> 16) & 0xFF));
        // XXX= даёт два байта, XX== — один

        if (pad < 2) {
            out.push_back((uint8_t)((n >> 8) & 0xFF));
        }

        if (pad < 1) {
            out.push_back((uint8_t)(n & 0xFF));
        }
    }

    return true;
}
