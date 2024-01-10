#include "Graphics.hpp"

// constructor
Graphics::Graphics() {}

// destructor
Graphics::~Graphics() {}

/*
Draws an enemy (its textures change between levels and 'animation' frames).
Draws the enemy's HP as a bar under the enemy.
*/
// parameters: enemy to be drawn (Enemy* enemy), the window where we want to draw (sf::RenderWindow& window) and the desired enemy texture (const sf::Texture& enemyTexture)
// returns nothing
void Graphics::drawEnemy(Enemy* enemy, sf::RenderWindow& window, const sf::Texture& enemyTexture) {
    sf::Sprite enemySprite(enemyTexture);
    float scaleX = 40.0f / enemyTexture.getSize().x;
    float scaleY = 40.0f / enemyTexture.getSize().y;
    enemySprite.setScale(scaleX, scaleY);
    enemySprite.setPosition((enemy->getX()) * 40, (enemy->getY()) * 40);

    window.draw(enemySprite);

    //HP bars
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(42.0f, 9.0f));
    background.setFillColor(sf::Color(0, 0, 0));
    background.setPosition(std::round(enemy->getX() * 40 - 1), std::round(enemy->getY() * 40 + 39));

    float hp = enemy->getHP();
    float maxHP = enemy->getMaxHP();
    float ratio = hp/maxHP;
    sf::Color color;
    //change color of the bar depending on the percentage of HP left
    if(ratio <= 0.25) {
        color = sf::Color::Red;
    }
    else if(ratio <= 0.50) {
        color = sf::Color(255, 165, 0);
    }
    else if(ratio <= 0.80) {
        color = sf::Color(255, 245, 0);
    }
    else {
        color = sf::Color(0, 160, 20);
    }
    sf::RectangleShape foreground;
    foreground.setSize(sf::Vector2f(40.0*ratio, 7.0f));
    foreground.setFillColor(color);
    foreground.setPosition(std::round(enemy->getX() * 40), std::round(enemy->getY() * 40 + 40));

    window.draw(background);
    window.draw(foreground);

} 

/*
Draw a tower, with different pictures depending on the type. Draw different small rectangles on top of the 
tower, if it has been updated.
*/
// parameters: tower to be drawn (Tower* tower), the window where we want to draw (sf::RenderWindow& window) and the desired tower textures (const std::vector<sf::Texture>& towerTextures)
// returns nothing
void Graphics::drawTower(Tower* tower, sf::RenderWindow& window, const std::vector<sf::Texture>& towerTextures) { 
    sf::Sprite towerSprite;
    float scaleX;
    float scaleY;

    if (tower->getType() == "bomb") {
        towerSprite.setTexture(towerTextures[0]);
        scaleX = 40.0f / towerTextures[0].getSize().x;
        scaleY = 40.0f / towerTextures[0].getSize().y;
    }
    else if (tower->getType() == "laserX") {
        towerSprite.setTexture(towerTextures[1]);
        scaleX = 40.0f / towerTextures[1].getSize().x;
        scaleY = 40.0f / towerTextures[1].getSize().y;
    }
    else if (tower->getType() == "laserY") {
        towerSprite.setTexture(towerTextures[2]);
        scaleX = 40.0f / towerTextures[2].getSize().x;
        scaleY = 40.0f / towerTextures[2].getSize().y;
    }
    else if (tower->getType() == "arrow") {
        towerSprite.setTexture(towerTextures[3]);
        scaleX = 40.0f / towerTextures[3].getSize().x;
        scaleY = 40.0f / towerTextures[3].getSize().y;
    }
    towerSprite.setScale(scaleX, scaleY);
    towerSprite.setPosition((tower->getXPos()) * 40, (tower->getYPos()) * 40);
    window.draw(towerSprite); 

    //draw small rectangles if the tower has been updated
    sf::RectangleShape damage(sf::Vector2f(5,5));
    sf::RectangleShape range(sf::Vector2f(5,5));
    sf::RectangleShape speed(sf::Vector2f(5,5));

    range.setFillColor(sf::Color::Blue);
    range.setPosition((tower->getXPos())*40 + 12, (tower->getYPos())*40 + 35);

    damage.setFillColor(sf::Color::Green);
    damage.setPosition((tower->getXPos())*40 + 18, (tower->getYPos())*40 + 35);

    speed.setFillColor(sf::Color::Yellow);
    speed.setPosition((tower->getXPos())*40 + 24, (tower->getYPos())*40 + 35);

    if (tower->isRangeUpgraded())
        window.draw(range);
    if (tower->isDamageUpgraded())
        window.draw(damage);
    if (tower->isSpeedUpgraded())
        window.draw(speed);
}

// draws a obstacle with different picture depending on the obstacle type
// parameters: obstacle to be drawn (Obstacle* obstacle), the window where we want to draw (sf::RenderWindow& window), texture for mud (const sf::Texture& mudTexture) and texture for poison (const sf::Texture& poisonTexture)
// returns nothing
void Graphics::drawObstacle(Obstacle* obstacle, sf::RenderWindow& window, const sf::Texture& mudTexture, const sf::Texture& poisonTexture) {
    sf::Sprite obstacleSprite;
    float scaleX;
    float scaleY;

    if(obstacle->getType() == "mud") {
        obstacleSprite.setTexture(mudTexture);
        scaleX = 40.0f / mudTexture.getSize().x;
        scaleY = 40.0f / mudTexture.getSize().y;
    }
    else if(obstacle->getType() == "poison") {
        obstacleSprite.setTexture(poisonTexture);
        scaleX = 40.0f / poisonTexture.getSize().x;
        scaleY = 40.0f / poisonTexture.getSize().y;
    }

    obstacleSprite.setScale(scaleX, scaleY);
    obstacleSprite.setPosition(obstacle->getXPos() * 40, obstacle->getYPos() * 40);
    window.draw(obstacleSprite);
}



// draws the grid, the enemies, towers and obstacles
// takes the grid (Grid*) and window (sf::RenderWindow&) as parameters, and the different textures needed for drawing
// returns nothing
void Graphics::drawGame(Grid* grid, sf::RenderWindow& window, const sf::Texture& enemyTexture, const sf::Texture& gridTexture,
                        const std::vector<sf::Texture>& towerTextures, const sf::Texture& mudTexture, const sf::Texture& poisonTexture) {

    sf::Sprite gridSprite(gridTexture);
    window.draw(gridSprite);

    std::vector<std::vector<Cell*>> gridMatrix = grid->getCells();

    //obstacle drawing is in its own loop so that enemies' healthbars do not appear under obstacles etc.
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            Cell* cell = gridMatrix[i][j];
            Obstacle* obstacle = cell->getObstacle();
            if (obstacle != nullptr) {
                drawObstacle(obstacle, window, mudTexture, poisonTexture);
            }
        }
    }
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            Cell* cell = gridMatrix[i][j];
            Enemy* enemy = cell->getEnemy();
            Tower* tower = cell->getTower();
            if (tower != nullptr) {
                drawTower(tower, window, towerTextures);
            }
            if (enemy != nullptr) {
                drawEnemy(enemy, window, enemyTexture);
            }
        }
    }
}

// draws the game over text and the final points
// parameters: takes the window (sf::RenderWindow&), game over texture (const sf::Texture&), final points (int) and the desired font (sf::Font) as parameters
// returns nothing
void Graphics::drawGameOver(sf::RenderWindow& window, const sf::Texture& gameOverTexture, int points, sf::Font font) {
    sf::Sprite gameOverSprite(gameOverTexture);
    float scaleX = 1422.0f / gameOverTexture.getSize().x;
    float scaleY = 800.0f / gameOverTexture.getSize().y;
    gameOverSprite.setScale(scaleX,scaleY);
    gameOverSprite.setPosition(0,0);
    window.draw(gameOverSprite);

    sf::Text pointsText;
    pointsText.setFont(font);
    pointsText.setCharacterSize(30);
    pointsText.setFillColor(sf::Color::White);
    pointsText.setPosition(450.0f, 450.0f);
    pointsText.setString("You lost with " + std::to_string(points) + " points. Try again.");
    window.draw(pointsText);

}

// draws the game passed text and the final points
// parameters: takes the window (sf::RenderWindow&), game passed texture (const sf::Texture&), final points (int) and the desired font (sf::Font) as parameters
// returns nothing
void Graphics::drawGamePassed(sf::RenderWindow& window, const sf::Texture& gamePassedTexture, int points, sf::Font font) {
    sf::Sprite gamePassedSprite(gamePassedTexture);
    float scaleX = 1422.0f / gamePassedTexture.getSize().x;
    float scaleY = 800.0f / gamePassedTexture.getSize().y;
    gamePassedSprite.setScale(scaleX,scaleY);
    gamePassedSprite.setPosition(0,0);
    window.draw(gamePassedSprite);

    sf::Text pointsText;
    pointsText.setFont(font);
    pointsText.setCharacterSize(30);
    pointsText.setFillColor(sf::Color::White);
    pointsText.setPosition(500.0f, 450.0f);
    pointsText.setString("You got " + std::to_string(points) + " points.");
    window.draw(pointsText);
}

// draws all the current projectile arrows
// parameters: list of current projectiles (std::vector<Projectile*>), window (sf::RenderWindow&), texture for projectiles (const sf::Texture&)
// returns nothing
void Graphics::drawArrows(std::vector<Projectile*> arrows, sf::RenderWindow& window, const sf::Texture& arrowTexture) {
    size_t i = 0;
    while(i < arrows.size()) {
        float angle = arrows[i]->getAngle();
        sf::Sprite arrowSprite(arrowTexture);
        float scaleX = 40.0f / arrowTexture.getSize().x;
        float scaleY = 40.0f / arrowTexture.getSize().y;
        arrowSprite.setScale(scaleX, scaleY);
        arrowSprite.setRotation(angle);
        arrowSprite.setOrigin(arrowSprite.getLocalBounds().width / 2, arrowSprite.getLocalBounds().height / 2);
        arrowSprite.setPosition(arrows[i]->getX() * 40+20, arrows[i]->getY() * 40+20);
        window.draw(arrowSprite);
        i++;
    }
}

// draws the menu on the right side of the grid
// takes the menu texture (sf::Texture&) and window (sf::RenderWindow&) as parameters
// returns nothing
void Graphics::drawMenu(sf::Texture& menuTexture, sf::RenderWindow& window) {
    sf::Sprite menuSprite(menuTexture);
    menuSprite.setPosition(800.0f, 0.0f);
    window.draw(menuSprite);
}

// draws a button on the window
// takes a pointer to the Button object to be drawn and a reference to the SFML RenderWindow where the button will be drawn as parameters
// returns nothing
void Graphics::drawButton(Button* button, sf::RenderWindow& window) {
    window.draw(button->getShape());
    window.draw(button->getText());
}

// draws the cooldown text
// parameters: cooldown text (sf::Text) and window (sf::RenderWindow&)
// returns nothing
void Graphics::drawCoolDownText(sf::Text coolDownText, sf::RenderWindow& window) {
    window.draw(coolDownText);
}

// draws all the other texts than the cooldown text
// parameters: all the different texts (sf::Text) and window (sf::RenderWindow&)
// returns nothing
void Graphics::drawTexts(sf::Text info, sf::Text lives, sf::Text level, sf::Text money, sf::Text points,
                sf::Text buildT, sf::Text buildO, sf::Text upgrade, sf::Text target, sf::Text upgrades, sf::RenderWindow& window) {
    window.draw(info);
    window.draw(lives);
    window.draw(level);
    window.draw(money);
    window.draw(points);
    window.draw(buildT);
    window.draw(buildO);
    window.draw(upgrade);
    window.draw(target);
    window.draw(upgrades);
}

