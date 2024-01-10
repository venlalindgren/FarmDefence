#include "Obstacle.hpp"


// constructor, which takes the type of the obstacle (std::string), x and y position (int) and price (int) as parameters
Obstacle::Obstacle(std::string type, int xPos, int yPos, int price)
    : type_(type), xPos_(xPos), yPos_(yPos), price_(price) {}

// destructor
Obstacle::~Obstacle() {}

// returns the type of the obstacle as std::string
std::string Obstacle::getType() {
    return type_;
}

// returns the x position of the obstacle as int
int Obstacle::getXPos() const {
    return xPos_;
}

// returns the y position of the obstacle as int
int Obstacle::getYPos() const {
    return yPos_;
}

// returns the price of the obstacle as int
int Obstacle::getPrice() const {
    return price_;
}

