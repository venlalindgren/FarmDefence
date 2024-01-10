#ifndef FARM_DEFENCE_OBSTACLE_CLASS
#define FARM_DEFENCE_OBSTACLE_CLASS

#include <string>


/*
An obstacle can be placed on the path of our grid. Obstacles affect enemies when they are in the same cell as the obstacle. 
One obstacle only causes damage or slows down once per enemy. 
The obstacle can either be a poison obstacle that reduces hitpoints or a mud obstacle that temporarily slows the enemy down.
Stats:
Poison: price 80, deals 15 damage
Mud: price 40, reduces enemy's speed by 1
*/

class Obstacle {
 public:
      Obstacle(std::string type, int xPos, int yPos, int price);
      ~Obstacle();
      std::string getType();
      int getXPos() const;
      int getYPos() const;
      int getPrice() const;

 private:
      std::string type_;
      int xPos_;
      int yPos_;
      int price_;
};


#endif
