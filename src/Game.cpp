#include "Game.hpp"


// constructor, which takes no parameters
// different enemy types are "chicken", "pig", "cow", "wolf" and "horse"
// initialized values: level 1, money 300, lives 5, cooldown 6 seconds, other values are 0 and the boolean values are false
Game::Game()
    : enemyTypes_({"chicken", "pig", "cow", "wolf", "horse"}), level_(1),
      money_(300), lives_(5), points_(0), gameOver_(false), gamePassed_(false),
      coolDownTimer_(6.0f), enemyTextureTime_(0.0f), enemyAmount_(0), timeUntilNextEnemy_(0.0f), doDeletes_(false) {}

// destructor
Game::~Game() {}

// Creates the 20x20 grid. The cells that belong in the gamePath vector are of type "path" and the rest of the cells are land
// returns the created grid as Grid*
Grid* Game::createGrid() {
    std::vector<std::vector<Cell*>> gridMatrix;
    std::vector<Cell*> pathVector;
    std::vector<std::tuple<int, int>> gamePath = 
        {std::make_tuple(0,9), std::make_tuple(1,9), std::make_tuple(2,9), std::make_tuple(2,8), std::make_tuple(2,7),
        std::make_tuple(2,6), std::make_tuple(2,5), std::make_tuple(3,5), std::make_tuple(4,5), std::make_tuple(5,5), 
        std::make_tuple(5,6), std::make_tuple(5,7), std::make_tuple(5,8), std::make_tuple(5,9), std::make_tuple(5,10),
        std::make_tuple(5,11), std::make_tuple(5,12), std::make_tuple(5,13), std::make_tuple(4,13), std::make_tuple(3,13), 
        std::make_tuple(3,14), std::make_tuple(3,15), std::make_tuple(3,16), std::make_tuple(4,16), std::make_tuple(5,16), 
        std::make_tuple(6,16), std::make_tuple(7,16), std::make_tuple(8,16), std::make_tuple(9,16), std::make_tuple(10,16), 
        std::make_tuple(10,15), std::make_tuple(10,14), std::make_tuple(10,13), std::make_tuple(10,12), std::make_tuple(10,11), 
        std::make_tuple(10,10), std::make_tuple(10,9), std::make_tuple(10,8), std::make_tuple(10,7), std::make_tuple(10,6), 
        std::make_tuple(10,5), std::make_tuple(10,4), std::make_tuple(10,3), std::make_tuple(10,2), std::make_tuple(11,2), 
        std::make_tuple(12,2), std::make_tuple(13,2), std::make_tuple(14,2), std::make_tuple(14,3), std::make_tuple(14,4), 
        std::make_tuple(14,5), std::make_tuple(14,6), std::make_tuple(14,7), std::make_tuple(14,8), std::make_tuple(14,9), 
        std::make_tuple(14,10), std::make_tuple(14,11), std::make_tuple(15,11), std::make_tuple(16,11), std::make_tuple(17,11), 
         std::make_tuple(18,11), std::make_tuple(19,11)};

    path_ = gamePath;

    for(int i = 0; i < 20; i++) {
        std::vector<Cell*> gridVector;
        for(int j = 0; j < 20; j++) {
            if (std::find(gamePath.begin(), gamePath.end(), std::make_tuple(i,j)) != gamePath.end()) {
                gridVector.push_back(new Cell("path", i, j));
            }
            else {
                gridVector.push_back(new Cell("land", i, j));
            }
        }
        gridMatrix.push_back(gridVector);
    }
    Grid* grid = new Grid(gridMatrix, path_);
    return grid;
}


// assigns 5 sec cooldown between levels to float coolDownTimer_, so that there is time to build new towers etc.
// returns nothing
void Game::startCooldown() {
    coolDownTimer_ = 5.0f;
}

/*
This function starts the game and keeps it running until the window is closed. It deals with user input (button clicking),
and calls different functions to run the game. It also initializes audiovisual elements. 
It takes no parameters and returns nothing.
*/
void Game::startGame() {
    sf::RenderWindow window(sf::VideoMode(1422, 800), "Farm Defence"); //the window is initialized here so that everything can be drawn to the same window
    // the window size is 1422x800 so that it has 16:9 aspect ratio
    Graphics graph;
    grid_ = createGrid();

    sf::Clock clock;

    //Next the different audio and visual elements are downloaded so that our game can use them.

    //sounds
    std::vector<sf::Sound*> soundVector;

    sf::SoundBuffer arrowBuffer;
    if(!arrowBuffer.loadFromFile("resources/Arrow.wav")) {
        std::cout << "Failed to download sound!";
    }
    sf::Sound arrowSound;
    arrowSound.setBuffer(arrowBuffer);
    arrowSound.setVolume(50);
    soundVector.push_back(&arrowSound);

    sf::SoundBuffer bombBuffer;
    if(!bombBuffer.loadFromFile("resources/Bomb.wav")) {
        std::cout << "Failed to download sound!";
    }
    sf::Sound bombSound;
    bombSound.setBuffer(bombBuffer);
    bombSound.setVolume(40);
    soundVector.push_back(&bombSound);

    sf::SoundBuffer laserBuffer;
    if(!laserBuffer.loadFromFile("resources/Laser.wav")) {
        std::cout << "Failed to download sound!";
    }
    sf::Sound laserSound;
    laserSound.setBuffer(laserBuffer);
    laserSound.setVolume(25);
    laserSound.setPitch(2.0);
    soundVector.push_back(&laserSound);

    sf::SoundBuffer winBuffer;
    if(!winBuffer.loadFromFile("resources/Victory.wav")) {
        std::cout << "Failed to download sound!";
    }
    sf::Sound winSound;
    winSound.setBuffer(winBuffer);
    winSound.setVolume(50);
    soundVector.push_back(&winSound);

    sf::SoundBuffer lostBuffer;
    if(!lostBuffer.loadFromFile("resources/Lost.wav")) {
        std::cout << "Failed to download sound!";
    }
    sf::Sound lostSound;
    lostSound.setBuffer(lostBuffer);
    lostSound.setVolume(50);
    soundVector.push_back(&lostSound);


    //enemy textures
    std::vector<sf::Texture> enemyTextures;
    
    sf::Texture chickenTexture1;
    if (!chickenTexture1.loadFromFile("resources/chickenV1.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(chickenTexture1);

    sf::Texture chickenTexture2;
    if (!chickenTexture2.loadFromFile("resources/chickenV2.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(chickenTexture2);

    sf::Texture pigTexture1;
    if (!pigTexture1.loadFromFile("resources/pigV1.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(pigTexture1);

    sf::Texture pigTexture2;
    if (!pigTexture2.loadFromFile("resources/pigV2.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(pigTexture2);

    sf::Texture cowTexture1;
    if (!cowTexture1.loadFromFile("resources/cowV1.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(cowTexture1);

    sf::Texture cowTexture2;
    if (!cowTexture2.loadFromFile("resources/cowV2.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(cowTexture2);

    sf::Texture wolfTexture1;
    if (!wolfTexture1.loadFromFile("resources/wolfV1.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(wolfTexture1);

    sf::Texture wolfTexture2;
    if (!wolfTexture2.loadFromFile("resources/wolfV2.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(wolfTexture2);

    sf::Texture horseTexture1;
    if (!horseTexture1.loadFromFile("resources/horseV1.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(horseTexture1);

    sf::Texture horseTexture2;
    if (!horseTexture2.loadFromFile("resources/horseV2.png")) {
        std::cout << "Failed to download enemy texture!";
    }
    enemyTextures.push_back(horseTexture2);


    //grid texture
    sf::Texture gridTexture;
    if (!gridTexture.loadFromFile("resources/grid.png")) {
        std::cout << "Failed to download path texture!";
    }

    //obstacle textures
    sf::Texture mudTexture;
    if (!mudTexture.loadFromFile("resources/mud.png")) {
        std::cout << "Failed to download mud texture!";
    }

    sf::Texture poisonTexture;
    if (!poisonTexture.loadFromFile("resources/poison.png")) {
        std::cout << "Failed to download poison texture!";
    }

    //tower textures
    std::vector<sf::Texture> towerTextures;
    sf::Texture bombTowerTexture;
    if (!bombTowerTexture.loadFromFile("resources/bombTower.png")) {
        std::cout << "Failed to download tower texture!";
    }
    towerTextures.push_back(bombTowerTexture);

    sf::Texture laserXTexture;
    if (!laserXTexture.loadFromFile("resources/laserX.png")) {
        std::cout << "Failed to download tower texture!";
    }
    towerTextures.push_back(laserXTexture);

    sf::Texture laserYTexture;
    if (!laserYTexture.loadFromFile("resources/laserY.png")) {
        std::cout << "Failed to download tower texture!";
    }
    towerTextures.push_back(laserYTexture);

    sf::Texture arrowTowerTexture;
    if (!arrowTowerTexture.loadFromFile("resources/arrowTower.png")) {
        std::cout << "Failed to download tower texture!";
    }
    towerTextures.push_back(arrowTowerTexture);

    //font
    sf::Font font;
    if(!font.loadFromFile("resources/NotoSans-Black.ttf")) {
        std::cout << "Failed to download font!";
    }
    //other textures
    sf::Texture arrowTexture;
    if (!arrowTexture.loadFromFile("resources/arrow.png")) {
        std::cout << "Failed to download arrow texture!";
    }

    sf::Texture menuTexture;
    if (!menuTexture.loadFromFile("resources/menu2.png")) {
        std::cout << "Failed to download menu texture!";
    }

    sf::Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("resources/gameOver.png")) {
        std::cout << "Failed to download texture!";
    }
    sf::Texture gamePassedTexture;
    if (!gamePassedTexture.loadFromFile("resources/gamePassed.png")) {
        std::cout << "Failed to download texture!";
    }

    //texts are initialized
    sf::Text coolDownText;
    coolDownText.setFont(font);
    coolDownText.setCharacterSize(25);
    coolDownText.setFillColor(sf::Color::White);
    coolDownText.setPosition(985.0f, 15.0f);

    sf::Text informationText;
    informationText.setFont(font);
    informationText.setCharacterSize(25);
    informationText.setFillColor(sf::Color::White);
    informationText.setPosition(825.0f, 55.0f);

    sf::Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(25);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(825.0f, 140.0f);

    sf::Text levelText;
    levelText.setFont(font);
    levelText.setCharacterSize(25);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(825.0f, 15.0f);

    sf::Text moneyText;
    moneyText.setFont(font);
    moneyText.setCharacterSize(25);
    moneyText.setFillColor(sf::Color::White);
    moneyText.setPosition(825.0f, 205.0f);

    sf::Text pointsText;
    pointsText.setFont(font);
    pointsText.setCharacterSize(25);
    pointsText.setFillColor(sf::Color::White);
    pointsText.setPosition(825.0f, 170.0f);

    sf::Text buildText;
    buildText.setFont(font);
    buildText.setString("Build tower:");
    buildText.setCharacterSize(25);
    buildText.setFillColor(sf::Color::White);
    buildText.setPosition(825.0f, 300.0f);

    sf::Text buildObstacleText;
    buildObstacleText.setFont(font);
    buildObstacleText.setString("Build obstacle:");
    buildObstacleText.setCharacterSize(25);
    buildObstacleText.setFillColor(sf::Color::White);
    buildObstacleText.setPosition(825.0f, 505.0f);

    sf::Text upgradeText;
    upgradeText.setFont(font);
    upgradeText.setString("Upgrade tower:");
    upgradeText.setCharacterSize(25);
    upgradeText.setFillColor(sf::Color::White);
    upgradeText.setPosition(820.0f, 650.0f);

    sf::Text targetText;
    targetText.setFont(font);
    targetText.setString("Choose target: ");
    targetText.setCharacterSize(25);
    targetText.setFillColor(sf::Color::White);
    targetText.setPosition(1200.0f, 300.0f);

    sf::Text upgradePrices;  //price table for upgrades
    upgradePrices.setFont(font);
    upgradePrices.setString("UPGRADE PRICES:\n Increase range:\n     arrow(40)\n     bomb(150)\n     laser(-) \nDouble damage:\n     arrow(80)\n     bomb(150)\n     laser(200)\nIncrease speed:\n     arrow(40)\n     bomb(80)\n     laser(80)");
    upgradePrices.setCharacterSize(12);
    upgradePrices.setFillColor(sf::Color::Black);
    upgradePrices.setPosition(1200.0f, 25.0f);

    //buttons are created
    Button* bombTowerButton = new Button("BombTower(200)", sf::Vector2f(135, 50), sf::Vector2f(825, 345), sf::Color::White, font, 13);
    Button* arrowTowerButton = new Button("ArrowTower(100)", sf::Vector2f(135, 50), sf::Vector2f(975, 345), sf::Color::Yellow, font, 13);
    Button* laserTowerXButton = new Button("LaserTowerX(300)", sf::Vector2f(135, 50), sf::Vector2f(825, 410), sf::Color::Magenta, font, 13);
    Button* laserTowerYButton = new Button("LaserTowerY(300)", sf::Vector2f(135, 50), sf::Vector2f(975, 410), sf::Color::Cyan, font, 13);
    Button* upgradeRangeButton = new Button("Increase range", sf::Vector2f(120, 50), sf::Vector2f(825, 690), sf::Color(211,211,211), font, 13);
    Button* upgradeDamageButton = new Button("Double damage", sf::Vector2f(120, 50), sf::Vector2f(950, 690), sf::Color(211,211,211), font, 13);
    Button* upgradeSpeedButton = new Button("Increase speed", sf::Vector2f(120, 50), sf::Vector2f(1075, 690), sf::Color(211,211,211), font, 13);
    Button* destroyButton = new Button("Destroy tower", sf::Vector2f(120, 50), sf::Vector2f(1200, 690), sf::Color::Red, font, 13);
    Button* mudObstacleButton = new Button("Obstacle: Mud(40)", sf::Vector2f(153, 50), sf::Vector2f(990, 545), sf::Color(139,69,19), font, 13);
    Button* poisonObstacleButton = new Button("Obstacle: Poison(80)", sf::Vector2f(153, 50), sf::Vector2f(825, 545), sf::Color(50,205,50), font, 13);
    Button* furthestEnemyButton = new Button("Target: Furthest", sf::Vector2f(170, 50), sf::Vector2f(1205, 350), sf::Color(169,169,169), font, 15);
    Button* weakestEnemyButton = new Button("Target: Weakest", sf::Vector2f(170, 50), sf::Vector2f(1205, 420), sf::Color(169,169,169), font, 15);
    Button* strongestEnemyButton = new Button("Target: Strongest", sf::Vector2f(170, 50), sf::Vector2f(1205, 490), sf::Color(169,169,169), font, 15);
    Button* closestEnemyButton = new Button("Target: Closest", sf::Vector2f(170, 50), sf::Vector2f(1205, 560), sf::Color(169,169,169), font, 15);

    //in the beginning they are not pressed
    bool bombTowerButtonPressed = false;
    bool laserTowerXButtonPressed = false;
    bool laserTowerYButtonPressed = false;
    bool arrowTowerButtonPressed = false;
    bool destroyButtonPressed = false;
    bool upgradeRangeButtonPressed = false;
    bool upgradeDamageButtonPressed = false;
    bool upgradeSpeedButtonPressed = false;
    bool mudObstacleButtonPressed = false;
    bool poisonObstacleButtonPressed = false;
    bool furthestEnemyButtonPressed = false;
    bool weakestEnemyButtonPressed = false;
    bool strongestEnemyButtonPressed = false;
    bool closestEnemyButtonPressed = false;

    //main loop, that runs as long as the game window is open
    while (window.isOpen()) { 
        sf::Event event; 
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { //user can close the window
                window.close();
                doDeletes_ = true;
            }
            // If a button is clicked, change its 'buttonPressed' to true.
            else if (!gameOver_ && !gamePassed_ && event.type == sf::Event::MouseButtonPressed) {
                if (bombTowerButton->isMouseOver(window)) 
                    bombTowerButtonPressed = true;
                else if ((laserTowerXButton->isMouseOver(window)))
                    laserTowerXButtonPressed = true;
                else if ((laserTowerYButton->isMouseOver(window)))
                    laserTowerYButtonPressed = true;
                else if ((arrowTowerButton->isMouseOver(window)))
                    arrowTowerButtonPressed = true;
                else if (destroyButton->isMouseOver(window))
                    destroyButtonPressed = true;
                else if (upgradeRangeButton->isMouseOver(window))
                    upgradeRangeButtonPressed = true;
                else if (upgradeDamageButton->isMouseOver(window))
                    upgradeDamageButtonPressed = true;
                else if (upgradeSpeedButton->isMouseOver(window))
                    upgradeSpeedButtonPressed = true;
                else if (mudObstacleButton->isMouseOver(window))
                    mudObstacleButtonPressed = true;
                else if (poisonObstacleButton->isMouseOver(window))
                    poisonObstacleButtonPressed = true;
                else if (furthestEnemyButton->isMouseOver(window))
                    furthestEnemyButtonPressed = true;
                else if (weakestEnemyButton->isMouseOver(window))
                    weakestEnemyButtonPressed = true;
                else if (strongestEnemyButton->isMouseOver(window))
                    strongestEnemyButtonPressed = true;
                else if (closestEnemyButton->isMouseOver(window))
                    closestEnemyButtonPressed = true;
                
                /*If the user clicks on a tower building button, they should after that click a land cell on the grid
                and the tower will be built there, if the cell is empty.*/
                else if (bombTowerButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() == nullptr && !clickedCell->isPath()) {
                            grid_->createTower("bomb", std::get<0>(pixelsToCell), std::get<1>(pixelsToCell));
                        }
                    }
                    bombTowerButtonPressed = false;
                }
                else if (laserTowerXButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() == nullptr && !clickedCell->isPath()) {
                            grid_->createTower("laserX", std::get<0>(pixelsToCell), std::get<1>(pixelsToCell));
                        }
                    }
                    laserTowerXButtonPressed = false;
                }
                else if (laserTowerYButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() == nullptr && !clickedCell->isPath()) {
                            grid_->createTower("laserY", std::get<0>(pixelsToCell), std::get<1>(pixelsToCell));
                        }
                    }
                    laserTowerYButtonPressed = false;
                }
                else if (arrowTowerButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() == nullptr && !clickedCell->isPath()) {
                            grid_->createTower("arrow", std::get<0>(pixelsToCell), std::get<1>(pixelsToCell));
                        }
                    }
                    arrowTowerButtonPressed = false;
                }
                /*if the user clicks on a 'destroy tower' button, they should after that click a tower on the grid
                and the tower will removed from there, if the cell has a tower in it*/
                else if (destroyButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() != nullptr) {
                            grid_->destroyTower(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell));
                        }
                    }
                    destroyButtonPressed = false;
                }
                /*If the user clicks on an 'upgrade range' button, they should after that click a tower on the grid
                and the tower's range will be upgraded, if it hasn't been upgraded already. LaserTowers range cannot be upgraded. 
                The price of the upgrade depends on the tower type. */
                else if (upgradeRangeButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() != nullptr) {
                            Tower* tower = clickedCell->getTower();
                            if(tower->getType() == "bomb") {
                                if(money_ >= 150) {
                                    if(tower->upgradeTower("range")) { // succeeded or not
                                        grid_->updateMoney(150); // decrease money by 150
                                    }
                                }
                            }
                            else if(tower->getType() == "arrow") {
                                if(money_ >= 40) {
                                    if(tower->upgradeTower("range")) {
                                        grid_->updateMoney(40);
                                    }
                                }
                            }
                        }
                    }
                    upgradeRangeButtonPressed = false;
                }
                /*If the user clicks on an 'upgrade damage' button, they should after that click a tower on the grid
                and the tower's damage will be upgraded, if it hasn't been upgraded already.
                The price of the upgrade depends on the tower type. */
                else if (upgradeDamageButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() != nullptr) {
                            Tower* tower = clickedCell->getTower();
                            if(tower->getType() == "laserX" || tower->getType() == "laserY") {
                                if(money_ >= 200) {
                                    if(tower->upgradeTower("damage")) { // succeeded or not
                                        grid_->updateMoney(200); // decrease money by 200
                                    }
                                }
                            }
                            else if(tower->getType() == "bomb") {
                                if(money_ >= 150) {
                                    if(tower->upgradeTower("damage")) {
                                        grid_->updateMoney(150);
                                    }
                                }
                            }
                            else if(tower->getType() == "arrow") {
                                if(money_ >= 80) {
                                    if(tower->upgradeTower("damage")) {
                                        grid_->updateMoney(80);
                                    }
                                }
                            }
                        }
                    }
                    upgradeDamageButtonPressed = false;
                }
                /*If the user clicks on an 'upgrade speed' button, they should after that click a tower on the grid
                and the tower's speed will be upgraded, if it hasn't been upgraded already.
                The price of the upgrade depends on the tower type. */
                else if (upgradeSpeedButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() != nullptr) {
                            Tower* tower = clickedCell->getTower();
                            if(tower->getType() == "laserX" || tower->getType() == "laserY") {
                                if(money_ >= 80) {
                                    if(tower->upgradeTower("speed")) { // succeeded or not
                                        grid_->updateMoney(80); // decrease money by 80
                                    }
                                }
                            }
                            else if(tower->getType() == "bomb") {
                                if(money_ >= 80) {
                                    if(tower->upgradeTower("speed")) {
                                        grid_->updateMoney(80);
                                    }
                                }
                            }
                            else if(tower->getType() == "arrow") {
                                if(money_ >= 40) {
                                    if(tower->upgradeTower("speed")) {
                                        grid_->updateMoney(40);
                                    }
                                }
                            }
                        }
                    }
                    upgradeSpeedButtonPressed = false;
                }
                /*If the user clicks on a 'build obstacle' button, they should after that click a path cell on the grid
                and the obstacle will be built there.*/
                else if (mudObstacleButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getObstacle() == nullptr && clickedCell->isPath()) {
                            grid_->createObstacle("mud", clickedCell->getX(), clickedCell->getY());
                        }
                    }
                    mudObstacleButtonPressed = false;
                }
                else if (poisonObstacleButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getObstacle() == nullptr && clickedCell->isPath()) {
                            grid_->createObstacle("poison", clickedCell->getX(), clickedCell->getY());
                        }
                    }
                    poisonObstacleButtonPressed = false;
                }
                /*If the user clicks on a 'change target' button, they should after that click a tower on the grid
                to change its target, according to the pressed button.*/
                else if (furthestEnemyButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() != nullptr) {
                            Tower* tower = clickedCell->getTower();
                            if (tower->getType() == "arrow"){
                                ArrowTower* arrowtower = dynamic_cast<ArrowTower*>(tower);
                                arrowtower->changeTarget(1);  //change the arrow to target the furthest enemy
                            }
                        }
                    }
                    furthestEnemyButtonPressed = false;
                }
                else if (weakestEnemyButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() != nullptr) {
                            Tower* tower = clickedCell->getTower();
                            if (tower->getType() == "arrow"){
                                ArrowTower* arrowtower = dynamic_cast<ArrowTower*>(tower);
                                arrowtower->changeTarget(2);  //change the arrow to target the enemy with lowest HP
                            }
                        }
                    }
                    weakestEnemyButtonPressed = false;
                }
                else if (strongestEnemyButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() != nullptr) {
                            Tower* tower = clickedCell->getTower();
                            if (tower->getType() == "arrow"){
                                ArrowTower* arrowtower = dynamic_cast<ArrowTower*>(tower);
                                arrowtower->changeTarget(3); //change the arrow to target the enemy with highest HP
                            }
                        }
                    }
                    strongestEnemyButtonPressed = false;
                }
                else if (closestEnemyButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::tuple<int, int> pixelsToCell = std::make_tuple(mousePosition.x / 40, mousePosition.y / 40);
                    if (grid_->isInsideGrid(std::get<0>(pixelsToCell), std::get<1>(pixelsToCell))) {
                        Cell* clickedCell = grid_->getCells()[std::get<0>(pixelsToCell)][std::get<1>(pixelsToCell)];
                        if(clickedCell->getTower() != nullptr) {
                            Tower* tower = clickedCell->getTower();
                            if (tower->getType() == "arrow"){
                                ArrowTower* arrowtower = dynamic_cast<ArrowTower*>(tower);
                                arrowtower->changeTarget(4); //change the arrow to target the closest enemy
                            }
                        }
                    }
                    closestEnemyButtonPressed = false;
                }

            //the game doesnt stretch when fullscreen
            }
            else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }
        
        window.clear();

        //run the game if it has not been won or lost
        if (!gameOver_ && !gamePassed_) {
            float deltaTime = clock.restart().asSeconds();
            enemyTextureTime_ += deltaTime;
            
            //differentiate between two pictures of the enemy to create animated effect
            if(enemyTextureTime_ < 0.3f) {
                graph.drawGame(grid_, window, enemyTextures[(level_-1)*2], gridTexture, towerTextures, mudTexture, poisonTexture);
            }
            else {
                graph.drawGame(grid_, window, enemyTextures[(level_-1)*2+1], gridTexture, towerTextures, mudTexture, poisonTexture);
                if(enemyTextureTime_ > 0.6f) {
                    enemyTextureTime_ -= 0.6f;
                }
            }

            std::vector<Projectile*>& arrows = grid_->getArrows();
            if(!arrows.empty()) {
                graph.drawArrows(arrows, window, arrowTexture);
            }
            grid_->updateArrowsState(deltaTime);


            graph.drawMenu(menuTexture, window);

            //if the game is running (is not between levels), create new enemies with some time between
            if(coolDownTimer_ == 0.0f) {
                if(timeUntilNextEnemy_ == 0.0f && enemyAmount_ < 10) {
                    grid_->createEnemy(enemyTypes_[level_-1]);
                    enemyAmount_ += 1;
                    timeUntilNextEnemy_ = 1.0f;
                }
                else {
                    timeUntilNextEnemy_ -= deltaTime;
                    timeUntilNextEnemy_ = std::max(timeUntilNextEnemy_, 0.0f);
                }
                grid_->updateGridState(deltaTime, enemyTypes_[level_-1], lives_);  //call this function to move the enemies
                
                //call towershoot and obstacledamage for all towers and obstacles on the grid
                for (int i = 0; i < grid_->getWidth(); i++) {
                    for (int j = 0; j < grid_->getWidth(); j++) {
                        Cell* cell = grid_->getCells()[i][j];
                        if (cell->getTower() != nullptr){
                            grid_->towerShoot(i, j, deltaTime, window, soundVector);
                        }
                        if (cell->getObstacle() != nullptr && cell->getEnemy() != nullptr){
                            grid_->obstacleDamage(i, j);
                        }
                    }
                }
            }
            else {
                std::ostringstream stream;
                stream << std::fixed << std::setprecision(1) << coolDownTimer_;
                std::string coolDownString = stream.str();
                coolDownText.setString("Cooldown: " + coolDownString);
                graph.drawCoolDownText(coolDownText, window);

                coolDownTimer_ -= deltaTime;
                coolDownTimer_ = std::max(coolDownTimer_, 0.0f);
                if(coolDownTimer_ == 0.0f) {
                    grid_->createEnemy(enemyTypes_[level_-1]);
                    enemyAmount_ += 1;
                    timeUntilNextEnemy_ = 1.0f;
                }
            }

            //get the statistics from grid (they are updated there for simplicity but displayed through the game class)
            money_ = grid_->getMoney();
            lives_ = grid_->getLives();
            points_ = grid_->getPoints();

            //update current statistics to the texts
            informationText.setString("Current enemy type: \n" + enemyTypes_[level_-1] + " (10)");
            livesText.setString("Lives left: " + std::to_string(lives_));
            levelText.setString("Level: " + std::to_string(level_));
            moneyText.setString("Money: " + std::to_string(money_));
            pointsText.setString("Points: " + std::to_string(points_));

            //draw everything on the menu
            graph.drawTexts(informationText, livesText, levelText, moneyText, pointsText, buildText, buildObstacleText, upgradeText, targetText, upgradePrices, window);
            graph.drawButton(bombTowerButton, window);
            graph.drawButton(laserTowerXButton, window);
            graph.drawButton(laserTowerYButton, window);
            graph.drawButton(arrowTowerButton, window);
            graph.drawButton(destroyButton, window);
            graph.drawButton(upgradeRangeButton, window);
            graph.drawButton(upgradeDamageButton, window);
            graph.drawButton(upgradeSpeedButton, window);
            graph.drawButton(mudObstacleButton, window);
            graph.drawButton(poisonObstacleButton, window);
            graph.drawButton(furthestEnemyButton, window);
            graph.drawButton(weakestEnemyButton, window);
            graph.drawButton(strongestEnemyButton, window);
            graph.drawButton(closestEnemyButton, window);

            if(lives_ == 0) {
                gameOver_ = true;
                doDeletes_ = true;
            }
            if(grid_->isLevelOver()) {
                enemyAmount_ = 0; //reset enemyAmount
                if(level_ == 5) { //if the level is 5 and the level is over, the player has won
                    gamePassed_ = true;
                }
                if(level_ < 5) {level_ += 1;} //otherwise move to next level
                grid_->updateLevelOver(); //change grid's levelOver_ to false
                startCooldown(); //cooldown between levels
            }
        }
        if (gamePassed_) {
            graph.drawGamePassed(window, gamePassedTexture, points_, font);
            if(!gameWonSoundPlayed_) {
                soundVector[3]->play();
            }
            gameWonSoundPlayed_ = true;

        }
        if (gameOver_) {
            graph.drawGameOver(window, gameOverTexture, points_, font);
            if(!gameOverSoundPlayed_) {
                soundVector[4]->play();
            }
            gameOverSoundPlayed_ = true;

        }
        window.display();
    } 
    
    // free the memory from pointers when game ends
    if (doDeletes_) {
        grid_->deleteCells(); // delete pointers of enemies, obstacles and towers inside cells, and then delete cells
        delete grid_; // delete pointer to grid
        // delete buttons' pointers
        delete bombTowerButton; 
        delete arrowTowerButton;
        delete laserTowerXButton;
        delete laserTowerYButton;
        delete upgradeSpeedButton;
        delete destroyButton;
        delete upgradeRangeButton;
        delete upgradeDamageButton;
        delete mudObstacleButton;
        delete poisonObstacleButton;   
        delete furthestEnemyButton;
        delete weakestEnemyButton;
        delete strongestEnemyButton;
        delete closestEnemyButton; 
    }

}



// THE FOLLOWING FUNCTIONS ARE FOR TESTING

// returns the current level as int
int Game::getLevel() const {
    return level_;
}

// returns the current amount of money as int
int Game::getMoney() const {
    return money_;
}

// returns the current number of lives left as int
int Game::getLives() const {
    return lives_;
}

// returns true if game is over, otherwise false
bool Game::isGameOver() const {
    return gameOver_;
}

// returns true if game is passed, otherwise false
bool Game::isGamePassed() const {
    return gamePassed_;
}

// returns the cooldowntimer as float
float Game::getCoolDownTimer() const {
    return coolDownTimer_;
}

// initializes game for tests
// takes no parameters and returns nothing
void Game::initializeForTest() {
    grid_ = createGrid();

    window_.create(sf::VideoMode(1422, 800), "FarmDefence Grid Test");

    sf::SoundBuffer arrowBuffer, bombBuffer, laserBuffer;
    arrowBuffer.loadFromFile("src/Arrow.wav");
    bombBuffer.loadFromFile("src/Bomb.wav");
    laserBuffer.loadFromFile("src/Laser.wav");

    sf::Sound arrowSound, bombSound, laserSound;
    arrowSound.setBuffer(arrowBuffer);
    bombSound.setBuffer(bombBuffer);
    laserSound.setBuffer(laserBuffer);

    arrowSound.setVolume(50);
    bombSound.setVolume(40);
    laserSound.setVolume(25);
    laserSound.setPitch(2.0);

    soundVector_.push_back(&arrowSound);
    soundVector_.push_back(&bombSound);
    soundVector_.push_back(&laserSound);
}