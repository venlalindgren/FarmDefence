#ifndef FARM_DEFENCE_ENEMY_CLASS
#define FARM_DEFENCE_ENEMY_CLASS

#include "ArrowTower.hpp"
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

/*
An enemy moves on a path on our grid. It can get hit and if it loses enough points, it dies.
Our grid understands an enemy to be in one cell, but the GUI shows its movement smoothly.
Therefore it has continuous possible x and y coordinate values.
The stats of the different enemies:
chicken: 60 hp, 30 money, speed 2.5, 50 points
pig: 100 hp, 50 money, speed 2.5, 75 points
cow: 160 hp, 75 money, speed 3, 100 points
wolf: 250 hp, 100 money, speed 3, 150 points
horse: 350 hp, 125 money, speed 3, 200 points
*/

class Enemy {
 public:
      Enemy(const std::string& type, float xPos, float yPos, int hitpoints, float speed, int pointValue, int moneyValue);
      ~Enemy();
      void updateHitpoints(int damage);
      void updatePosition(float deltaTime, int dir);
      void updateTarget(int prevX, int prevY, int newX, int newY); 
      float distanceToCurrentCell() const;
      float distanceToTargetCell() const;
      void addTower(ArrowTower* tower);
      void deleteTower(ArrowTower* towerToDelete);
      int getTargetX();
      int getTargetY();
      int getCellX() const;
      int getCellY() const;
      void newCellX(int x);
      void newCellY(int y);
      float getX() const;
      float getY() const;
      std::string getType() const;
      bool isAlive();
      int getHP();
      int getMaxHP();
      int getDirection();
      int getPointValue() const;
      int getMoneyValue() const;
      float getBaseSpeed() const;
      void changeSpeed(bool reduce);
      bool hasBeenPoisonDamaged();
      bool hasBeenMudSlowed();
      void poisonDamaged(bool state);
      void mudSlowed(bool state);

 private:
      std::string type_;
      float xPos_;
      float yPos_;
      int cellX_;
      int cellY_;
      int nextTargetX_;
      int nextTargetY_;
      float speed_;
      float baseSpeed_;
      int hitpoints_;
      int maxHP_;
      bool alive_;
      int direction_; //direction (1 right, 2 left, 3 down, 4 up)
      int pointValue_; //the amount of points the player gains when the enemy is killed
      int moneyValue_; // the amount of money the player gains when the enemy is killed   
      bool poisonDamaged_;
      bool mudSlowed_;
      std::vector<ArrowTower*> towersTargetingThisEnemy_; //list of towers currently targeting this enemy
};


#endif
