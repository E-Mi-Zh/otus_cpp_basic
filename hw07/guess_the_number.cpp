#include <iostream>
#include <ctime>    // for std::time
#include <fstream> //for file ops

int main()
{
    const int max_number = 100;
    std::srand(std::time(nullptr));
    const int guessed_number = std::rand() % max_number;
    int guess;
    int attempts = 0;

    std::cout << "Hi! Enter your name, please:" << std::endl;
	std::string username;
	std::cin >> username;

	std::cout << "Guessed number: " << guessed_number << std::endl;

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

	const std::string high_scores_filename = "high_scores.txt";

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

	// Read the high score file and print all results
    std::ifstream scores_in_file{high_scores_filename};
    if (!scores_in_file.is_open()) {
        std::cout << "Failed to open file for read: " << high_scores_filename << "!" << std::endl;
        return -1;
    }

    std::cout << std::endl;
    std::cout << "High scores table:" << std::endl;

    // std::string username;
    int highscore = 0;
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