#include <iostream>
#include "Game.hpp"

// main function which starts the game, takes no parameters
// returns int value, which is 0 if the game started successfully, otherwise returns 1
int main() {
    try {
        Game game;
        game.startGame();
    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }
    return 0;
}

