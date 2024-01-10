#ifndef FARM_DEFENCE_PROJECTILE_CLASS
#define FARM_DEFENCE_PROJECTILE_CLASS

#include "Enemy.hpp"
#include <string>
#include <cmath>
#include <iostream>

/*
In our game projectiles are arrows. They are created when an arrowtower shoots an enemy. They appear next to the tower,
and then travel to the target enemy.
*/

class Projectile {
   public:
      Projectile(const std::string type, float xPos, float yPos, float targetX, float targetY, float speed);
      ~Projectile();
      float getX() const;
      float getY() const;
      int updatePosition(float deltaTime);
      void setAngle();
      float getAngle();
      float getDistance();

   private:
      const std::string type_;
      float xPos_;
      float yPos_;
      float speed_;
      float targetX_;
      float targetY_;
      float angle_;
};


#endif
