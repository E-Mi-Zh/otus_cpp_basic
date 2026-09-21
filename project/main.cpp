#include <iostream>
#include <cstdlib>

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
    return EXIT_SUCCESS;
}
