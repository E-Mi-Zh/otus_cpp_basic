#include <algorithm>
#include <iostream>
#include <limits>
#include <thread>
#include <vector>

#include "CRC32.hpp"
#include "IO.hpp"

/// @brief Переписывает последние 4 байта значением value
void replaceLastFourBytes(std::vector<char>& data, uint32_t value) {
  std::copy_n(reinterpret_cast<const char*>(&value), 4, data.end() - 4);
}

void try_hack(unsigned int batch, std::vector<char> data, size_t from,
              size_t to, const uint32_t originalCrc32,
              std::vector<char>* result) {
  std::cout << "from = " << from << " to = " << to << std::endl;

  /*
   * Внимание: код ниже крайне не оптимален.
   * В качестве доп. задания устраните избыточные вычисления
   */
  for (size_t i = from; i < to; ++i) {
    // Заменяем последние четыре байта на значение i
    replaceLastFourBytes(data, uint32_t(i));
    // Вычисляем CRC32 текущего вектора result
    auto currentCrc32 = crc32(data.data(), data.size());

    if (currentCrc32 == originalCrc32) {
      std::cout << "Success\n";
      *result = data;
      return;
    }
    // Отображаем прогресс
    if (i % 1000 == 0) {
      std::cout << "batch " << batch << " progress: "
                << static_cast<double>(i) / static_cast<double>(to)
                << std::endl;
    }
  }
}

/**
 * @brief Формирует новый вектор с тем же CRC32, добавляя в конец оригинального
 * строку injection и дополнительные 4 байта
 * @details При формировании нового вектора последние 4 байта не несут полезной
 * нагрузки и подбираются таким образом, чтобы CRC32 нового и оригинального
 * вектора совпадали
 * @param original оригинальный вектор
 * @param injection произвольная строка, которая будет добавлена после данных
 * оригинального вектора
 * @return новый вектор
 */
std::vector<char> hack(const std::vector<char>& original,
                       const std::string& injection) {
  const uint32_t originalCrc32 = crc32(original.data(), original.size());

  std::vector<char> result(original.size() + injection.size() + 4);
  auto it = std::copy(original.begin(), original.end(), result.begin());
  std::copy(injection.begin(), injection.end(), it);

  unsigned int i;
  unsigned int n_threads = std::thread::hardware_concurrency();
  std::cout << "We can work in " << n_threads << std::endl;

  const size_t maxVal = std::numeric_limits<uint32_t>::max();
  size_t batch_size = maxVal / n_threads;
  std::cout << "Batch size " << batch_size << std::endl;
  std::vector<std::thread> threads(n_threads);
  std::vector<std::vector<char>> results(n_threads);

  size_t from = 0;
  size_t to = batch_size;

  for (i = 0; i < n_threads; i++) {
    threads[i] =
        std::thread(try_hack, i, result, from, to, originalCrc32, &results[i]);
    from = from + batch_size;
    to = to + batch_size;
  }

  for (i = 0; i < n_threads; i++) {
    threads[i].join();
  }

  for (i = 0; i < n_threads; i++) {
    if (!results[i].empty()) {
      return results[i];
    }
  }

  throw std::logic_error("Can't hack");
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Call with two args: " << argv[0]
              << " <input file> <output file>\n";
    return 1;
  }

  try {
    const std::vector<char> data = readFromFile(argv[1]);
    const std::vector<char> badData = hack(data, "He-he-he");
    writeToFile(argv[2], badData);
  } catch (std::exception& ex) {
    std::cerr << ex.what() << '\n';
    return 2;
  }
  return 0;
}
