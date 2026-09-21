#include <iostream>
#include <cstdlib>
#include "tunnel_frame.h"

// Параметры вызова
struct cmd_args_s {
};

cmd_args_s cmd_args;

// Вывод справки
void print_usage()
{
    std::cout << "WayStation - client-server IPv4 tunnel" << std::endl;
    std::cout << "Usage:" << std::endl;
}

int main(int argc, char** argv)
{
    (void) argv; // заглушка, чтобы не было ворнинга про unused
    print_usage();
    if (argc > 1) {
        std::cout << "Got " << (argc - 1) << " arguments, ignoring them for now." << std::endl;
    }

    // Проверяем кадр
    const uint8_t hi[] = { 'h', 'i' };
    std::vector<uint8_t> frame;
    pack_frame(hi, 2, frame);
    dump_hex(frame.data(), frame.size());
    frame_header_s hdr;
    parse_header(frame.data(), frame.size(), hdr);
    std::cout << hdr.magic0 << " " << hdr.magic1 << " " << hdr.length << std::endl;

    return EXIT_SUCCESS;
}
