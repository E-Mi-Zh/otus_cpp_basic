#include <iostream>
#include <cstring>          // memset
#include <sys/socket.h>     // socket, bind, send, recv
#include <netinet/in.h>     // sockaddr_in, htons
#include <arpa/inet.h>      // inet_pton
#include <unistd.h>         // close

#include "tcp_link.h"


// Конструктор. Обнулять нельзя, т.к. 0 - это stdin
tcp_link::tcp_link()
{
    this->fd = -1;
    this->listen_fd = -1;
}

// Деструктор
tcp_link::~tcp_link()
{
    this->close();
}

// Закрываем рабочий и слушающий сокеты
void tcp_link::close()
{
    if (this->fd >= 0) {
        ::close(this->fd);
        this->fd = -1;
    }
    if (this->listen_fd >= 0) {
        ::close(this->listen_fd);
        this->listen_fd = -1;
    }
}

// bind, listen и accept одного клиента.
// to-do: в будущем добавить несколько соединений
bool tcp_link::listen_and_accept(int port)
{
    this->close();
    this->listen_fd = socket(AF_INET, SOCK_STREAM, 0); // TCP-сокет для listen
    if (this->listen_fd < 0) {
        perror("Can't create socket!");
        return false;
    }

    int opt = 1;
    if (setsockopt(this->listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Can't set reuse option!");
        this->close();
        return false;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(this->listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Can't bind socket!");
        this->close();
        return false;
    }

    if (listen(this->listen_fd, 1) < 0) {
        perror("Error listening socket!");
        this->close();
        return false;
    }

    struct sockaddr_in client_addr;             // адрес клиента
    socklen_t client_len = sizeof(client_addr);
    this->fd = accept(this->listen_fd, (struct sockaddr*)&client_addr, &client_len);
    if (this->fd < 0) {
        perror("Can't accept connection to socket!");
        this->close();
        return false;
    }
    ::close(this->listen_fd);
    this->listen_fd = -1;
    std::cout << "Got accepted connection" << std::endl;
    return true;
}

// Создаёт сокет и подключается к host:port, используется клиентом
bool tcp_link::connect_to_server(const char* host, int port)
{
    this->close();
    this->fd = socket(AF_INET, SOCK_STREAM, 0);     // исходящий TCP сокет
    if (this->fd < 0) {
        perror("Cant't create socket!");
        return false;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    // Конвертируем строку с IP в двоичный адрес
    if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
        std::cout << "bad host: " << host << std::endl;
        this->close();
        return false;
    }

    if (connect(this->fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Can't connect to server!");
        this->close();
        return false;
    }
    std::cout << "connected to " << host << std::endl;
    return true;
}

// Отправляем буфер целиком
// единичный send может отправить не всё
bool tcp_link::send_all(const uint8_t* buf, size_t total)
{
    size_t done = 0;
    while (done < total) {
        ssize_t n = send(this->fd, buf + done, total - done, 0);

        if (n < 0) {
            continue;
        }

        if (n <= 0) {
            perror("Can't send data to socket!");
            return false;
        }
        done += (size_t)n;                  // сдвигаю начало следующего send
    }
    return true;
}

// Принимаем данные. Сколько дошло сейчас не важно, собирать кадр будет парсер
// >0 данные, 0 другая сторона закрыла, <0 ошибка
int tcp_link::recv_data(uint8_t* buf, size_t max)
{
    while (true) {
        ssize_t n = recv(this->fd, buf, max, 0);

        if (n < 0) {
            continue;
        }

        return n;
    }
}
