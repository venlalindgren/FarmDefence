#include "Tower.hpp"


// constructor, which takes tower type, damage, x and y position, price and speed as parameters
// upgrade variables are initialized as false
Tower::Tower(std::string type, int damage, int xPos, int yPos, int price, float speed)
    : type_(type), damage_(damage), isReady_(true), xPos_(xPos), yPos_(yPos), speed_(speed), cooldown_(0.0f),
      price_(price), rangeUpgraded_(false), damageUpgraded_(false), speedUpgraded_(false) {}
    // lower number for speed means faster tower

// destructor
Tower::~Tower() {}

// returns the x coordinate of the tower as int
int Tower::getXPos() const {
    return xPos_;
}

// returns the y coordinate of the tower as int
int Tower::getYPos() const {
    return yPos_;
}

// returns the tower's damage as int
int Tower::getDamage() {
    return damage_;
}

// returns the price of the tower as int
int Tower::getPrice() {
    return price_;
}

// returns the type of the tower as std::string
std::string Tower::getType() {
    return type_;
}

// returns a bool value whether the tower is ready to shoot or not
// returns true if the tower is ready, false otherwise
bool Tower::readyToShoot() {
    return isReady_;
}

// The tower needs to cool down between each shot
// starts cooldown, which is equal to the speed of the tower, changes tower's isReady_ variable to false
// returns nothing
void Tower::startCooldown() {
    cooldown_ = speed_;
    isReady_ = false;
}

// decreases cooldow by time since last frame (float deltaTime) until it's zero
void Tower::updateCooldown(float deltaTime) {
    if(cooldown_ > 0) {
        cooldown_ -= deltaTime;
        cooldown_ = std::max(cooldown_, 0.0f);
        if(cooldown_ == 0.0f) {
            isReady_ = true; // assumption that always shortcooldown <= cooldown, otherwise problems could occur
        }
    }
}

// doubles the damage of the tower
// returns nothing
void Tower::doubleDamage(){
    damage_ = damage_ * 2;
    damageUpgraded_ = true;
}

// increases speed of the tower
// since smaller speed_ value (time between shots) equals faster speed, the function actually decreases the speed_ variable
// returns nothing
void Tower::increaseSpeed(){
    speed_ -= 1.0f;
    speedUpgraded_ = true;
}

// returns a bool value which is true if the tower's damage has been upgraded, false otherwise
bool Tower::isDamageUpgraded() {
    return damageUpgraded_;
}

// returns a bool value which is true if the tower's range has been upgraded, false otherwise
bool Tower::isRangeUpgraded() {
    return rangeUpgraded_;
}

// returns a bool value which is true if the tower's speed has been upgraded, false otherwise
bool Tower::isSpeedUpgraded() {
    return speedUpgraded_;
}

// changes the range upgraded status as true
// returns nothing
void Tower::upgradeRange() {
    rangeUpgraded_ = true;
}

