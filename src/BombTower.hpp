#ifndef FARM_DEFENCE_BOMBTOWER_CLASS
#define FARM_DEFENCE_BOMBTOWER_CLASS

#include "Tower.hpp"
#include <string>


/*
A bombtower shoots every enemy inside its range.
Its range is a square of cells (by default 5x5 with the tower in the middle). 
Default damage is 40, speed 5.0f and price 200.
Cooldown is 0 when the bombtower is created.
Upgrade prices: speed 80 money, damage 150 money, range (+1 range = 6x6 adjacent tiles) 150 money
*/

class BombTower : public Tower {
 public:
   BombTower(int xPos, int yPos);
   int getRange();
   bool upgradeTower(std::string upgradeType);

 private:
   int range_;
};


#endif
