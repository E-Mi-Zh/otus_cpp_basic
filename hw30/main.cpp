#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <thread>
#include <vector>
#include <chrono>
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

  /* Вычисляем CRC32 от неизменяемой части */
  auto prefixCrc32 = crc32(data.data(), data.size() - 4);
  /* В цикле будем считать CRC32 только для изменяемого хвоста*/
  std::vector<char> value(4);

  for (size_t i = from; i < to; ++i) {
    // Заменяем последние четыре байта на значение i
    replaceLastFourBytes(value, uint32_t(i));
    // Вычисляем CRC32 хвоста
    auto currentCrc32 = crc32(value.data(), value.size(), ~prefixCrc32);

    if (currentCrc32 == originalCrc32) {
      std::cout << "Success\n";
      replaceLastFourBytes(data, uint32_t(i));
      *result = data;
      return;
    }
    // Отображаем прогресс
    if (i % 1000 == 0) {
      std::cout << "batch " << batch << " progress: "
                << static_cast<double>(i) / static_cast<double>(to)
                << "\n";
    }
  }
}

void profile_crc(std::vector<char> data, size_t from, unsigned int n, uint32_t* res) {
  unsigned int sum_res = 0;
  /* Вычисляем CRC32 от неизменяемой части */
  auto prefixCrc32 = crc32(data.data(), data.size() - 4);
  /* В цикле будем считать CRC32 только для изменяемого хвоста*/
  std::vector<char> value(4);
  for (size_t i = from; i < from + n; ++i) {
    // Заменяем последние четыре байта на значение i
    replaceLastFourBytes(value, uint32_t(i));
    // Вычисляем CRC32 хвоста
    auto currentCrc32 = crc32(value.data(), value.size(), ~prefixCrc32);
    sum_res = sum_res + currentCrc32;
  }
  *res = sum_res;
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

  std::cout << "Trying to determine optimal number of threads!"<< std::endl;
  std::vector<double> times(n_threads);
  std::vector<unsigned int> crc_res(n_threads);

  for (unsigned int j = 0; j < n_threads; j++) {
    std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    from = 0;
    for (i = 0; i < j+1; i++) {
      threads[i] = std::thread(profile_crc, result, from, static_cast<unsigned int>(10000000 / (j+1)), &crc_res[i]);
      from = from + batch_size;
    }
    for (i = 0; i < j+1; i++) {
      threads[i].join();
    }
    std::chrono::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Threads " << j+1 << " took " << elapsed.count() << " seconds" << std::endl;
    times[j] = elapsed.count();
  }

  unsigned int min_threads = n_threads;
  double min_time = times[0];
  for (i = 0; i < n_threads; i++) {
    if (times[i] < min_time) {
      min_threads = i;
      min_time = times[i];
    }
  }
  std::cout << "Optimal N of threads " << min_threads + 1 << std::endl;

  /* Пытаемся использовать результаты расчётов, чтобы компилятор их не выкинул */
  uint64_t sum_res = 0;
  for (i = 0; i < n_threads; i++){
    sum_res = sum_res + crc_res[i];
  }
  std::cout << "sum_res = " << sum_res << std::endl;

  n_threads = min_threads;

  from = 0;
  to = batch_size;

  for (i = 0; i < n_threads; i++) {
    threads[i] = std::thread(try_hack, i, result, from, to, originalCrc32, &results[i]);
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
