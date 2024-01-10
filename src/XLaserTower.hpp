#ifndef FARM_DEFENCE_XLASERTOWER_CLASS
#define FARM_DEFENCE_XLASERTOWER_CLASS

#include "Tower.hpp"
#include <string>

/*
A Xlasertower shoots the closest enemy that it can find in the same row. 
If two enemies are as close, it chooses the one on the right side of the tower.
Deals 100 damage to a single enemy when shoots, speed is 4.0f, price is 300 and the range is the whole row
Upgrade prices: speed 80 money, damage 200 money
*/

class XLaserTower : public Tower {
 public:
    XLaserTower(int xPos, int yPos);
    bool upgradeTower(std::string upgradeType); 
};


#endif
