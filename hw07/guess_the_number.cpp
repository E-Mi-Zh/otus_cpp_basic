#include <iostream>
#include <ctime>    // for std::time
#include <fstream> //for file ops

constexpr unsigned int MAX_NUMBER = 100;
constexpr unsigned int MAX_USERS = 100;

static int max_number = MAX_NUMBER;
static bool scores_only = false;

int process_args(const int argc, char** argv)
{
    std::string arg_value;
    bool max_number_set = false;
    unsigned int level = 0;

    if (argc < 2) {
        // We have no arguments
        return 0;
    }

    for (int i = 1; i <= (argc-1); i++) {
        arg_value = argv[i];
		if (arg_value == "-table") {
            scores_only = true;
            // We don't need parse other args because with `-table` we will not play actually
            return 0;
		}
		if (arg_value == "-max") {
            if (max_number_set) {
                std::cout << "Wrong usage! The argument '-max' conflicts with argument '-level'!" << std::endl;
				return -1;
            }
			// We've detected the '-max' argument. And we expect that after this argument there is a value:
            if (argc < 3) {
				std::cout << "Wrong usage! The argument '-max' requires some value!" << std::endl;
				return -1;
			}
            max_number_set = true;
			// We need to parse the string to the int value
            i++;
            max_number = std::stoi(argv[i]);
		}
		if (arg_value == "-level") {
            if (max_number_set) {
                std::cout << "Wrong usage! The argument '-level' conflicts with argument '-max'!" << std::endl;
				return -1;
            }
			// We've detected the '-level' argument. And we expect that after this argument there is a value:
            if (argc < 3) {
				std::cout << "Wrong usage! The argument '-level' requires some value!" << std::endl;
				return -1;
			}
            max_number_set = true;
			// We need to parse the string to the int value
            i++;
            level = std::stoi(argv[i]);
            switch (level) {
                case 1:
                    max_number = 10;
                    break;
                case 2:
                    max_number = 50;
                    break;
                case 3:
                    max_number = 100;
                    break;
                default:
                    std::cout << "Wrong usage! The argument '-level' value from 1 to 3! Setting to 1" << std::endl;
                    max_number = 10;
                    break;
            }
		}
    }

    return 0;
}

unsigned int get_random_number(const unsigned int max_number = MAX_NUMBER)
{
    std::srand(std::time(nullptr));
    return (std::rand() % max_number);
}

const std::string get_username()
{
	std::string username;

    std::cout << "Hi! Enter your name, please:" << std::endl;
	std::cin >> username;

    return username;
}

int save_highscore(const std::string username, const int attempts, const std::string high_scores_filename = "high_scores.txt")
{
    // Write new high score to the records table
	// We should open the output file in the append mode - we don't want
    // to erase previous results.
    std::ofstream scores_out_file{high_scores_filename, std::ios_base::app};
    if (!scores_out_file.is_open()) {
        std::cout << "Failed to open file for write: " << high_scores_filename << "!" << std::endl;
        return -1;
    }

    // Append new results to the table:
    scores_out_file << username << ' ';
    scores_out_file << attempts;
    scores_out_file << std::endl;

    return 0;
}

int print_highscore(const std::string high_scores_filename = "high_scores.txt")
{
    int highscore = 0;
    std::string username;

    // Read the high score file and print all results
    std::ifstream scores_in_file{high_scores_filename};
    if (!scores_in_file.is_open()) {
        std::cout << "Failed to open file for read: " << high_scores_filename << "!" << std::endl;
        return -1;
    }

    std::cout << std::endl;
    std::cout << "High scores table:" << std::endl;

    while (true) {
        // Read the username first
        scores_in_file >> username;
        // Read the high score next
        scores_in_file >> highscore;
        // Ignore the end of line symbol
        scores_in_file.ignore();

        if (scores_in_file.fail()) {
            break;
        }

        // Print the information to the screen
        std::cout << username << ' ' << highscore << std::endl;
    }

    return 0;
}

/*
*  Дополнительное задание 3: выводить только минимальное значение числа попыток
*  для каждого пользователя.
*
*  Очевидно, что могут быть разные решения этой задачи. Во-первых просится некая
*  структура данных в динамической памяти (мы её ещё не проходили), т.к. число
*  пользователей заранее неизвестно.
*  Во-вторых это может быть стандартный контейнер (мы их ещё не проходили) - вектор,
*  список или хеш-таблица.
*  В-третьих, предполагая, что мы находимся в Линуксе, можно было бы воспользоваться
*  внешними утилитами вроде sort и uniq.
*
*  Однако т.к. на данный момент мы прошли только базовые типы C++ попробуем обойтись
*  ими.
*/

/* В Си есть qsort, можно было бы прочитать весь файл в массив и
*  отсортировать его. В C++ не знаю аналога, писать свой лень, поэтому
*  будем искать перебором "в лоб".
*/
int get_username_index(std::string users[], std::string username, unsigned int& total_users)
{
    unsigned int i;
    for (i = 0; ((i < MAX_USERS) && (i < total_users)); i++) {
        if (username == users[i]) {
            return i;
        }
    }

    // nothing found
    total_users = i;
    return -1;
}

int print_only_min_highscore(const std::string high_scores_filename = "high_scores.txt")
{
    unsigned int highscore = 0;
    std::string username;
    std::string users[MAX_USERS];
    unsigned int attempts[MAX_USERS];
    int user_index = 0;
    unsigned int total_users = 0;

    // Read the high score file and print all results
    std::ifstream scores_in_file{high_scores_filename};
    if (!scores_in_file.is_open()) {
        std::cout << "Failed to open file for read: " << high_scores_filename << "!" << std::endl;
        return -1;
    }

    std::cout << std::endl;
    std::cout << "High scores table:" << std::endl;

    while (true) {
        // Read the username first
        scores_in_file >> username;
        // Read the high score next
        scores_in_file >> highscore;
        // Ignore the end of line symbol
        scores_in_file.ignore();

        if (scores_in_file.fail()) {
            break;
        }

        user_index = get_username_index(users, username, total_users);
        if (user_index < 0) {
            // add new user
            users[total_users] = username;
            attempts[total_users] = highscore;
            total_users++;
            continue;
        }

        // check and modify highscore if needed
        if (highscore < attempts[user_index]) {
            attempts[user_index] = highscore;
        }
    }

    for (unsigned int i = 0; i < total_users; i++) {
        // Print the information to the screen
        std::cout << users[i] << ' ' << attempts[i] << std::endl;
    }

    return 0;
}

int load_all_scores(std::string users[], unsigned int attempts[], const std::string high_scores_filename = "high_scores.txt")

{
    unsigned int i = 0;

    std::ifstream scores_in_file{high_scores_filename, std::ios_base::in};
    if (!scores_in_file.is_open()) {
        std::cout << "Failed to open file for read: " << high_scores_filename << "!" << std::endl;
        return -1;
    }

    while (true) {
        scores_in_file >> users[i];
        scores_in_file >> attempts[i];
        scores_in_file.ignore();

        if (scores_in_file.fail()) {
            break;
        }

        i++;

        if (i == MAX_USERS) {
            std::cout << "load_all_scores: Max users number reached, exiting" << std::endl;
            break;
        }
    }

    return i;
}

int save_only_min_highscore(const std::string username, const unsigned int attempt, const std::string high_scores_filename = "high_scores.txt")
{
    std::string users[MAX_USERS];
    unsigned int attempts[MAX_USERS];
    int total_users = 0;
    bool found_user = false;

    total_users = load_all_scores(users, attempts, high_scores_filename);
    std::cout << "total_users=" << total_users << std::endl;
    std::cout << "username=" << username << "\tattempt=" << attempt << std::endl;
    if (total_users < 0) {
        std::cout << "Error loading highscores!" << std::endl;
    }

    std::ofstream scores_out_file{high_scores_filename, std::ios_base::trunc};
    if (!scores_out_file.is_open()) {
        std::cout << "Failed to open file for write: " << high_scores_filename << "!" << std::endl;
        return -1;
    }

    for (int i = 0; i < total_users; i++) {
        if (users[i] == username) {
            std::cout << "Q! i=" << i << " attempts[i]" << attempts[i] << std::endl;
            found_user = true;
            if (attempt < attempts[i]) {
                attempts[i] = attempt;
            }
            std::cout << "Q! i=" << i << " attempts[i]" << attempts[i] << std::endl;
        }
        std::cout << i << " " << users[i] << " " << attempts[i] << std::endl;
        scores_out_file << users[i] << ' ';
        scores_out_file << attempts[i];
        scores_out_file << std::endl;
    }

    if (!found_user) {
        // we have new user, append him
        scores_out_file << username << ' ';
        scores_out_file << attempt;
        scores_out_file << std::endl;
    }

    return 0;
}

int main(int argc, char** argv)
{
    unsigned int guess;
    unsigned int attempts = 0;

    if (process_args(argc, argv) < 0) {
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    if (scores_only) {
        // print_highscore();
        print_only_min_highscore();
        return 0;
    }

    const unsigned int guessed_number = get_random_number(max_number);

	std::cout << "Guessed number: " << guessed_number << std::endl;
    const std::string username = get_username();
    std::cout << "Enter your guess:" << std::endl;

    while (true) {
        std::cin >> guess;
        attempts++;

        if (guess < guessed_number) {
            std::cout << "less than " << guess << std::endl;
        }
        else if (guess > guessed_number) {
            std::cout << "greater than " << guess << std::endl;
        }
        else {
            std::cout << "you win! attempts = " << attempts << std::endl;
            break;
        }
	};

    // save_highscore(username, attempts);
    save_only_min_highscore(username, attempts);
    // print_highscore();
    print_only_min_highscore();

    return 0;
}