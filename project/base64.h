#pragma once

#include <cstddef>          // size_t
#include <cstdint>          // uint8_t, сдвиг без знака
#include <string>
#include <vector>

// Т.к. в payload могут встретятся \r и \n, то мы кодируем
// байты в Base64 перед тем как отправлять строкой в IRC
bool base64_encode(const uint8_t* in, size_t len, std::string& out);
// Декодируем IRC строку обратно в байты
bool base64_decode(const std::string& in, std::vector<uint8_t>& out);
