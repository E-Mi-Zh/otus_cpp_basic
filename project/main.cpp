#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <cstdlib>              // srand
#include <ctime>                // time
#include <thread>
#include <atomic>               // std::atomic
#include <cxxopts.hpp>          // command line args parsing

#include "tunnel_frame.h"           // pack_frame
#include "codec.h"                  // i_codec, identity, padding, irc
#include "tun_device.h"
#include "tcp_link.h"               // сокет после TUN

// структура с аргументами запуска
struct run_args {
    std::string mode;                   // client или server
    std::string host;                   // IP-адрес транспорта
    int port;
    std::string tun;                    // имя туннельного интерфейса
    std::string addr;                   // адрес на туннельном интерфейсе - CIDR, например 10.0.0.1/24
    std::string plugin;                 // плагин - identity, padding, irc
};

run_args args;
cxxopts::Options options("WayStation", "client-server IPv4 tunnel");
cxxopts::ParseResult result;

// Печатает хелп
void print_usage()
{
    std::cout << options.help() << std::endl;
    std::cout << "Same --plugin on both sides. There is no handshake." << std::endl << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  waystation --mode server --port 9000 --tun ws0 --addr 10.0.0.1/24 --plugin padding" << std::endl;
    std::cout << "  waystation --mode client --host 10.200.0.1 --port 9000 --tun ws1 --addr 10.0.0.2/24 --plugin padding" << std::endl;
}

// парсим аргументы
int process_args(int argc, char** argv)
{
    options.add_options()
        ("m,mode", "Working mode: client or server", cxxopts::value<std::string>())
        ("s,host", "IP - client only, transport address of the server", cxxopts::value<std::string>())
        ("p,port", "port number to connect", cxxopts::value<int>())
        ("t,tun", "tunnel interface name", cxxopts::value<std::string>())
        ("a,addr", "CIDR", cxxopts::value<std::string>())
        ("c,plugin", "Plugin: identity|padding|irc", cxxopts::value<std::string>())
        ("h,help", "Print usage");

    result = options.parse(argc, argv);

    if (argc < 2) {
        print_usage();
        return 0;
    }

    if (result.count("help")) {
        print_usage();
        exit(EXIT_SUCCESS);
    }

    if (result.count("mode") == 0) {
        std::cout << "Wrong usage: --mode is required" << std::endl;
        return -1;
    }
    args.mode = result["mode"].as<std::string>();

    if ((args.mode != "client") && (args.mode != "server")) {
        std::cout << "Wrong usage: --mode must be client or server" << std::endl;
        return -1;
    }

    // клиент без адреса сервера
    if ((args.mode == "client") && (args.host.size() == 0)) {
        if (result.count("host") == 0) {
            std::cout << "Wrong usage: --mode client requires --host" << std::endl;
            return -1;
        }
        args.host = result["host"].as<std::string>();
    }

    if (result.count("port") == 0) {
        std::cout << "Wrong usage: --port is required" << std::endl;
        return -1;
    }
    args.port = result["port"].as<int>();

    if (result.count("tun") == 0) {
        std::cout << "Wrong usage: --tun is required" << std::endl;
        return -1;
    }
    args.tun = result["tun"].as<std::string>();

    if (result.count("addr") == 0) {
        std::cout << "Wrong usage: --addr is required" << std::endl;
        return -1;
    }
    args.addr = result["addr"].as<std::string>();

    if (result.count("plugin") == 0) {
        std::cout << "Wrong usage: --plugin is required" << std::endl;
        return -1;
    }
    args.plugin = result["plugin"].as<std::string>();

    if ((args.plugin != "identity") && (args.plugin != "padding") && (args.plugin != "irc")) {
        std::cout << "Wrong usage: --plugin must be identity, padding or irc" << std::endl;
        return -1;
    }

    return 0;
}

// Счётчики
// Сколько IP-пакетов ушло в TCP
// Пишет только поток A, второй поток его не трогает
std::atomic<unsigned long> tx_packets{0};
// Байты payload
std::atomic<unsigned long> tx_bytes{0};
std::atomic<bool> running{true};
// Сколько IP-пакетов записали в TUN, пишет только поток B.
std::atomic<unsigned long> rx_packets{0};
// Байты payload, которые ушли в TUN
std::atomic<unsigned long> rx_bytes{0};

// создаём объекты для отправки и приёма
bool make_codecs(std::unique_ptr<i_codec>& enc, std::unique_ptr<i_codec>& dec)
{
    enc = 0;
    dec = 0;
    if (args.plugin == "identity") {
        enc = std::unique_ptr<i_codec> (new identity_codec{});
        dec = std::unique_ptr<i_codec> (new identity_codec{});
        return true;
    }

    if (args.plugin == "padding") {
        enc = std::unique_ptr<i_codec> (new padding_codec{});
        dec = std::unique_ptr<i_codec> (new padding_codec{});
        return true;
    }

    if (args.plugin == "irc") {
        enc = std::unique_ptr<i_codec> (new irc_codec{});
        dec = std::unique_ptr<i_codec> (new irc_codec{});
        return true;
    }

    std::cout << "Wrong usage: --plugin must be identity, padding or irc" << std::endl;

    return false;
}

// читает пакеты из TUN, упаковывает, используя кодек и отправляет в сеть (транспорт, сокет)
void pump_out(tun_device* tun, tcp_link* link, std::unique_ptr<i_codec>& enc)
{
    // IP-пакет, больше MTU с запасом
    uint8_t buf[2048];
    while (running) {
        // чтение с блокировкой
        int n = tun->read_packet(buf, 2048);

        if (n <= 0) {
            perror("Error read from TUN");
            running = false;
            break;
        }

        // Упаковываем пакет в кадр
        std::vector<uint8_t> frame;
        if (!pack_frame(buf, (size_t)n, frame)) {
            continue;
        }

        // Кодируем кадр
        std::vector<uint8_t> wire;
        if (!enc->encode(frame.data(), frame.size(), wire)) {
            std::cout << "encode failed" << std::endl;
            continue;
        }

        // Отправляем в сокет
        if (!link->send_all(wire.data(), wire.size())) {
            running = false;
            break;
        }

        // Увеличиваем счётчики
        tx_packets = tx_packets + 1;
        tx_bytes = tx_bytes + (unsigned long)n;         // длина payload
        std::cout << "sent " << n << " bytes, tx_packets=" << tx_packets << std::endl;
    }
}

// Поток B: берём из сокета, распаковываем, в TUN отправляем только payload
void pump_in(tun_device* tun, tcp_link* link, std::unique_ptr<i_codec>& dec)
{
    uint8_t buf[2048];
    while (running) {
        int n = link->recv_data(buf, 2048);

        if (n == 0) {
            std::cout << "peer closed" << std::endl;
            running = false;
            break;
        }

        if (n < 0) {
            perror("Error receiving data!");
            running = false;
            break;
        }

        // Распаковка, эх, распаковочка!
        if (!dec->decode(buf, (size_t)n)) {
            std::cout << "decode failed, drop" << std::endl;
            running = false;
            break;
        }

        std::vector<uint8_t> frame;
        // вытаскиваем кадры, пока они есть
        while (dec->pop_frame(frame)) { 
            if (frame.size() <= HEADER_SIZE) {
                std::cout << "short frame" << std::endl;
                continue;
            }

            frame_header_s hdr;
            if (!parse_header(frame.data(), frame.size(), hdr)) {
                std::cout << "bad header in frame" << std::endl;
                continue;
            }

            if ((size_t)hdr.length + HEADER_SIZE != frame.size()) {
                std::cout << "length mismatch" << std::endl;
            }

            const uint8_t* payload = frame.data() + HEADER_SIZE;
            size_t payload_len = frame.size() - HEADER_SIZE;

            // версия IP, должна быть 0x45
            int first = payload[0];
            std::cout << "to tun first byte 0x" << std::hex << first << std::dec << std::endl;
            if (payload[0] != 0x45) {
                std::cout << "not ipv4, skip write" << std::endl;
                continue;
            }

            // отправляем пакет в TUN
            if (!tun->write_packet(payload, payload_len)) {
                running = false;
                break;
            }

            // Увеличиваем счётчики
            rx_packets = rx_packets + 1;
            rx_bytes = rx_bytes + (unsigned long)payload_len;
            std::cout << "wrote " << payload_len << " bytes, rx_packets=" << rx_packets << std::endl;
        }
    }
}

// Поток С - раз в пять секунд печатает счётчики
void report_stats()
{
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "tx " << tx_packets << " packets / " << tx_bytes << " bytes, rx " << rx_packets << " packets / " << rx_bytes << " bytes" << std::endl; // полезная нагрузка, не провод
    }
}

// Печатаем аргументы
void print_args()
{
    std::cout << "mode=" << args.mode << std::endl;
    std::cout << "host=" << args.host << std::endl;
    std::cout << "port=" << args.port << std::endl;
    std::cout << "tun=" << args.tun << std::endl;
    std::cout << "addr=" << args.addr << std::endl;
    std::cout << "plugin=" << args.plugin << std::endl;
}

int main(int argc, char** argv)
{
    if (process_args(argc, argv) < 0) {
        return EXIT_FAILURE;
    }

    if (argc < 2) {
        return EXIT_SUCCESS;            // Хелп уже напечатали в process_args
    }

    print_args();

    std::unique_ptr<i_codec> enc;
    std::unique_ptr<i_codec> dec;

    if (!make_codecs(enc, dec)) {
        // delete enc;
        // delete dec;
        return EXIT_FAILURE;
    }

    std::cout << "enc=" << enc->name() << " dec=" << dec->name() << std::endl;

    if (args.plugin == "padding") {
        std::srand((unsigned)std::time(0));
    }

    tun_device tun;
    if (!tun.open(args.tun, args.addr)) {
        return EXIT_FAILURE;
    }

    tcp_link link;
    bool ok = false;
    if (args.mode == "server") {
        ok = link.listen_and_accept(args.port);
    } else {
        // здесь подключаемся к адресу транспорта, не TUN
        ok = link.connect_to_server(args.host.c_str(), args.port);
    }
    if (!ok) {
        return EXIT_FAILURE;
    }

    // поток энкодер
    std::thread out_thread(pump_out, &tun, &link, std::ref(enc));
    // поток декодер
    std::thread in_thread(pump_in, &tun, &link, std::ref(dec));
    // отдельный поток печатает счётчики
    std::thread stats_thread(report_stats);
    out_thread.join();
    in_thread.join();
    stats_thread.join();

    return EXIT_SUCCESS;
}
