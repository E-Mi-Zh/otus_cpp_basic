// Read files and prints top k word by frequency

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <chrono>
#include <mutex>                // mutex & lock_guard
#include <thread>               // threads

const size_t TOPK = 10;

using Counter = std::map<std::string, std::size_t>;

Counter freq_dict;

static std::mutex dict_mutex;

std::string tolower(const std::string &str);

void count_words(std::istream& stream, Counter&);

void print_topk(std::ostream& stream, const Counter&, const size_t k);

void count_files(unsigned int thread_id, unsigned int n_threads, unsigned int n_files, char** argv);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    unsigned int n_threads = std::thread::hardware_concurrency();
    if (n_threads == 0) {
        n_threads = 1;
    }

    std::cout << "Can have up to " << n_threads << " threads" << std::endl;

    unsigned int n_files = static_cast<unsigned int>(argc - 1);
    std::cout << "Got " << n_files << " input files" << std::endl;

    if (n_threads > n_files) {
        n_threads = n_files;
    }

    std::cout << "Will work in " << n_threads << " threads" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 1; i < argc; ++i) {
        std::ifstream input{argv[i]};
        if (!input.is_open()) {
            std::cerr << "Failed to open file " << argv[i] << '\n';
            return EXIT_FAILURE;
        }
    }

    std::vector<std::thread> threads(n_threads);
    for (unsigned int i = 0; i < n_threads; i++) {
        threads[i] = std::thread(count_files, i, n_threads, n_files, argv);
    }

    for (unsigned int i = 0; i < n_threads; i++) {
        threads[i].join();
    }

    print_topk(std::cout, freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
}

std::string tolower(const std::string &str) {
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str),
                   std::back_inserter(lower_str),
                   [](unsigned char ch) { return std::tolower(ch); });
    return lower_str;
};

void count_words(std::istream& stream, Counter& counter) {
    std::for_each(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  [&counter](const std::string &s) {
                    std::lock_guard<std::mutex> lock(dict_mutex);
                    ++counter[tolower(s)];
                });
}

void print_topk(std::ostream& stream, const Counter& counter, const size_t k) {
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());
    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
        words.push_back(it);
    }

    std::partial_sort(
        std::begin(words), std::begin(words) + static_cast<std::ptrdiff_t>(k), std::end(words),
        [](auto lhs, auto &rhs) { return lhs->second > rhs->second; });

    std::for_each(
        std::begin(words), std::begin(words) + static_cast<std::ptrdiff_t>(k),
        [&stream](const Counter::const_iterator &pair) {
            stream << std::setw(4) << pair->second << " " << pair->first
                      << '\n';
        });
}

void count_files(unsigned int thread_id, unsigned int n_threads,
                 unsigned int n_files, char** argv) {
    for (unsigned int i = 1 + thread_id; i <= n_files; i = i + n_threads) {
        std::ifstream input{argv[i]};
        count_words(input, freq_dict);
    }
}
