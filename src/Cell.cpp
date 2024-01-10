#include "Cell.hpp"


// constructor, which takes cellType and x and y position as parameters
// the cell's enemy, tower and obstacle are null pointers by default
Cell::Cell(std::string cellType, int xPos, int yPos)
     : cellType_(cellType), xPos_(xPos), yPos_(yPos), enemy_(nullptr), tower_(nullptr), obstacle_(nullptr) {}

// destructor
Cell::~Cell() {}

// returns a string describing whether the cell is "land" or "path"
std::string Cell::getCellType() const {
    return cellType_;
}

// returns the enemy in the cell (Enemy* enemy_)
// if there is no enemy, returns null pointer
Enemy* Cell::getEnemy() {
    return enemy_;
}

// adds enemy (type Enemy*) given as parameter to the cell
// returns nothing
void Cell::addEnemy(Enemy* enemy) { 
    enemy_ = enemy;
}

// removes enemy from the cell and sets enemy_ as null pointer
// returns nothing
void Cell::removeEnemy() {
    enemy_ = nullptr;
}

// returns the tower in the cell (Tower* tower_)
// if there is no tower, returns null pointer
Tower* Cell::getTower() {
    return tower_;
}

// adds tower (type Tower*) given as parameter to the cell
// returns nothing
void Cell::addTower(Tower* tower) {
    tower_ = tower;
}

// removes tower from the cell and sets tower_ as null pointer
// returns nothing
void Cell::removeTower() {
    tower_ = nullptr;
}

// returns the obstacle in the cell (Obstacle* obstacle_)
// if there is no obstacle, returns null pointer
Obstacle* Cell::getObstacle() {
    return obstacle_;
}

// adds obstacle (type Obstacle*) given as parameter to the cell
// returns nothing
void Cell::addObstacle(Obstacle* obstacle) {
    obstacle_ = obstacle; 
}

// removes obstacle from the cell and sets obstacle_ as null pointer
// returns nothing
void Cell::removeObstacle() {
    obstacle_ = nullptr;
}

// returns the x coordinates of the cell as a float
float Cell::getX() const {
    return xPos_;
}

// returns the y coordinates of the cell as a float
float Cell::getY() const {
    return yPos_;
}

// returns bool value which is true if cell type is path and false if not
bool Cell::isPath() {
    if (cellType_ == "path") {
        return true;
    }
    else
        return false;
}

// returns bool value which is true if cell is empty (no obstacle, enemy or tower) and false if not
bool Cell::isEmpty() {
    if (obstacle_ == nullptr && tower_ == nullptr && enemy_ == nullptr)
        return true;
    else   
        return false;

}

