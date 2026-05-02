#include <iostream>
#include <ctime>    // for std::time

int main()
{
    const int max_number = 100;
    std::srand(std::time(nullptr));
    const int guessed_number = std::rand() % max_number;
    int guess;

	std::cout << "Guessed number: " << guessed_number << std::endl;

    std::cout << "Enter your guess:" << std::endl;

    while (true) {
        std::cin >> guess;

        if (guess < guessed_number) {
            std::cout << "less than " << guess << std::endl;
        }
        else if (guess > guessed_number) {
            std::cout << "greater than " << guess << std::endl;
        }
        else {
            std::cout << "you win!" << std::endl;
            break;
        }
	};

    return 0;
}