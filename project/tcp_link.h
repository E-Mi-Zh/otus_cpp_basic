#pragma once

#include <cstddef>      // size_t
#include <cstdint>      // uint8_t

// Класс-обёртка над сокетом
class tcp_link {
public:
    tcp_link();
    ~tcp_link();
    // Слушаем и принимаем одного клиента
    // bind, listen, accept, в fd остаётся клиент
    bool listen_and_accept(int port);
    // подключиться к серверу
    // socket + connect к host:port
    bool connect_to_server(const char* host, int port);
    // Отправить буфер
    bool send_all(const uint8_t* buf, size_t n);
    // Прочитать данные из потока
    // Результат >0 - получили данные; 0 - сокет закрыт; <0 - ошибка
    int recv_data(uint8_t* buf, size_t max);
    void close();
private:
    int fd;
    int listen_fd; // сокет, на котором слушаем, будет закрыт после accept
};
