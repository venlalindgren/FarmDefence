#include "BombTower.hpp"


// constructor which takes x and y coordinates as parameter when being created
// default range is 2, which equals 5x5 nearest cells with the tower in the middle
BombTower::BombTower(int xPos, int yPos)
    : Tower("bomb", 40, xPos, yPos, 200, 5.0f), range_(2) {}

// returns the range (int range_) of the bombtower
int BombTower::getRange() {
    return range_;
}

// different upgrades based on given input
// takes the upgrade type (std::string) as parameter
// returns bool (true if upgrade succeeded, else false)
bool BombTower::upgradeTower(std::string upgradeType) {
    bool retVal = false; 
    if (upgradeType == "damage"){ //if damage is upgraded, it is doubled
        if (!isDamageUpgraded()) {
            doubleDamage();
            retVal = true;
        }
    }
    else if (upgradeType == "range"){ //if range is upgraded, it is increased by 1
        if (!isRangeUpgraded()) {
            range_ += 1;    
            upgradeRange();
            retVal = true;
        }
    }
    else if (upgradeType == "speed"){  //if speed is upgraded, it is increased by 1
        if (!isSpeedUpgraded()) {
            increaseSpeed();
            retVal = true;
        }
    }
    
    return retVal;
}