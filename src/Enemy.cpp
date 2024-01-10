#include "Enemy.hpp"


// constructor, which takes enemy type, x and y position, hitpoints, speed, point value and money value as parameters
// the enemy is created to the cell with coordinates (0, 9)
// the first target cell has coordinates (1, 9) and the direction of the enemy is 1 (to the right) when the enemy is created
Enemy::Enemy(const std::string& type, float xPos, float yPos, int hitpoints, float speed, int pointValue, int moneyValue)
    : type_(type), xPos_(xPos), yPos_(yPos), alive_(true), baseSpeed_(speed), speed_(speed), hitpoints_(hitpoints), pointValue_(pointValue), 
      moneyValue_(moneyValue), nextTargetX_(1), nextTargetY_(9), direction_(1), maxHP_(hitpoints), cellX_(0), cellY_(9), poisonDamaged_(false), mudSlowed_(false) {}
    //nextTargetX_ is the x of the next cell in path (or the x of the current cell if enemy has just arrived to the cell), same for y

// destructor, takes care that enemy is removed from all towers targeting it
Enemy::~Enemy() {
    for (ArrowTower* tower : towersTargetingThisEnemy_) {
        tower->removeTargetEnemy();
    }
}

// returns the cells x position as integer (int cellX_)
int Enemy::getCellX() const {
    return cellX_;
}

// returns the cells y position as integer (int cellY_)
int Enemy::getCellY() const {
    return cellY_;
}

// changes the enemy's current cell x position according to parameter int x
// returns nothing
void Enemy::newCellX(int x) {
    cellX_ = x;
}

// changes the enemy's current cell y position according to parameter int y
// returns nothing
void Enemy::newCellY(int y) {
    cellY_ = y;
}

// returns the enemy's target cell's x coordinates (int nextTargetX_)
int Enemy::getTargetX() {
    return nextTargetX_;
}

// returns the enemy's target cell's y coordinates (int nextTargetY_)
int Enemy::getTargetY() {
    return nextTargetY_;
}

// returns enemy's current x position (float xPos_)
float Enemy::getX() const {
    return xPos_;
}

// returns enemy's current y position (float yPos_)
float Enemy::getY() const {
    return yPos_;
}

// returns the type of the enemy as a string
std::string Enemy::getType() const {
    return type_;
}


/* updates the position of enemy according to the time passed since last frame (float deltaTime)
 and the direction (int dir) of enemy's movement, both are given as parameters */
 // returns nothing
void Enemy::updatePosition(float deltaTime, int dir) {
    float step = speed_ * deltaTime;
    if(dir == 1) {
        float dx = nextTargetX_ - xPos_;
        if (dx > step) {
            xPos_ += step;
        } 
        else {
            xPos_ = nextTargetX_; // if enemy would go past the centre of the target cell, enemy is forced to the middle of the target cell
        }
    }
    else if(dir == 2) {
        float dx = xPos_ - nextTargetX_;
        if (dx > step) {
            xPos_ -= step;
        } 
        else {
            xPos_ = nextTargetX_;
        }
    }
    else if(dir == 3) {
        float dy = nextTargetY_ - yPos_;
        if (dy > step) {
            yPos_ += step;
        } 
        else {
            yPos_ = nextTargetY_;
        }
    }
    else if(dir == 4) {
        float dy = yPos_ - nextTargetY_;
        if (dy > step) {
            yPos_ -= step;
        } 
        else {
            yPos_ = nextTargetY_;
        }
    }
    else {
        return;
    }
}

// returns the direction of the enemy (int direction_)
int Enemy::getDirection() {
    return direction_;
}

// returns the point value of the enemy (int pointValue_)
int Enemy::getPointValue() const {
    return pointValue_;
}

// returns the money value of the enemy (int moneyValue_)
int Enemy::getMoneyValue() const {
    return moneyValue_;
}

// indicates whether the enemy has already been damaged by a poison obstacle (in one cell)
// returns a bool value which is true if enemy has been poisoned and false otherwise
bool Enemy::hasBeenPoisonDamaged() {  
    return poisonDamaged_;
}

// indicates whether the enemy has already been slowed by a mud obstacle (in one cell)
// returns a bool value which is true if enemy has been slowed down and false otherwise
bool Enemy::hasBeenMudSlowed() {
    return mudSlowed_;
}

// changes the state, whether the enemy has been poison damaged or not according to the bool parameter
// if the parameter is true, it changes poison damaged state of the enemy to true, false bool parameter does the opposite
// returns nothing
void Enemy::poisonDamaged(bool state) {
    poisonDamaged_ = state;
}

// changes the state, whether the enemy has been slowed down or not according to the bool parameter
// if the parameter is true, it changes slowed down state of the enemy to true, false bool parameter does the opposite
// returns nothing
void Enemy::mudSlowed(bool state) {
    mudSlowed_ = state;
}

// assigns a new target for enemy and calculates the next movement direction
// takes as parameters the previous target's x and y and also the new target's x and y
// returns nothing
void Enemy::updateTarget(int prevX, int prevY, int newX, int newY) {
    nextTargetX_ = newX;
    nextTargetY_ = newY;
    int xDif = newX - prevX;
    int yDif = newY - prevY;
    if (xDif > 0) {
        direction_ = 1;
    } 
    else if (xDif < 0) {
        direction_ = 2;
    }
    else if (yDif > 0) {
        direction_ = 3;
    }
    else if (yDif < 0) {
        direction_ = 4;
    }
    else {
        return;
    }
}

// adds a tower given as parameter (type ArrowTower*) to the list of towers which are targeting this enemy (std::vector<ArrowTower*> towersTargetingThisEnemy_)
// returns nothing
void Enemy::addTower(ArrowTower* tower) {
    towersTargetingThisEnemy_.push_back(tower);
}

// removes a tower given as parameter (type ArrowTower*) from the list of towers targeting this enemy (std::vector<ArrowTower*> towersTargetingThisEnemy_)
// returns nothing
void Enemy::deleteTower(ArrowTower* towerToDelete) {
    auto it = std::find_if(towersTargetingThisEnemy_.begin(), towersTargetingThisEnemy_.end(), [towerToDelete](const Tower* tower) {
        return (tower->getXPos() == towerToDelete->getXPos() && tower->getYPos() == towerToDelete->getYPos());
    });
    if (it != towersTargetingThisEnemy_.end()) {
        towersTargetingThisEnemy_.erase(it);
    }
}

// returns the distance (as float) of enemy's position to the cell coordinates of the current cell where enemy is in the grid
float Enemy::distanceToCurrentCell() const {
    float dx = std::abs(xPos_ - cellX_);
    float dy = std::abs(yPos_ - cellY_);
    float pythagora = std::sqrt(dx*dx+dy*dy);
    return pythagora;
}

// returns the distance (as float) of enemy's position to the enemy's target cell coordinates
float Enemy::distanceToTargetCell() const {
    float dx = std::abs(xPos_ - nextTargetX_);
    float dy = std::abs(yPos_ - nextTargetY_);
    float pythagora = std::sqrt(dx*dx+dy*dy);
    return pythagora;
}

// updates enemy's hitpoints by reducing the damage given as parameter (int damage)
// changes the state of bool alive_ if enemy dies
// returns nothing
void Enemy::updateHitpoints(int damage) {
    hitpoints_ = std::max(hitpoints_ - damage, 0);
    if (hitpoints_ == 0) {
        alive_ = false;
    }
}

// returns a bool value (alive_) whether the enemy is alive or not
// returns true if enemy is alive, false otherwise
bool Enemy::isAlive() {
    return alive_;
}

// returns the current hitpoints of the enemy (int hitpoints_)
int Enemy::getHP() {
    return hitpoints_;
}

// returns the max hitpoints of the enemy (int maxHP_)
int Enemy::getMaxHP() {
    return maxHP_;
}

// returns the base speed of the enemy (float baseSpeed_)
float Enemy::getBaseSpeed() const {
    return baseSpeed_;
}

// this function is called to temporarily slow the enemy down
// parameter bool value determines whether the speed of the enemy is slowed down or returned back to normal
// parameter value true slows down enemy and false returns the speed of the enemy back to normal
// returns nothing
void Enemy::changeSpeed(bool reduce) {
    if(reduce) {
        speed_ = baseSpeed_-1;
    }
    else {
        speed_ = baseSpeed_;
    }
}


