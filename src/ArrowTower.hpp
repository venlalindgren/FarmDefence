#ifndef FARM_DEFENCE_ARROWTOWER_CLASS
#define FARM_DEFENCE_ARROWTOWER_CLASS

#include "Tower.hpp"
#include <string>

class Enemy; // forward declaration

/*
An arrowtower shoots one enemy inside its range. 
The target can be chosen by user (furthest, lowest HP, highest HP, closest)
Its range is a circle, calculated with pythagoras.
Default damage is 30, range 3.0f, speed 3.0f and price 100.
Default target is the furthest enemy.
Cooldowns are 0 when the arrowtower is created.
Upgrade prices: speed 40 money, damage 80 money, range (+1.0f) 40 money
*/

class ArrowTower : public Tower {
 public:
    ArrowTower(int xPos, int yPos);
    float getRange() const;
    bool upgradeTower(std::string upgradeType);
    void startShortCooldown(float travelTime);
    void updateShortCooldown(float deltaTime);
    void setTargetEnemy(Enemy* enemy);
    void removeTargetEnemy();
    int getTarget() const;
    void changeTarget(int i);
    Enemy* getTargetEnemy() const;
    float getShortCooldown();
    bool targetFound();

 private:
    float range_;
    float shortCooldown_; // time until shot projectile has reached the target
    Enemy* targetEnemy_;
    int target_; // default 1 (1 furthest enemy, 2 lowest HP, 3 highest HP, 4 closest)
};


#endif
