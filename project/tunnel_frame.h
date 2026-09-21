#pragma once

#include <cstdint>          // uint8_t, uint32_t
#include <cstddef>          // size_t
#include <vector>

// размер заголовка кадра, без паддинга
#define HEADER_SIZE 6

#define MAX_FRAME_SIZE 65536
// magic чтобы видеть в отладке и дампах
const uint8_t FRAME_MAGIC_0 = 0xAB;
const uint8_t FRAME_MAGIC_1 = 0xCD;
const uint8_t FRAME_VERSION = 0x01;
const uint8_t FRAME_TYPE_DATA = 0x01;

struct frame_header_s {
    uint8_t magic0;
    uint8_t magic1;
    uint32_t length;                // длина payload
};

// кладём payload в кадр
bool pack_frame(const uint8_t* payload, size_t len, std::vector<uint8_t>& out);
// читает заголовок и парсим
bool parse_header(const uint8_t* bytes, size_t n, frame_header_s& hdr);
// печатаем буфер в hex
void dump_hex(const uint8_t* p, size_t n);

class frame_parser {
public:
    // добавить байты из буфера (сети)
    // false - ошибка
    bool consume(const uint8_t* data, size_t len);
    // выдаёт готовый кадр
    // false - мало данных
    bool pop(std::vector<uint8_t>& frame);
private:
    // удаляет из буфера готовые кадры
    bool pull_ready();
    std::vector<uint8_t> tail;
    std::vector<std::vector<uint8_t>> ready;
};
