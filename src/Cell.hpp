#ifndef FARM_DEFENCE_CELL_CLASS
#define FARM_DEFENCE_CELL_CLASS

#include "Grid.hpp"
#include "Enemy.hpp"
#include "Obstacle.hpp"
#include "Tower.hpp"
#include <vector>
#include <string>

/*
A cell knows its own type and it's coordinates in the grid.
A cell can have an enemy, a tower or an obstacle in it.
By default, a cell is empty. 
*/

class Cell {
    public:
        Cell(std::string cellType, int xPos, int yPos);
        ~Cell();
        std::string getCellType() const;
        Enemy* getEnemy();
        void addEnemy(Enemy* enemy);
        void removeEnemy();
        float getX() const;
        float getY() const;
        Tower* getTower();
        void addTower(Tower* tower);
        void removeTower();
        Obstacle* getObstacle();
        void addObstacle(Obstacle* obstacle);
        void removeObstacle();
        bool isPath();
        bool isEmpty();

    private:
        Tower* tower_; //default null
        Obstacle* obstacle_; //default null
        Enemy* enemy_; //default null
        std::string cellType_;   //"land" or "path"
        float xPos_;
        float yPos_;
};


#endif
