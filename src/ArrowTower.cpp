#include "ArrowTower.hpp"


// constructor which takes x and y coordinates as parameter when being created
ArrowTower::ArrowTower(int xPos, int yPos)
    : Tower("arrow", 30, xPos, yPos, 100, 3.0f), range_(3.0f), targetEnemy_(nullptr), shortCooldown_(0.0f), target_(1) {}

// returns the range (float range_) of the arrowtower
float ArrowTower::getRange() const {
    return range_;
}

// different upgrades for the arrowtower based on given input
// takes the upgrade type (std::string) as parameter
// returns bool (true if upgrade succeeded, else false)
bool ArrowTower::upgradeTower(std::string upgradeType) {
    bool retVal = false;
    if (upgradeType == "damage"){ //if damage is upgraded for the first time, it is doubled
        if (!isDamageUpgraded()) {
            doubleDamage();
            retVal = true;
        }
    }
    else if (upgradeType == "range"){ //if range is upgraded for the first time, it is increased by 1
        if (!isRangeUpgraded()) {
            range_ += 1;    
            upgradeRange();
            retVal = true;
        }
    }
    else if (upgradeType == "speed"){ //if speed is upgraded for the first time,, it is increased by 1
        if (!isSpeedUpgraded()) {
            increaseSpeed();
            retVal = true;
        }
    }
    return retVal;
}

// returns how to pick a target (int target_) for the arrowtower (1 = furthest enemy, 2 = lowest HP, 3 = highest HP, 4 = closest)
int ArrowTower::getTarget() const {
    return target_;
}

// changes the target of the arrowtower according to given parameter i
// returns nothing
void ArrowTower::changeTarget(int i) { //change target depending on user's choice (furthest, lowest HP, highest HP, closest)
    if(i == 1 || i == 2 || i == 3 || i == 4) {
        target_ = i;
    }
}

// starts a short cooldown when the tower tries to shoot (time until the shooting consequences are applied to enemy, which is equal to projectile's travel time given as parameter)
// returns nothing
void ArrowTower::startShortCooldown(float travelTime) {
    shortCooldown_ = travelTime;
}

// updates the short cooldown according to time since last frame (float deltaTime)
// returns nothing
void ArrowTower::updateShortCooldown(float deltaTime) {
    shortCooldown_ -= deltaTime;
    shortCooldown_ = std::max(shortCooldown_, 0.0f);
}

// sets a new target enemy for the arrowtower, according to parameter Enemy* enemy
// returns nothing
void ArrowTower::setTargetEnemy(Enemy* enemy) {
    targetEnemy_ = enemy;
}

// returns the target enemy (Enemy* targetEnemy_) of the arrowtower   
Enemy* ArrowTower::getTargetEnemy() const {
    return targetEnemy_;
}

// changes the target enemy of the tower to null pointer
// returns nothing
void ArrowTower::removeTargetEnemy() {
    targetEnemy_ = nullptr;
}

// returns the short cooldown time (float shortCooldown_) of the arrowtower
float ArrowTower::getShortCooldown() {
    return shortCooldown_;
}

