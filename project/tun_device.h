#pragma once

#include <cstddef>              // size_t
#include <cstdint>              // uint8_t
#include <string>

// Виртуальный интерфейс: открыть TUN, назначить адрес, читать и писать пакеты
class tun_device {
public:
    // Ставит fd в -1.
    // т.к. ноль нельзя закрывать (стандартный ввод), то устанавливаем fd = -1
    tun_device();
    ~tun_device();
    // Открыть TUN с заданным именем и адресом/маской
    bool open(const std::string& name, const std::string& cidr);
    // Прочитать пакет из TUN
    int read_packet(uint8_t* buf, size_t max);
    // Отправить пакет ядру через TUN
    bool write_packet(const uint8_t* buf, size_t n);
    // Закрывает /dev/net/tun, если он открыт
    void close();
private:
    // проверяем имя интерфейса или адрес на лишние символы
    bool plain_token(const std::string& s);
    // обёртка над system, для команды ip, чтобы не возиться с нетлинком
    bool run_cmd(const std::string& cmd); // печать и system
    // дескриптор /dev/net/tun, -1 если закрыт
    int fd;
    // имя TUN интерфейса
    std::string ifname;
};
