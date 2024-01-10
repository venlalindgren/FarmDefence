#include "XLaserTower.hpp"


// constructor, which takes x and y position as parameters
XLaserTower::XLaserTower(int xPos, int yPos)
    : Tower("laserX", 100, xPos, yPos, 300, 4.0f) {}


// performs different upgrades based on given input (laserTower doesn't have a range, so range cannot be upgraded)
// takes the upgrade type (std::string) as parameter and returns a bool value which is true if the upgrade succeeded, false otherwise
bool XLaserTower::upgradeTower(std::string upgradeType) {
    bool retVal = false;
    if (upgradeType == "damage") {
        if (!isDamageUpgraded()) { //if damage is upgraded, it is doubled
            doubleDamage();
            retVal = true;
        }
    }
    else if (upgradeType == "speed"){
        if (!isSpeedUpgraded()) { //if speed is upgraded, it is increased by 1
            increaseSpeed();
            retVal = true;
        }
    }
    return retVal;
}

