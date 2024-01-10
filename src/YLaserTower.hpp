#ifndef FARM_DEFENCE_YLASERTOWER_CLASS
#define FARM_DEFENCE_YLASERTOWER_CLASS

#include "Tower.hpp"
#include <string>

/*
An Ylasertower shoots the closest enemy that it can find in the same column. 
If two enemies are as close, it chooses the one which is below the tower.
Deals 100 damage to a single enemy, speed is 4.0f, price is 300 and the range is the whole column
Upgrade prices: speed 80 money, damage 200 money
*/

class YLaserTower : public Tower {
 public:
    YLaserTower(int xPos, int yPos);
    bool upgradeTower(std::string upgradeType);
};


#endif
