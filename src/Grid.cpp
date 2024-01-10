#include "Grid.hpp"


// constructor, which takes matrix of cells as parameter and also the coordinates of path cells
// money is initialized to 300, lives to 5 and points to 0
Grid::Grid(std::vector<std::vector<Cell*>> cells, const std::vector<std::tuple<int, int>> path)
    : cells_(cells), path_(path), width_(20), enemyAmount_(0), money_(300),
      lives_(5), points_(0), levelOver_(false), arrows_() {}

// destructor
Grid::~Grid() {}

// frees memory when game ends
// for each cell: first, deletes enemy, obstacle or tower inside cell, then cell itself
// no parameters or return values
void Grid::deleteCells() {
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            Cell* cell = cells_[i][j];
            Enemy* enemy = cell->getEnemy();
            if (enemy != nullptr) {
                delete enemy;
                cell->removeEnemy();
            }
            Tower* tower = cell->getTower();
            if (tower != nullptr) {
                delete tower;
                cell->removeTower();
            }
            Obstacle* obstacle = cell->getObstacle();
            if (obstacle != nullptr) {
                delete obstacle;
                cell->removeObstacle();
            }
            delete cell;
        }
    }
}

// returns the width of the grid as int
int Grid::getWidth() const {
    return width_;
}

// returns the current amount of money as int
int Grid::getMoney() const {
    return money_;
}

// returns the current number of lives left as int
int Grid::getLives() const {
    return lives_;
}

// returns the current number of points as int
int Grid::getPoints() const {
    return points_;
}

// returns the number of enemies as int
int Grid::getEnemyAmount() const {
    return enemyAmount_;
}

// updates the amount of money according to the parameter int amount
// returns nothing
void Grid::updateMoney(int amount) {
    money_ -= amount;
}

// returns whether the current level is over or not
// the return bool value is true if the level is over and false otherwise
bool Grid::isLevelOver() const {
    return levelOver_;
}

// changes the state of bool levelOver_ to false
// returns nothing
void Grid::updateLevelOver() {
    levelOver_ = false;
}

// returns all the cells of the grid as std::vector<std::vector<Cell*>>&
const std::vector<std::vector<Cell*>>& Grid::getCells() const{
    return cells_;
}

// returns all current projectiles as std::vector<Projectile*>&
std::vector<Projectile*>& Grid::getArrows() {
    return arrows_;
}


// calculates the optimal collision point for enemy and projectile
// takes tower's x and y coordinates, target enemy, and arrow's speed as parameters
// returns a std::tuple<float, float, float>, where the first two values indicate the best target coordinates for the projectile and the last value is the projectile's travel time to the target
std::tuple<float, float, float> Grid::calculateProjectileTarget(float towerX, float towerY, Enemy* enemy, float arrowSpeed) {
    float enemySpeed = enemy->getBaseSpeed();
    float enemyMovementTimeSeconds = 0.0f; // how long it takes for enemy to reach the certain coordinates
    float enemyX = enemy->getX();
    float enemyY = enemy->getY();
    float bestTimeDifference = 1000.0f;
    std::tuple<int, int> enemyTarget = std::make_tuple(enemy->getTargetX(), enemy->getTargetY());
    int dir = enemy->getDirection();
    std::tuple<float, float> currentTestCoordinates = std::make_tuple(enemyX, enemyY);
    std::tuple<float, float> bestTargetCoordinates = currentTestCoordinates;
    bool originalTarget = true; // are we still searching the optimal collision point between enemy and enemy's target coordinates
    bool secondTarget = true; // are we still searching the optimal collision point between enemy's target coordinates and the coordinates of the next path cell
    std::tuple<int, int> nextTargetCellCoordinates;
    std::tuple<int, int> nextTargetCellCoordinates2;
    // the loop finds the coordinates for next two cells on the path after enemy's target cell
    auto it = std::find(path_.begin(), path_.end(), enemyTarget);
    if (it != path_.end()) {
        it++;
        if (it != path_.end()) {
            nextTargetCellCoordinates = *it;
            it++;
            if (it != path_.end()) {
            nextTargetCellCoordinates2 = *it;
            }
            else {
                nextTargetCellCoordinates = std::make_tuple(20, 11);
            }
        }
        else {
            nextTargetCellCoordinates = std::make_tuple(20, 11);
            nextTargetCellCoordinates2 = std::make_tuple(21, 11);
        }
    }
    else {
        nextTargetCellCoordinates = std::make_tuple(21, 11);
        nextTargetCellCoordinates = std::make_tuple(22, 11);
    }

    // the direction of enemy after it has reached the current target coordinates
    int nextDir;
    if(std::get<0>(nextTargetCellCoordinates) > enemy->getTargetX()) {
        nextDir = 1;
    }
    else if(std::get<0>(nextTargetCellCoordinates) < enemy->getTargetX()) {
        nextDir = 2;
    }
    else if(std::get<1>(nextTargetCellCoordinates) > enemy->getTargetY()) {
        nextDir = 3;
    }
    else if(std::get<1>(nextTargetCellCoordinates) < enemy->getTargetY()) {
        nextDir = 4;
    }

    // the direction of enemy after it has reached the cell coordinates after current target
    int nextDir2;
    if(std::get<0>(nextTargetCellCoordinates2) > std::get<0>(nextTargetCellCoordinates)) {
        nextDir2 = 1;
    }
    else if(std::get<0>(nextTargetCellCoordinates2) < std::get<0>(nextTargetCellCoordinates)) {
        nextDir2 = 2;
    }
    else if(std::get<1>(nextTargetCellCoordinates2) > std::get<1>(nextTargetCellCoordinates)) {
        nextDir2 = 3;
    }
    else if(std::get<1>(nextTargetCellCoordinates2) < std::get<1>(nextTargetCellCoordinates)) {
        nextDir2 = 4;
    }

    float distanceTravelled = 0.0f; // to keep track if enemy would move at normal speed
    float distanceTravelledInMud = 0.0f; // to keep track if enemy would move in mud
    float enemySpeedInMud = enemySpeed-1.0f;

    // search for the coordinates of optimal collision point between enemy and the projectile with a max bias of 0.05
    // limited enemy's movement distance to max 1.3 because more is not needed in the testing
    int count = 0; // to keep track which cell is the test target
    while(distanceTravelled+distanceTravelledInMud < 1.3f) {
        enemyMovementTimeSeconds = (distanceTravelled/enemySpeed)*1.0f + (distanceTravelledInMud/enemySpeedInMud)*1.0f;
        float distX = std::get<0>(currentTestCoordinates) - towerX;
        float distY = std::get<1>(currentTestCoordinates) - towerY;
        float distanceToTarget = std::sqrt(distX*distX+distY*distY);
        float projectileMovementTimeSeconds = distanceToTarget/arrowSpeed;
        float timeDifference = std::abs(enemyMovementTimeSeconds-projectileMovementTimeSeconds);
        if(timeDifference < bestTimeDifference) {
            bestTimeDifference = timeDifference;
            bestTargetCoordinates = currentTestCoordinates;
        }
        bool mud = false;
        int x;
        int y;
        int curx;
        int cury;
        int testDir;
        // separate if clauses depending on the hypothetical target
        if(count == 0) { // if test target is the enemy's target
            x = enemy->getTargetX();
            y = enemy->getTargetY();
            curx = enemy->getCellX();
            cury = enemy->getCellY();
            testDir = dir;
        }
        else if(count == 1) { // if test target is the next cell after enemy's target
            x = std::get<0>(nextTargetCellCoordinates);
            y = std::get<1>(nextTargetCellCoordinates);
            curx = enemy->getTargetX();
            cury = enemy->getTargetY();
            testDir = nextDir;
        }
        else if(count == 2) { // if test target is two cells after enemy's target
            x = std::get<0>(nextTargetCellCoordinates2);
            y = std::get<1>(nextTargetCellCoordinates2);
            curx = std::get<0>(nextTargetCellCoordinates);
            cury = std::get<1>(nextTargetCellCoordinates);
            testDir = nextDir2;
        }
        // separate if clauses depending on the hypothetical enemy's direction
        if(testDir == 1) {
            // mud or not
            if((x <= 20) && (std::get<0>(currentTestCoordinates) < (x-0.5f))) {
                if(getCells()[curx][cury]->getObstacle() != nullptr && (getCells()[curx][cury]->getObstacle()->getType() == "mud")) {
                    mud = true;
                }
            }
            else if(x < 20) {
                if(getCells()[x][y]->getObstacle() != nullptr && (getCells()[x][y]->getObstacle()->getType() == "mud")) {
                    mud = true;
                }
            }
            // if current test coordinates would go past the target cell's coordinates, force the test coordinates to target cell coordinates
            if(std::get<0>(currentTestCoordinates)+0.1f >= x) {
                // the loop should increase distanceTravelled only so (max 1.3f) that the loop never ends up here when the target is two cells after enemy's target
                // otherwise addition to distanceTravelled could be zero and would require a new cell to look for
                if(mud) {distanceTravelledInMud += x-std::get<0>(currentTestCoordinates);}
                else {distanceTravelled += x-std::get<0>(currentTestCoordinates);}
                currentTestCoordinates = std::make_tuple(x, y);
                count += 1;
            }
            // if the cell would be changed for next test coordinates, force the test coordinates to the edge of the two cells
            else if(std::get<0>(currentTestCoordinates) < x-0.5f && std::get<0>(currentTestCoordinates)+0.1f >= x-0.5f) {
                if(mud) {distanceTravelledInMud += (x-0.5f)-std::get<0>(currentTestCoordinates);}
                else {distanceTravelled += (x-0.5f)-std::get<0>(currentTestCoordinates);}
                currentTestCoordinates = std::make_tuple(x-0.5f, y);
            }
            // increase test coordinates on the path by 0.1
            else {
                if(mud) {distanceTravelledInMud += 0.1f;}
                else {distanceTravelled += 0.1f;}
                currentTestCoordinates = std::make_tuple(std::get<0>(currentTestCoordinates)+0.1f, y);
            }
        }
        else if(testDir == 2) {
            if((x < 20) && (std::get<0>(currentTestCoordinates) > (x+0.5f))) {
                if(getCells()[curx][cury]->getObstacle() != nullptr && (getCells()[curx][cury]->getObstacle()->getType() == "mud")) {
                    mud = true;
                }
            }
            else if(x < 20) {
                if(getCells()[x][y]->getObstacle() != nullptr && (getCells()[x][y]->getObstacle()->getType() == "mud")) {
                    mud = true;
                }
            }
            if(std::get<0>(currentTestCoordinates)-0.1f <= x) {
                if(mud) {distanceTravelledInMud += std::get<0>(currentTestCoordinates)-x;}
                else {distanceTravelled += std::get<0>(currentTestCoordinates)-x;}
                currentTestCoordinates = std::make_tuple(x, y);
                count += 1;
            }
            else if(std::get<0>(currentTestCoordinates) > x+0.5f && std::get<0>(currentTestCoordinates)-0.1f <= x+0.5f) {
                if(mud) {distanceTravelledInMud += std::get<0>(currentTestCoordinates)-(x+0.5f);}
                else {distanceTravelled += std::get<0>(currentTestCoordinates)-(x+0.5f);}
                currentTestCoordinates = std::make_tuple(x+0.5f, y);
            }
            else {
                if(mud) {distanceTravelledInMud += 0.1f;}
                else {distanceTravelled += 0.1f;}
                currentTestCoordinates = std::make_tuple(std::get<0>(currentTestCoordinates)-0.1f, y);
            }
        }
        else if(testDir == 3) {
            if((x < 20) && (std::get<1>(currentTestCoordinates) < (y-0.5f))) {
                if(getCells()[curx][cury]->getObstacle() != nullptr && (getCells()[curx][cury]->getObstacle()->getType() == "mud")) {
                    mud = true;
                }
            }
            else if(x < 20) {
                if(getCells()[x][y]->getObstacle() != nullptr && (getCells()[x][y]->getObstacle()->getType() == "mud")) {
                    mud = true;
                }
            }
            if(std::get<1>(currentTestCoordinates)+0.1f >= y) {
                if(mud) {distanceTravelledInMud += y-std::get<1>(currentTestCoordinates);}
                else {distanceTravelled += y-std::get<1>(currentTestCoordinates);}
                currentTestCoordinates = std::make_tuple(x, y);
                count += 1;
            }
            else if(std::get<1>(currentTestCoordinates) < y-0.5f && std::get<1>(currentTestCoordinates)+0.1f >= y-0.5f) {
                if(mud) {distanceTravelledInMud += (y-0.5f)-std::get<1>(currentTestCoordinates);}
                else {distanceTravelled += (y-0.5f)-std::get<1>(currentTestCoordinates);}
                currentTestCoordinates = std::make_tuple(x, y-0.5f);
            }
            else {
                if(mud) {distanceTravelledInMud += 0.1f;}
                else {distanceTravelled += 0.1f;}
                currentTestCoordinates = std::make_tuple(x, std::get<1>(currentTestCoordinates)+0.1f);
            }
        }
        else if(testDir == 4) {
            if((x < 20) && (std::get<1>(currentTestCoordinates) > (y+0.5f))) {
                if(getCells()[curx][cury]->getObstacle() != nullptr && (getCells()[curx][cury]->getObstacle()->getType() == "mud")) {
                    mud = true;
                }
            }
            else if(x < 20) {
                if(getCells()[x][y]->getObstacle() != nullptr && (getCells()[x][y]->getObstacle()->getType() == "mud")) {
                    mud = true;
                }
            }
            if(std::get<1>(currentTestCoordinates)-0.1f <= y) {
                if(mud) {distanceTravelledInMud += std::get<1>(currentTestCoordinates)-y;}
                else {distanceTravelled += std::get<1>(currentTestCoordinates)-y;}
                currentTestCoordinates = std::make_tuple(x, y);
                count += 1;
            }
            else if(std::get<1>(currentTestCoordinates) > y+0.5f && std::get<1>(currentTestCoordinates)-0.1f <= y+0.5f) {
                if(mud) {distanceTravelledInMud += std::get<1>(currentTestCoordinates)-(y+0.5f);}
                else {distanceTravelled += std::get<1>(currentTestCoordinates)-(y+0.5f);}
                currentTestCoordinates = std::make_tuple(x, y+0.5f);
            }
            else {
                if(mud) {distanceTravelledInMud += 0.1f;}
                else {distanceTravelled += 0.1f;}
                currentTestCoordinates = std::make_tuple(x, std::get<1>(currentTestCoordinates)-0.1f);
            }
        }
    }
    float finalDistX = std::get<0>(bestTargetCoordinates) - towerX;
    float finalDistY = std::get<1>(bestTargetCoordinates) - towerY;
    float finalDistanceToTarget = std::sqrt(finalDistX*finalDistX+finalDistY*finalDistY);
    float finalProjectileMovementTimeSeconds = finalDistanceToTarget/arrowSpeed;
    std::tuple<float, float, float> retVal = std::tuple_cat(bestTargetCoordinates, std::make_tuple(finalProjectileMovementTimeSeconds));
    return retVal;
}


// check whether a clicked spot in the window is inside the grid
// takes the x and y coordinates of the clicked spot as parameters
// returns a bool value which is true if the clicked spot is inside the grid, false otherwise
bool Grid::isInsideGrid(int x, int y) {
    if ((x >= 0) && (x < width_)) {
        if ((y >= 0) && (y < width_)) {
            return true;
        }
        else { 
            return false; 
        }
    }
    else {
        return false;
    }
}  


// creates an enemy and adds it to the first cell of the path
// takes the enemy type as parameter (std::string)
// returns nothing
void Grid::createEnemy(std::string type) { 
    std::tuple<int, int> coordinateTuple = path_[0];
    Cell* firstCell = getCells()[std::get<0>(coordinateTuple)][std::get<1>(coordinateTuple)];
    Enemy* enemy;
    if(type == "chicken") {
        enemy = new Enemy(type, firstCell->getX()-1.0f, firstCell->getY(), 60, 2.5, 50, 30);
    }
    else if(type == "pig") {
        enemy = new Enemy(type, firstCell->getX()-1.0f, firstCell->getY(), 100, 2.5, 75, 50);
    }
    else if(type == "cow") {
        enemy = new Enemy(type, firstCell->getX()-1.0f, firstCell->getY(), 160, 3, 100, 75);
    }
    else if(type == "wolf") {
        enemy = new Enemy(type, firstCell->getX()-1.0f, firstCell->getY(), 250, 3, 150, 100);
    }
    else if(type == "horse") {
        enemy = new Enemy(type, firstCell->getX()-1.0f, firstCell->getY(), 350, 3, 200, 125);
    }
    cells_[firstCell->getX()][firstCell->getY()]->addEnemy(enemy);
    enemyAmount_ += 1;
}

// updates enemy's position in the grid
// takes the enemy to be moved as parameter (Enemy* enemy)
// returns nothing
void Grid::updateEnemyPosition(Enemy* enemy) {
    if (enemy != nullptr) {
        int currentX = static_cast<int>(enemy->getX());
        int currentY = static_cast<int>(enemy->getY());
        cells_[enemy->getCellX()][enemy->getCellY()]->removeEnemy();
        cells_[enemy->getTargetX()][enemy->getTargetY()]->addEnemy(enemy);
        enemy->newCellX(enemy->getTargetX());
        enemy->newCellY(enemy->getTargetY());
        //reset these variables:
        enemy->mudSlowed(false);
        enemy->poisonDamaged(false);
        enemy->changeSpeed(false);
    }
}

// creates a new obstacle to the grid
// takes the obstacle type as parameter (std::string) and also the x and y coordinates where the obstacle will be built
// returns nothing
void Grid::createObstacle(std::string type, int x, int y) {
    Obstacle* obstacle;
    if(type == "mud") {
        if(money_ >= 40) {
            obstacle = new Obstacle(type, x, y, 40);
            getCells()[x][y]->addObstacle(obstacle);
            money_ -= obstacle->getPrice();
        }
    }
    else if(type == "poison") {
        if(money_ >= 80) {
            obstacle = new Obstacle(type, x, y, 80);
            getCells()[x][y]->addObstacle(obstacle);
            money_ -= obstacle->getPrice();
        }
    }

}

// Used to damage or slow down enemies, when they are in the same cell. The effects can only be applied once per enemy from a single obstacle.
// If the obstacle type is mud, it slows down the enemy
// If the obstacle type is poison, it causes damage to the enemy
// If the enemy dies due to poison damage, the enemy is deleted
// takes as parameters the x and y coordinates of the cell where the obstacle and the enemy is
// returns nothing
void Grid::obstacleDamage(int x, int y) {
    Cell* cell = getCells()[x][y];
    Obstacle* obstacle = cell->getObstacle();
    Enemy* enemy = cell->getEnemy();

    if (enemy!= nullptr && obstacle != nullptr) {
        if (!enemy->hasBeenPoisonDamaged() && !enemy->hasBeenMudSlowed()) {
            if (obstacle->getType() == "poison") {
                enemy->updateHitpoints(20);
                enemy->poisonDamaged(true);
            }
            else if(obstacle->getType() == "mud") {
                enemy->changeSpeed(true);
                enemy->mudSlowed(true);
            }
            if (!enemy->isAlive()) { //if the enemy gas died, increase points and money and remove it from the cell
                points_ += enemy->getPointValue();
                money_ += enemy->getMoneyValue();
                delete enemy;
                cell->removeEnemy();
            }
        }
    }
}

// creates a tower to the desired cell
// takes as parameters the type of the tower (std::string) and the x and y coordinates where the tower will be built
// returns nothing
void Grid::createTower(std::string type, int x, int y) {
    if (type == "bomb") {
        Tower* tower = new BombTower(x, y);
        if (money_ >= tower->getPrice()) {
            getCells()[x][y]->addTower(tower);
            money_ -= tower->getPrice();
        }
    }
    else if (type == "laserY") {
        Tower* tower = new YLaserTower(x, y);
        if (money_ >= tower->getPrice()) {
            getCells()[x][y]->addTower(tower);
            money_ -= tower->getPrice();
        }
    }
    else if (type == "laserX") {
        Tower* tower = new XLaserTower(x, y);
        if (money_ >= tower->getPrice()) {
            getCells()[x][y]->addTower(tower);
            money_ -= tower->getPrice();
        }
    }
    else if (type == "arrow") {
        Tower* tower = new ArrowTower(x, y);
        if (money_ >= tower->getPrice()) {
            getCells()[x][y]->addTower(tower);
            money_ -= tower->getPrice();
        }
    }
}

// destroys a tower
// takes as parameters the x and y coordinates of the tower
// returns nothing
void Grid::destroyTower(int x, int y) {
    Cell* cell = getCells()[x][y];
    Tower* tower = cell->getTower();
    money_ += tower->getPrice() * 0.5;
    if (tower != nullptr) {
        delete tower;
        cell->removeTower();
    }
}


// Shoots enemy/enemies found inside the range of the tower. Different towers shoot in different ways. 
// takes the tower's coordinates, time since last frame, game window and vector of sounds as parameters
// returns nothing
void Grid::towerShoot(int towerX, int towerY, float deltaTime, sf::RenderWindow& window, const std::vector<sf::Sound*>& soundVector) {
    Cell* cell = getCells()[towerX][towerY]; 
    Tower* tower = cell->getTower();  //get the correct tower by the coordinates

    if (tower != nullptr) {   
        //A bombTower shoots every enemy inside its range. Its range is a square of cells. 
        if (tower->getType() == "bomb") {
            bool enemyShot = false;
            BombTower* bombtower = dynamic_cast<BombTower*>(tower);
            if (bombtower != nullptr) {
                if (bombtower->readyToShoot()) { //if the tower is ready to shoot, find enemies inside the square range and shoot them
                    for (int i = towerX - bombtower->getRange(); i <= towerX + bombtower->getRange(); i++) { //look for enemies from 'range' number of squares to the left and right from tower
                        for (int j = towerY - bombtower->getRange(); j <= towerY + bombtower->getRange(); j++) { //look for enemies from 'range' number of squares up and down from tower
                            if (isInsideGrid(i, j)) {    
                                Cell* cell = getCells()[i][j];
                                Enemy* enemy = cell->getEnemy();
                                if (enemy != nullptr) { //if an enemy is found
                                    sf::RectangleShape explosion; //color(very shortly) the cell where the enemy is shot in red
                                    explosion.setFillColor(sf::Color(255, 99, 71, 255));
                                    explosion.setSize(sf::Vector2f(40.0f, 40.0f));
                                    explosion.setPosition(enemy->getCellX()*40, enemy->getCellY()*40);
                                    window.draw(explosion);
                                    soundVector[1]->play(); //play bomb sound

                                    enemy->updateHitpoints(bombtower->getDamage()); //update the hitpoints of the enemy
                                    enemyShot = true; //indicate that the tower has shot an enemy
                                    if (!enemy->isAlive()) {
                                        points_ += enemy->getPointValue();
                                        money_ += enemy->getMoneyValue();
                                        delete enemy;
                                        getCells()[i][j]->removeEnemy();
                                    }

                                }
                            }
                        } 
                    }
                    if(enemyShot) {  //start cooldown after enemy is shot
                        bombtower->startCooldown();
                    }
                }
                else {
                    bombtower->updateCooldown(deltaTime);
                }
                
            }
        }
        //A laserYtower shoots the closest enemy that it can find in the same column. If two enemies are as close, it chooses the one below the tower.
        else if (tower->getType() == "laserY") {
            bool enemyShot = false;
            std::vector<std::tuple<Enemy*, int>> foundEnemies; //enemies are added to this vector in order from closest to furthest

            YLaserTower* lasertowerY = dynamic_cast<YLaserTower*>(tower);
            if (lasertowerY != nullptr) {
                if (lasertowerY->readyToShoot()) { //if the tower is ready to shoot
                    for (int j = 1 ; j < width_ ; j++) { //check for enemies from the whole column
                            if (isInsideGrid(towerX, towerY+j)) {//+1 cell below the tower
                                Enemy* enemy = getCells()[towerX][towerY+j]->getEnemy(); 
                                if (enemy != nullptr) {
                                    foundEnemies.push_back(std::make_tuple(enemy,towerY+j));
                                    }
                                }
                            if (isInsideGrid(towerX, towerY-j)) {//+1 cell above the tower
                                Enemy* enemy = getCells()[towerX][towerY-j]->getEnemy();
                                if (enemy != nullptr) {
                                    foundEnemies.push_back(std::make_tuple(enemy,towerY-j));
                                }
                            }
                    }
                    //if the tower found some enemies, it shoots the closest one
                    if(foundEnemies.size() != 0){
                            Enemy* enemy = std::get<0>(foundEnemies[0]);
                            int y = std::get<1>(foundEnemies[0]);
                            int dist = enemy->getCellY()-towerY;
                            
                            //(very quickly) draw a 'laser' from tower to enemy
                            sf::RectangleShape laser;
                            laser.setFillColor(sf::Color(64, 224, 208, 235));
                            if (dist < 0) {  //the direction and size of the laser block depend on the distance
                            laser.setSize(sf::Vector2f(40.0f, (-1)*dist*40.0f));
                            laser.setPosition(enemy->getCellX()*40, enemy->getCellY() * 40);
                            }
                            else {
                            laser.setSize(sf::Vector2f(40.0f, dist*40.0f));
                            laser.setPosition(towerX*40, (towerY+1)*40);
                            }
                            window.draw(laser);
                            soundVector[2]->play(); //play laser sound

                            enemy->updateHitpoints(lasertowerY->getDamage());
                            enemyShot = true;
                            if (!enemy->isAlive()) {
                                points_ += enemy->getPointValue();
                                money_ += enemy->getMoneyValue();
                                delete enemy;
                                getCells()[towerX][y]->removeEnemy();
                            }
                    }

                    if(enemyShot) {
                        lasertowerY->startCooldown();
                    }
                }
                lasertowerY->updateCooldown(deltaTime);
            }
        }

        //A laserXtower shoots the closest enemy that it can find in the same row. If two enemies are as close, it chooses the one right from the tower.
        else if (tower->getType() == "laserX") {
            bool enemyShot = false;
            std::vector<std::tuple<Enemy*, int>> foundEnemies; //enemies are added to this vector in order from closest to furthest

            XLaserTower* lasertowerX = dynamic_cast<XLaserTower*>(tower);
            if (lasertowerX != nullptr) {
                if (lasertowerX->readyToShoot()) {
                    for (int j = 1 ; j < width_ ; j++) { //check for enemies from the whole row
                            if (isInsideGrid(towerX+j, towerY)) { //+1 cell right from the tower
                                Enemy* enemy = getCells()[towerX+j][towerY]->getEnemy();
                                if (enemy != nullptr) {
                                    foundEnemies.push_back(std::make_tuple(enemy,towerX+j));
                                    }
                                }
                            if (isInsideGrid(towerX-j, towerY)) { //+1 cell left from the tower
                                Enemy* enemy = getCells()[towerX-j][towerY]->getEnemy();
                                if (enemy != nullptr) {
                                    foundEnemies.push_back(std::make_tuple(enemy,towerX-j));
                                    }
                                }
                    }

                    if(foundEnemies.size() != 0){ //if the tower found some enemies, it shoots the closest one
                            Enemy* enemy = std::get<0>(foundEnemies[0]);
                            int x = std::get<1>(foundEnemies[0]);
                            int dist = enemy->getCellX()-towerX;

                             //(very quickly) draw a 'laser' from tower to enemy
                            sf::RectangleShape laser;
                            laser.setFillColor(sf::Color(255, 0, 255, 200));
                            if (dist < 0) { //the direction and size of the laser block depend on the distance
                            laser.setSize(sf::Vector2f((-1)*dist*40.0f, 40.0f));
                            laser.setPosition(enemy->getCellX()*40, enemy->getCellY() * 40);
                            }
                            else {
                            laser.setSize(sf::Vector2f(dist*40.0f, 40.0f));
                            laser.setPosition((towerX+1)*40, towerY*40);
                            }
                            window.draw(laser);
                            soundVector[2]->play(); //play laser sound

                            enemy->updateHitpoints(lasertowerX->getDamage());
                            enemyShot = true;
                            if (!enemy->isAlive()) {
                                points_ += enemy->getPointValue();
                                money_ += enemy->getMoneyValue();
                                delete enemy;
                                getCells()[x][towerY]->removeEnemy();
                            }
                    }
                    if(enemyShot) {
                        lasertowerX->startCooldown();
                    }
                }
                lasertowerX->updateCooldown(deltaTime);
            }
        }


        if (tower->getType() == "arrow") {  //shoots enemy (inside range) based on the chosen target (furthest, weakest, strongest, closest)
            bool enemyShot = false;
            std::vector<Enemy*> foundEnemies;
            ArrowTower* arrowtower = dynamic_cast<ArrowTower*>(tower);
            Cell* currentCell;
            Enemy* enemy;
            int i = path_.size() - 1;
            if (arrowtower != nullptr) {
                if (arrowtower->readyToShoot()) {
                    if (arrowtower->getTargetEnemy() == nullptr) {
                        // look for all the enemies which are inside the range of the arrowtower (range is a circle measured with pythagoras)
                        while(i >= 0) {
                            std::tuple<int, int> coordinateTuple = path_[i];
                            currentCell = getCells()[std::get<0>(coordinateTuple)][std::get<1>(coordinateTuple)];
                            enemy = currentCell->getEnemy();
                            if (enemy != nullptr) {
                                float dx = enemy->getX() - towerX;
                                    float dy = enemy->getY() - towerY;
                                    float pythagora = std::sqrt((dx*dx + dy*dy));
                                    if(pythagora <= arrowtower->getRange()) {
                                        foundEnemies.push_back(enemy);
                                    }
                            }
                            i--;
                        }
                        if (foundEnemies.size() != 0) {

                            int j = arrowtower->getTarget();
                            Enemy* targetEnemy = nullptr;

                            //shoots the furthest enemy
                            if(j == 1) {
                                bool enemyFound = false;
                                for (auto pathCoordinates = path_.rbegin(); pathCoordinates != path_.rend(); pathCoordinates++) {
                                    if(!enemyFound) {
                                        int pathX, pathY;
                                        std::tie(pathX, pathY) = *pathCoordinates;
                                        for (const auto& enemy : foundEnemies) {
                                            if(enemy->getCellX() == pathX && enemy->getCellY() == pathY) {
                                                targetEnemy = enemy;
                                                targetEnemy->addTower(arrowtower);
                                                enemyFound = true;
                                            }
                                        }
                                    }
                                }
                            }

                            // shoots the weakest enemy
                            else if(j == 2) {
                                auto minHPEnemyIterator = std::min_element(foundEnemies.begin(), foundEnemies.end(),
                                    [](Enemy* enemy1, Enemy* enemy2) {
                                        return enemy1->getHP() < enemy2->getHP();
                                    }
                                );
                                if (minHPEnemyIterator != foundEnemies.end()) {
                                    targetEnemy = *minHPEnemyIterator;
                                    targetEnemy->addTower(arrowtower);
                                }
                            }

                            // shoots the strongest enemy
                            else if(j == 3) {
                                auto maxHPEnemyIterator = std::max_element(foundEnemies.begin(), foundEnemies.end(),
                                    [](Enemy* enemy1, Enemy* enemy2) {
                                        return enemy1->getHP() < enemy2->getHP();
                                    }
                                );
                                if (maxHPEnemyIterator != foundEnemies.end()) {
                                    targetEnemy = *maxHPEnemyIterator;
                                    targetEnemy->addTower(arrowtower);
                                }
                            }

                            // shoots the closes enemy
                            else if(j == 4) {
                                float shortestDistance = 1000.0f;
                                for (const auto& singleEnemy : foundEnemies) {
                                    float dx = singleEnemy->getX() - towerX;
                                    float dy = singleEnemy->getY() - towerY;
                                    float pythagora = std::sqrt((dx*dx + dy*dy));
                                    if(pythagora <= shortestDistance) {
                                        shortestDistance = pythagora;
                                        targetEnemy = singleEnemy;
                                    }
                                }
                                targetEnemy->addTower(arrowtower);
                            }
                            // create a projectile which travels from the tower to the target enemy
                            Projectile* arrow;
                            float projectileSpeed = 12.0f;
                            std::tuple<float, float, float> targetTuple = calculateProjectileTarget(towerX, towerY, targetEnemy, projectileSpeed);
                            float targX, targY, travelTime;
                            std::tie(targX, targY, travelTime) = targetTuple;
                            arrow = new Projectile("arrow", towerX, towerY, targX, targY, projectileSpeed);
                            arrow->setAngle();
                            arrows_.push_back(arrow);
                            arrowtower->startShortCooldown(travelTime);// start cooldown until the effects of the towershoot are applied to enemy (same as projectile's travel time)
                            arrowtower->startCooldown(); // cooldown until the tower is able to shoot again
                            soundVector[0]->play();
                            arrowtower->setTargetEnemy(targetEnemy); //target found to true

                        }
                    } 
                }
                else {
                    // update cooldowns
                    arrowtower->updateCooldown(deltaTime);
                    if(arrowtower->getShortCooldown() > 0.0f) {
                        arrowtower->updateShortCooldown(deltaTime);
                    }
                    // the effects of towershoot can be applied to enemy
                    if(arrowtower->getShortCooldown() == 0.0f && arrowtower->getTargetEnemy() != nullptr) {
                        Enemy* enemy = arrowtower->getTargetEnemy();
                        enemy->updateHitpoints(arrowtower->getDamage());
                        enemy->deleteTower(arrowtower);
                        arrowtower->removeTargetEnemy();
                        enemyShot = true;
                        if (!enemy->isAlive()) {
                            points_ += enemy->getPointValue();
                            money_ += enemy->getMoneyValue();
                            getCells()[enemy->getCellX()][enemy->getCellY()]->removeEnemy();
                            delete enemy;
                        }
                    }
                }
            }
        }
    }
}

// moves all current projectiles and deletes them if the target is reached
// takes as parameter the time elapsed since last frame (float deltaTime)
// returns nothing
void Grid::updateArrowsState(float deltaTime) {
    if(!arrows_.empty()) {
        for (auto it = arrows_.begin(); it != arrows_.end();) {
            int value = (*it)->updatePosition(deltaTime);
            if (value == 0) {
                delete (*it);
                it = arrows_.erase(it);
            }
            else {
                it++;
            }
        }
    }
}

// This function moves the enemies on the path and checks if the current level is over or not
// takes the time elapsed since last frame (float deltaTime), enemy type (std::string enemyType) and current number of lives (int) as parameters
// returns nothing
void Grid::updateGridState(float deltaTime, std::string enemyType, int lives) {
    int i = path_.size()-1;
    Enemy* enemy;
    Cell* currentCell;
    bool enemyFound = false;
    // moves all the enemies on the path
    while(i >= 0) {
        std::tuple<int, int> coordinateTuple = path_[i];
        currentCell = getCells()[std::get<0>(coordinateTuple)][std::get<1>(coordinateTuple)];
        enemy = currentCell->getEnemy();
        if (enemy != nullptr && enemy->isAlive()) {
            enemyFound = true;
            size_t targetIndex = i+1;
            int dir = enemy->getDirection();
            // enemy is in the last cell of the path
            if (targetIndex == path_.size()) {
                if((enemy->getX() == enemy->getTargetX()) && (enemy->getTargetX() != 20)) {
                    enemy->updateTarget(enemy->getX(), enemy->getY(), 20, 11);
                }
                enemy->updatePosition(deltaTime, dir);
                if(enemy->getX() == 20.0f) { // enemy got through the path
                    currentCell->removeEnemy();
                    delete enemy;
                    lives_ -= 1;
                    if (lives_ == 0) {
                        i = -1; //exit the loop
                    }
                }
            }
            else {
                if(enemy->distanceToTargetCell() < enemy->distanceToCurrentCell()) {
                    updateEnemyPosition(enemy);
                }
                // if moving horizontally
                if(dir == 1 || dir == 2) {
                    if(enemy->getX() == enemy->getTargetX()) {
                        std::tuple<int, int> coordinateTuple = path_[targetIndex];
                        Cell* nextTargetCell = getCells()[std::get<0>(coordinateTuple)][std::get<1>(coordinateTuple)];
                        enemy->updateTarget(enemy->getX(), enemy->getY(), nextTargetCell->getX(), nextTargetCell->getY());
                    }
                }
                // if moving vertically
                if(dir == 3 || dir == 4) {
                    if(enemy->getY() == enemy->getTargetY()) {
                        std::tuple<int, int> coordinateTuple = path_[targetIndex];
                        Cell* nextTargetCell = getCells()[std::get<0>(coordinateTuple)][std::get<1>(coordinateTuple)];
                        enemy->updateTarget(enemy->getX(), enemy->getY(), nextTargetCell->getX(), nextTargetCell->getY());
                    }
                }
                enemy->updatePosition(deltaTime, dir);

            }
        }
        i--;
    }
    if(!enemyFound && enemyAmount_ == 10) { // if there are no enemies alive on the grid and all enemies of current level have spawned
        levelOver_ = true;
        enemyAmount_ = 0;
    }
}

