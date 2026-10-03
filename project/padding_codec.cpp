#include <iostream>
#include <cstdlib>          // rand, srand
#include <arpa/inet.h>      // htonl
#include <cstring>          // memcpy

#include "codec.h"

padding_codec::padding_codec()
{
    // каждый раз при создании объекта начинаем с ожидания длины паддинга
    this->state = PAD_WAIT_N; 
    this->pad_left = 0;
    this->frame_total = 0;
}

const char* padding_codec::name() const
{
    return "padding";
}

// пишет длину N, N случайных байт и затем кадр
bool padding_codec::encode(const uint8_t* frame, size_t frame_len, std::vector<uint8_t>& out)
{
    if ((frame == nullptr) && (frame_len != 0)) {
        std::cout << "padding encode: null frame" << std::endl;
        return false;
    }

    out.clear();
    // n - случайное число от 0 до 64
    int n = std::rand() % (PAD_MAX - PAD_MIN + 1) + PAD_MIN;
    uint16_t nn = htons(n);
    uint8_t buf[2];
    std::memcpy(buf, &nn, 2);
    out.push_back(buf[0]);                   // старший байт длины (big-endian)
    out.push_back(buf[1]);                   // младший байт длины

    for (int i = 0; i < n; i++) { // ровно N случайных байт, не больше
        out.push_back((uint8_t)(std::rand() & 0xFF));
    }

    for (size_t i = 0; i < frame_len; i++) {
        out.push_back(frame[i]);
    }
    return true;
}

// Снимаем лишнее (паддинг) частми передаём целые кадры в парсер
// recv может возвращать буферы байтов с произвольными границами
bool padding_codec::decode(const uint8_t* data, size_t data_len)
{
    if (data_len > 0) {
        for (size_t i = 0; i < data_len; i++) {
            stash.push_back(data[i]);
        }
    }
    // кручу автомат, пока хватает байт на текущий шаг
    while (true) {
        if (this->state == PAD_WAIT_N) {
            // ждём появления размера паддинга (два байта с длиной)
            if (this->stash.size() < 2) {
                // в оставшемся буфере меньше двух байт, значит длины там точно нет, ждём следующего вызова
                return true;
            }

            uint16_t nn;
            std::memcpy(&nn, &this->stash[0], 2);
            uint16_t n = ntohs(nn);

            if (n > PAD_MAX) {
                // длина слишком большая, значит это мусор
                std::cout << "padding: bad length " << n << std::endl;
                // сбрасываем буфер
                this->stash.clear();
                return false;
            }

            // Удаляем два байта длины из буфера
            this->stash.erase(this->stash.begin(), this->stash.begin() + 2); // длина съедена
            // сохраняем сколько байт в паддинге
            this->pad_left = n;
            // переключаем автомат в следующее состояние: ожидание паддинга (мусора)
            this->state = PAD_WAIT_PAD;
        } else if (this->state == PAD_WAIT_PAD) {
            if (this->stash.size() < this->pad_left) {
                // приняли ещё не весь паддинг
                return true;
            }

            // выбрасываем паддинг из буфера
            this->stash.erase(this->stash.begin(), this->stash.begin() + this->pad_left);
            // очищаем буфер для нового кадра
            this->frame_acc.clear();
            this->frame_total = 0;
            // переключаем автомат в новое состояние - ожидание кадра
            this->state = PAD_WAIT_FRAME;
        } else {
            // PAD_WAIT_FRAME: забираем один кадр и возвращаемся к длине
            if (this->frame_total == 0) {
                // кадров ещё не набрано
                // сначала аккумулируем заголовок
                if (this->frame_acc.size() < HEADER_SIZE) {
                    // сколько не хватает до длины заголовка
                    size_t need = HEADER_SIZE - this->frame_acc.size();
                    if (this->stash.size() < need) {
                        // в буфере меньше байт, чем надо
                        // вставляем то что есть (часть заголовка) и ждём ещё
                        this->frame_acc.insert(this->frame_acc.end(), this->stash.begin(), this->stash.end());
                        this->stash.clear();
                        return true;
                    }
                    // сохраняем недостающую часть заголовка и удаляем её из буфера
                    this->frame_acc.insert(this->frame_acc.end(), this->stash.begin(), this->stash.begin() + need);
                    this->stash.erase(this->stash.begin(), this->stash.begin() + need);
                }
                // здесь мы уже имеем во frame_acc полный заголовок кадра
                // теперь можем анализировать его и получить длину payload
                frame_header_s hdr;
                parse_header(this->frame_acc.data(), this->frame_acc.size(), hdr);
                if ((hdr.magic0 != FRAME_MAGIC_0) || (hdr.magic1 != FRAME_MAGIC_1) || (hdr.length > MAX_FRAME_SIZE)) {
                    // битый кадр
                    // очищаем буферы и сбрасываем автомат к ожиданию длины паддинга
                    std::cout << "padding: bad frame header" << std::endl;
                    this->stash.clear();
                    this->frame_acc.clear();
                    this->state = PAD_WAIT_N;
                    return false;
                }
                this->frame_total = HEADER_SIZE + (size_t)hdr.length;
            }
            // теперь набираем тело кадра
            if (this->frame_acc.size() < this->frame_total) {
                size_t need = this->frame_total - this->frame_acc.size();
                if (this->stash.size() < need) {
                    this->frame_acc.insert(this->frame_acc.end(), this->stash.begin(), this->stash.end());
                    this->stash.clear();
                    return true;
                }
                this->frame_acc.insert(this->frame_acc.end(), this->stash.begin(), this->stash.begin() + need);
                this->stash.erase(this->stash.begin(), this->stash.begin() + need);
            }
            // если парсер сфейлится, значит кадр всё же битый
            if (!this->parser.consume(this->frame_acc.data(), this->frame_acc.size())) {
                this->frame_acc.clear();
                this->frame_total = 0;
                this->state = PAD_WAIT_N;
                return false;
            }
            this->frame_acc.clear();
            this->frame_total = 0;
            this->state = PAD_WAIT_N;
        }
    }
}

bool padding_codec::pop_frame(std::vector<uint8_t>& frame)
{
    return this->parser.pop(frame);
}
