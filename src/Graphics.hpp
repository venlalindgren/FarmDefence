#ifndef FARM_DEFENCE_GRAPHICS
#define FARM_DEFENCE_GRAPHICS

#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "Enemy.hpp"
#include "Grid.hpp"
#include "Cell.hpp"
#include "Button.hpp"
#include "Obstacle.hpp"

/*
The graphics class is responsible for (almost) all drawing. The window has to be given as a parameter,
so that everything is drawn to the same window.
*/

class Graphics {
 public:
    Graphics();
    ~Graphics();
    void drawGame(Grid* grid, sf::RenderWindow& window, const sf::Texture& enemyTexture,
                const sf::Texture& gridTexture, const std::vector<sf::Texture>& towerTextures, 
                const sf::Texture& mudTexture, const sf::Texture& poisonTexture);
    void drawEnemy(Enemy* enemy, sf::RenderWindow& window, const sf::Texture& enemyTexture);
    void drawTower(Tower* tower, sf::RenderWindow& window, const std::vector<sf::Texture>& towerTextures);
    void drawObstacle(Obstacle* obstacle, sf::RenderWindow& window, const sf::Texture& mudTexture, const sf::Texture& poisonTexture);
    void drawGameOver(sf::RenderWindow& window, const sf::Texture& gameOverTexture, int points, sf::Font font);
    void drawGamePassed(sf::RenderWindow& window, const sf::Texture& gamePassedTexture, int points, sf::Font font);
    void drawArrows(std::vector<Projectile*> arrows, sf::RenderWindow& window, const sf::Texture& arrowTexture);
    void drawButton(Button* button, sf::RenderWindow& window);
    void drawMenu(sf::Texture& menuTexture, sf::RenderWindow& window);
    void drawCoolDownText(sf::Text coolDownText, sf::RenderWindow& window);
    void drawTexts(sf::Text info, sf::Text lives, sf::Text level, sf::Text money, sf::Text points,
                sf::Text buildT, sf::Text buildO, sf::Text upgrade, sf::Text target, sf::Text upgrades, sf::RenderWindow& window); 
};


#endif
