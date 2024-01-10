#ifndef FARM_DEFENCE_GRID_CLASS
#define FARM_DEFENCE_GRID_CLASS

#include "Cell.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"
#include "BombTower.hpp"
#include "YLaserTower.hpp"
#include "XLaserTower.hpp"
#include "ArrowTower.hpp"
#include "Projectile.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <tuple>
#include <algorithm>


class Cell; //forward declaration

/*
Grid is responsible for doing almost everything in our game. A grid knows all its cells, and therefore it knows
where towers, enemies, obstacles exist. It has functions that move the enemies, and functions for enemy shooting etc.
*/

class Grid {
 public:
      Grid(std::vector<std::vector<Cell*>> cells, const std::vector<std::tuple<int, int>> path);
      ~Grid();
      void deleteCells();
      int getWidth() const;
      const std::vector<std::vector<Cell*>>& getCells() const;
      bool isInsideGrid(int x, int y);
      void createEnemy(std::string type);
      void createTower(std::string type, int x, int y);
      void destroyTower(int x, int y);
      void towerShoot(int towerX, int towerY, float deltaTime, sf::RenderWindow& window, const std::vector<sf::Sound*>& soundVector);
      void createObstacle(std::string type, int x, int y);
      void obstacleDamage(int x, int y);
      void updateMoney(int amount);
      void updateEnemyPosition(Enemy* enemy);
      void updateGridState(float deltaTime, std::string enemyType, int lives);
      void updateArrowsState(float deltaTime);
      bool isLevelOver() const;
      void updateLevelOver();
      std::vector<Projectile*>& getArrows();
      int getMoney() const;
      int getLives() const;
      int getPoints() const;
      std::tuple<float, float, float> calculateProjectileTarget(float towerX, float towerY, Enemy* enemy, float arrowSpeed);
      int getEnemyAmount() const;

 private:
      std::vector<std::vector<Cell*>> cells_;
      std::vector<std::tuple<int, int>> path_;
      std::vector<Projectile*> arrows_; // all the current projectiles
      int enemyAmount_; // how many enemies have been spawned during the current level
      int money_;
      int lives_;
      int points_;
      int width_; // width of the grid (20 since grid is 20x20)
      bool levelOver_; // indicates if the current level is over or not
};


#endif
