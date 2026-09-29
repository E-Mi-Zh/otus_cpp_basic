#include <iostream>
#include <string>

#include "codec.h"
#include "base64.h"

// Проверяем, что строка целиком из цифр
bool all_digits(const std::string& s)
{
    if (s.empty()) {
        return false;
    }

    for (size_t i = 0; i < s.size(); i++) {
        if ((s[i] < '0') || (s[i] > '9')) {
            return false;
        }
    }

    return true;
}

// Извлекаем данные после "FT:": seq, frag/total, данные.
bool split_ft(const std::string& rest, int& seq, int& frag, int& total, std::string& data)
{
    // конец seq
    size_t c1 = rest.find(':');
    if (c1 == std::string::npos) {
        return false;
    }

    // конец frag/total
    size_t c2 = rest.find(':', c1 + 1);
    if (c2 == std::string::npos) {
        return false;
    }

    // seq до первого ':'
    std::string seq_s = rest.substr(0, c1);

    // frag/total
    std::string mid = rest.substr(c1 + 1, c2 - c1 - 1);

    // оставшееся - это данные в Base64
    data = rest.substr(c2 + 1);

    // разбиваем frag/total на части
    size_t slash = mid.find('/');
    if (slash == std::string::npos) {
        return false;
    }

    // до '/'
    std::string frag_s = mid.substr(0, slash);
    // после '/'
    std::string total_s = mid.substr(slash + 1);

    if (!all_digits(seq_s) || !all_digits(frag_s) || !all_digits(total_s)) {
        return false;
    }
    // номер кадра
    seq = std::stoi(seq_s);
    // номер фрагмента, с 1
    frag = std::stoi(frag_s);
    // всего фрагментов
    total = std::stoi(total_s);

    return true;
}

irc_codec::irc_codec()
{
    this->next_seq = 1;         // первый кадр будет FT:1
    this->cur_seq = -1;
    this->cur_total = 0;
    this->cur_got = 0;
    this->fatal = false;
}

const char* irc_codec::name() const
{
    return "irc";
}

// выбрасываем  недособранный кадр
void irc_codec::reset_asm()
{
    this->cur_seq = -1;
    this->cur_total = 0;
    this->cur_got = 0;
    this->parts.clear();
}

// Кодируем кадр в строки PRIVMSG по IRC_B64_CHUNK символов в Base64
bool irc_codec::encode(const uint8_t* frame, size_t frame_len, std::vector<uint8_t>& out)
{
    if (frame == 0 && frame_len != 0) {
        std::cout << "irc encode: null frame" << std::endl;
        return false;
    }

    std::string b64;
    if (!base64_encode(frame, frame_len, b64)) {
        return false;
    }

    // даже пустая строка это строка чата
    int total = 1;
    if (!b64.empty()) {
        // сколько кусков?
        // округляем вверх от длины Base64 / размер фрагмента.
        total = (int)((b64.size() + IRC_B64_CHUNK - 1) / IRC_B64_CHUNK);
    }

    int seq = this->next_seq;
    // для следующего encode увеличим seq
    this->next_seq = this->next_seq + 1;
    out.clear();

    // нумеруем фрагменты с 1
    for (int frag = 1; frag <= total; frag++) {
        // Смещение текущего фрагмента внутри строки Base64.
        size_t off = (size_t)(frag - 1) * IRC_B64_CHUNK;

        // длина по умолчанию не больше лимита
        size_t n = IRC_B64_CHUNK;
        if (off >= b64.size()) {
            // Пустой кадр - единственный фрагмент без данных
            n = 0;
        } else if (off + n > b64.size()) {
            // Последний фрагмент короче лимита - берём остаток Base64.
            n = b64.size() - off;
        }

        // символы Base64, не байты кадра
        std::string chunk = b64.substr(off, n);

        // Явно добавляем \r\n
        std::string line = "PRIVMSG #tunnel :FT:" + std::to_string(seq) + ":"
            + std::to_string(frag) + "/" + std::to_string(total) + ":" + chunk + "\r\n";

        // Добавляем готовую строку в выходной буфер
        // для отправки send
        out.insert(out.end(), line.begin(), line.end()); // байты для send
    }

    return true;
}

// Принимаем строку без \r\n
// служебные строки IRC пропускаем (логируем)
void irc_codec::take_line(const std::string& line)
{
    // префикс строки, отправляем в PRIVMSG
    const std::string prefix = "PRIVMSG #tunnel :FT:";
    // пропускаем посторонние строки
    if (line.size() < prefix.size() || line.compare(0, prefix.size(), prefix) != 0) {
        std::cout << "irc: skip foreign line" << std::endl;
        return;
    }

    int seq = 0;
    int frag = 0;
    int total = 0;
    std::string data;

    // Пытаемся распарсить строку
    if (!split_ft(line.substr(prefix.size()), seq, frag, total, data)) {
        std::cout << "irc: bad line, skip" << std::endl;
        return;
    }

    // проверяем лимиты, 256 фрагментов достаточно для MAX_FRAME_SIZE
    if (total < 1 || total > 256 || frag < 1 || frag > total) {
        std::cout << "irc: bad frag, skip" << std::endl;
        return;
    }

    if (frag == 1) {
        // Если начинаем новый кадр, то предыдущий выбрасываем
        if (this->cur_seq != -1 && this->cur_got != this->cur_total) {
            std::cout << "irc: drop incomplete seq " << this->cur_seq << std::endl;
        }

        this->cur_seq = seq;
        this->cur_total = total;
        this->cur_got = 0;
        this->parts.assign((size_t)total, std::string());
    }

    if (this->cur_seq < 0 || seq != this->cur_seq) {
        // frag 2 без frag 1, или чужой номер
        std::cout << "irc: foreign fragment seq " << seq << ", skip" << std::endl;
        return;
    }

    if (total != this->cur_total || frag != this->cur_got + 1) {
        // пропустили фрагмент или другой total - дальше собирать кадр бессмысленно
        // будем ждать следующего frag 1
        std::cout << "irc: hole in seq " << seq << ", drop" << std::endl;
        this->reset_asm();
        return;
    }

    // сохраняем фрагмент
    this->parts[(size_t)frag - 1] = data;
    // счётчик принятых
    this->cur_got = this->cur_got + 1;

    if (this->cur_got != this->cur_total) {
        // кадр собран не весь
        return;
    }

    // склеиваем фрагменты в Base64 строку
    std::string all;
    for (size_t i = 0; i < this->parts.size(); i++) {
        all = all + this->parts[i];
    }

    // байты кадра
    std::vector<uint8_t> raw;
    // пытаемся декодировать Base64
    if (!base64_decode(all, raw)) {
        std::cout << "irc: bad base64, drop seq " << seq << std::endl;
        this->reset_asm();
        return;
    }

    // Пробуем распарсить кадр
    if (!this->parser.consume(raw.data(), raw.size())) {
        // magic или длина кадра битые
        std::cout << "irc: frame parser rejected seq " << seq << std::endl;
        this->reset_asm();
        this->fatal = true;
        return;
    }

    // кадр успешно ушёл в parser, слот свободен для следующего seq
    this->reset_asm();
}

// набираем байты до \r\n
bool irc_codec::decode(const uint8_t* data, size_t data_len)
{
    if ((data == 0) && (data_len != 0)) {
        std::cout << "irc decode: null" << std::endl;
        return false;
    }

    if (data_len > 0) {
        // складываем во внутренний буфер
        this->line_acc.append(reinterpret_cast<const char*>(data), data_len);
    }

    // Если дошли до лимита, а \r\n так и не пришёл, то сбрасываем
    if (this->line_acc.size() > IRC_LINE_MAX) {
        std::cout << "irc: line too long, drop" << std::endl;
        this->line_acc.clear();
        return false;
    }

    // в одном recv может быть несколько строк, переберём в цикле
    while (true) {
        // ищем разделитель
        size_t p = this->line_acc.find("\r\n");
        if (p == std::string::npos) {
            return !this->fatal;
        }

        // удаляем \r\n
        std::string line = this->line_acc.substr(0, p);

        // удаляем из буфера-накопителя
        this->line_acc.erase(0, p + 2);

        // обрабатываем (парсим)
        this->take_line(line);

        if (this->fatal) {
            return false;
        }
    }
}

// возвращает собранный кадр из парсера
bool irc_codec::pop_frame(std::vector<uint8_t>& frame)
{
    // false, если кадр ещё не собрался
    return this->parser.pop(frame);
}
