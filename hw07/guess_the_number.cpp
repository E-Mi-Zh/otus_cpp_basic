#include <iostream>
#include <ctime>    // for std::time
#include <fstream> //for file ops

constexpr unsigned int MAX_NUMBER = 100;

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

int main()
{
    const int max_number = MAX_NUMBER;
    unsigned int guess;
    int attempts = 0;

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

    save_highscore(username, attempts);
    print_highscore();

    return 0;
}