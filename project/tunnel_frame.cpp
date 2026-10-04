#include <iostream>
#include <iomanip>          // setw, setfill
#include <cstring>          // memcpy
#include <arpa/inet.h>      // htonl

#include "tunnel_frame.h"

bool pack_frame(const uint8_t* payload, size_t len, std::vector<uint8_t>& out)
{
    if (len > MAX_FRAME_SIZE) {
        std::cout << "pack_frame: payload too big" << std::endl;
        return false;
    }
    out.push_back(FRAME_MAGIC_0);
    out.push_back(FRAME_MAGIC_1);

    uint32_t l = htonl(len);                // host → network byte order
    uint8_t buf[4];
    std::memcpy(&buf[0], &l, 4);
    out.insert(out.end(), buf, buf + 4);

    out.insert(out.end(), payload, payload + len);

    return true;
}

bool parse_header(const uint8_t* bytes, size_t n, frame_header_s& hdr)
{
    if (n < HEADER_SIZE) {
        return false;
    }
    hdr.magic0 = bytes[0];
    hdr.magic1 = bytes[1];

    uint32_t l;
    std::memcpy(&l, &bytes[2], 4);
    hdr.length = ntohl(l);

    return true;
}

void dump_hex(const uint8_t* p, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        std::printf("%02x ", p[i]);
        if ((i + 1) % 16 == 0)
            std::printf("\n");
    }
    if (n % 16 != 0)
        std::printf("\n");
    std::printf("\n");
}

bool frame_parser::consume(const uint8_t* data, size_t len)
{
    if (len == 0) {
        return true;
    }

    this->tail.insert(this->tail.end(), data, data + len);

    return this->pull_ready();          // пробуем вернуть кадр
}

bool frame_parser::pull_ready()
{
    while (this->tail.size() >= HEADER_SIZE) {
        frame_header_s hdr;

        if (!parse_header(this->tail.data(), this->tail.size(), hdr)) {
            return true;
        }

        // Если magic не совпал, значит кадры рассинхронизировались
        if ((hdr.magic0 != FRAME_MAGIC_0) || (hdr.magic1 != FRAME_MAGIC_1)) {
            std::cout << "bad magic, drop tail" << std::endl;
            this->tail.clear();
            return false;
        }

        if (hdr.length > MAX_FRAME_SIZE) {
            std::cout << "frame length too big, drop tail" << std::endl;
            this->tail.clear();
            return false;
        }

        size_t need = HEADER_SIZE + hdr.length;
        if (this->tail.size() < need) {
            // получен ещё не весь кадр, ждём ещё байты от consume
            return true;
        }

        std::vector<uint8_t> frame;
        frame.insert(frame.end(), this->tail.begin(), this->tail.begin() + need);
        this->ready.push_back(frame);
        this->tail.erase(this->tail.begin(), this->tail.begin() + need);
    }

    return true;
}

bool frame_parser::pop(std::vector<uint8_t>& frame)
{
    if (this->ready.empty()) {
        return false;
    }

    frame = this->ready[0];
    this->ready.erase(this->ready.begin());
    return true;
}