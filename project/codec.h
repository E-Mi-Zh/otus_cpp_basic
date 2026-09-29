#pragma once

#include <cstddef>      // size_t
#include <cstdint>      // uint8_t
#include <vector>
#include <string>

#include "tunnel_frame.h"

// Класс-интерфейс кодека, конкретный алгоритм будут реализовывать плагины
class i_codec {
public:
    virtual ~i_codec() {}
    // имя плагина для логов и для хелпа
    virtual const char* name() const = 0;
    // кодируем кадр в поток байт TCP
    virtual bool encode(const uint8_t* frame, size_t frame_len, std::vector<uint8_t>& out) = 0; // кадр -> байты в TCP
    // забирает байты из сокета и хранит их
    virtual bool decode(const uint8_t* data, size_t data_len) = 0;
    // возвращает готовый кадр
    virtual bool pop_frame(std::vector<uint8_t>& frame) = 0;
};

// Identity - кодек заглушка, ничего не добавляет к кадру, для тестов
class identity_codec : public i_codec {
public:
    const char* name() const override;
    bool encode(const uint8_t* frame, size_t frame_len, std::vector<uint8_t>& out) override;
    bool decode(const uint8_t* data, size_t data_len) override;
    bool pop_frame(std::vector<uint8_t>& frame) override;
private:
    frame_parser parser;
};

// Padding кодек: дописываем перед кадром N случайных байт
#define PAD_MIN 0
#define PAD_MAX 64

// состояния для кодека (автомата)
enum pad_state {
    PAD_WAIT_N,      // ждём 2 байта длины
    PAD_WAIT_PAD,    // пропускаем N байт мусора
    PAD_WAIT_FRAME   // ждём пока наберётся кадр для передачи парсеру
};

class padding_codec : public i_codec {
public:
    // сбрасывает автомат, начинаем с WAIT_N
    padding_codec();
    const char* name() const override;
    // Пишет длину N, N случайных байт и затем кадр
    bool encode(const uint8_t* frame, size_t frame_len, std::vector<uint8_t>& out) override;
    // снимает обёртку частями и складывает целые кадры в парсер
    bool decode(const uint8_t* data, size_t data_len) override;
    // возвращает кадр паддинга
    bool pop_frame(std::vector<uint8_t>& frame) override;
private:
    // внутреннее состояние, одно из PAD_WAIT_N | PAD | FRAME
    int state;
    // сколько лишних байт (паддинга) осталось отбросить
    uint16_t pad_left;
    size_t frame_total;
    // необработанные байты
    std::vector<uint8_t> stash;
    // собираемый кадр (когда находимся в WAIT_FRAME)
    std::vector<uint8_t> frame_acc;
    frame_parser parser;
};

// Сколько символов Base64 класть в одну строку PRIVMSG
#define IRC_B64_CHUNK 400
#define IRC_LINE_MAX 8192

// Эмуляция IRC: кадр отправляется строками PRIVMSG, не байтами
// Конец кадра - поле total в frag/total
class irc_codec : public i_codec {
public:
    irc_codec();
    const char* name() const override;
    // Кодирует кадр в одну или несколько строк PRIVMSG (через \r\n).
    bool encode(const uint8_t* frame, size_t frame_len, std::vector<uint8_t>& out) override;
    // Собирает принятые байты (символы) в буфер
    bool decode(const uint8_t* data, size_t data_len) override;
    // Отдаёт чистый кадр в TUN
    bool pop_frame(std::vector<uint8_t>& frame) override;
private:
    // сбрасываем сборку кадра
    void reset_asm();
    // обрабатываем строку (парсим)
    void take_line(const std::string& line);
    // номер следующего кадра
    int next_seq;
    // строка где копим символы до \r\n
    std::string line_acc;
    // текущий сегмент
    int cur_seq; // -1, если кадра в сборке нет
    // сколько всего будет
    int cur_total;
    // сколько пришло
    int cur_got;
    // фрагменты Base64
    std::vector<std::string> parts;
    bool fatal;
    frame_parser parser;
};
