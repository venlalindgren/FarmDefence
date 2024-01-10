#ifndef FARM_DEFENCE_GAME_CLASS
#define FARM_DEFENCE_GAME_CLASS

#include "Grid.hpp"
#include "Enemy.hpp"
#include "Cell.hpp"
#include "Button.hpp"
#include "Graphics.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream> //for cooldown text
#include <iomanip> //for cooldown text
#include <iostream>
#include <tuple>
#include <algorithm>
#include <vector>
#include <string>


// the Game class is responsible for managing relevant things in the game and
// on each frame calls other classes' functions, which are for example implemented in the Grid class

class Game {
 public:
      Game();
      ~Game();
      Grid* createGrid();
      void startGame();
      void startCooldown();
      //The following functions are for tests
      int getLevel() const;
      int getMoney() const;
      int getLives() const;
      bool isGameOver() const;
      bool isGamePassed() const;
      float getCoolDownTimer() const;
      void initializeForTest();

 private:
      Grid* grid_;
      std::vector<std::string> enemyTypes_; // possible different enemy types
      std::vector<std::tuple<int, int>> path_; // list of cell coordinates which are path
      int level_;
      int money_;
      int lives_;
      int points_;
      float coolDownTimer_; // cooldown timer, which is used between levels
      bool gameOver_;
      bool gamePassed_;
      float enemyTextureTime_; // used to change between textures for enemies to get animation effect
      int enemyAmount_; // how many enemies have been spawned during the current level
      float timeUntilNextEnemy_; // used to spawn enemies with a specified time interval
      bool gameWonSoundPlayed_;
      bool gameOverSoundPlayed_;
      bool doDeletes_;
      sf::RenderWindow window_;
      std::vector<sf::Sound*> soundVector_;

};


#endif
