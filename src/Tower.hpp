#ifndef FARM_DEFENCE_TOWER_CLASS
#define FARM_DEFENCE_TOWER_CLASS

#include <string>

/*
This is a base class for towers. All the towers in our game extend from Tower. 
A tower knows its type, damage, x and y coordinates, build price, speed, upgrades and if it is ready to shoot or not
If a tower is destroyed, half of the original build price is given back as money
*/

class Tower {
   public:
      explicit Tower(std::string type, int damage, int xPos, int yPos, int price, float speed);
      virtual ~Tower(); 
      int getXPos() const;
      int getYPos() const;
      int getDamage();
      int getPrice();
      std::string getType();
      bool readyToShoot();
      void startCooldown();
      void updateCooldown(float deltaTime);
      virtual bool upgradeTower(std::string upgradeType) = 0;
      void doubleDamage();
      void increaseSpeed();
      bool isDamageUpgraded();
      bool isRangeUpgraded();
      bool isSpeedUpgraded();
      void upgradeRange();

   private:
      std::string type_;
      float speed_;
      bool isReady_;
      float cooldown_;
      int damage_;
      int xPos_;
      int yPos_;
      int price_;
      bool rangeUpgraded_;
      bool damageUpgraded_;
      bool speedUpgraded_;
};


#endif
