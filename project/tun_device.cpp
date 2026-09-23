#include <iostream>
#include <cstring>                      // memset, strncpy
#include <cerrno>                       // EINTR
#include <cstdlib>                      // system
#include <fcntl.h>                      // open
#include <unistd.h>                     // read, write, close, access
#include <sys/ioctl.h>                  // ioctl
#include <linux/if.h>                   // ifreq, IFNAMSIZ
#include <linux/if_tun.h>               // TUNSETIFF

#include "tun_device.h"

tun_device::tun_device()
{
    this->fd = -1;
}

tun_device::~tun_device()
{
    this->close();
}

void tun_device::close()
{
    if (this->fd >= 0) {
        ::close(this->fd);
        this->fd = -1;
    }
}

// проверяем, что строка не содержит лишних символов (можно передать в шелл system)
bool tun_device::plain_token(const std::string& s)
{
    if (s.empty() || (s.size() >= IFNAMSIZ)) {
        return false;
    }

    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        // буквы, цифры, точка и слэш для CIDR
        bool ok = ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||
                  ((c >= '0') && (c <= '9')) || (c == '.') || (c == '/') || (c == '_') || (c == '-');
        // пробел или ; | & ломают шелл
        if (!ok) {
            return false;
        }
    }

    return true;
}

// Печатает команду ip и выполняет её
bool tun_device::run_cmd(const std::string& cmd)
{
    std::cout << "run_cmd: " << cmd << std::endl;
    int rc = std::system(cmd.c_str());

    if (rc != 0) {
        std::cout << "cmd return " << rc << std::endl;
        return false;
    }

    return true;
}

// Создаёт интерфейс ioctl-ом, затем назначает адрес и поднимает его.
bool tun_device::open(const std::string& name, const std::string& cidr)
{
    // Проверяем, что имя интерфейса и CIDR не содержат лишних символов
    if (!this->plain_token(name) || !this->plain_token(cidr)) {
        std::cout << "bad tun name or addr" << std::endl;
        return false;
    }

    this->close();
    this->fd = ::open("/dev/net/tun", O_RDWR);

    if (this->fd < 0) {
        perror("Can't open tun!");
        return false;
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    // L3 без служебного заголовка
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ - 1);
    if (ioctl(this->fd, TUNSETIFF, &ifr) < 0) {
        perror("ioctl TUNSETIFF error!");
        this->close();
        return false;
    }

    this->ifname = ifr.ifr_name;
    // имя команды без пути
    const char* ip = "ip";
    if (access("/sbin/ip", X_OK) == 0) {
        ip = "/sbin/ip";
    } else if (access("/usr/sbin/ip", X_OK) == 0) {
        ip = "/usr/sbin/ip";
    }

    // формируем строки для system
    std::string add = std::string(ip) + " addr add " + cidr + " dev " + this->ifname;
    std::string up = std::string(ip) + " link set " + this->ifname + " up";

    // назначаем IP
    if (!this->run_cmd(add)) {
        this->close();
        return false;
    }

    // поднимаем интерфейс
    if (!this->run_cmd(up)) {
        this->close();
        return false;
    }

    std::cout << "TUN " << this->ifname << " up and ready!" << cidr << std::endl;

    return true;
}

// Блокирующее чтение одного IP-пакета
// возвращаем длину пакета, отрицательное - ошибка
int tun_device::read_packet(uint8_t* buf, size_t max)
{
    while (true) {
        ssize_t n = ::read(this->fd, buf, max);
        if (n < 0) {
            continue;
        }
        return n;
    }
}

// Отправляем пакет в TUN
bool tun_device::write_packet(const uint8_t* buf, size_t n)
{
    // сколько байт записано
    size_t done = 0;

    while (done < n) {
        ssize_t w = ::write(this->fd, buf + done, n - done);
        if (w < 0) {
            continue;
        }

        if (w <= 0) {
            perror("Can't write to TUN");
            return false;
        }
        done = done + w;
    }

    return true;
}
