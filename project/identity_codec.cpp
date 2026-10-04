#include <iostream>

#include "codec.h"

const char* identity_codec::name() const
{
    return "identity";
}

// Копирует кадр в выход как есть.
bool identity_codec::encode(const uint8_t* frame, size_t frame_len, std::vector<uint8_t>& out)
{
    if ((frame == nullptr) && (frame_len != 0)) {
        std::cout << "identity encode: null frame" << std::endl;
        return false;
    }
    out.assign(frame, frame + frame_len);

    return true;
}

bool identity_codec::decode(const uint8_t* data, size_t data_len)
{
    return this->parser.consume(data, data_len);
}

bool identity_codec::pop_frame(std::vector<uint8_t>& frame)
{
    return this->parser.pop(frame);
}
