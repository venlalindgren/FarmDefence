#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.hpp"
#include "Grid.hpp"

class GameTest : public ::testing::Test {
protected:
    Game game;
    Grid* grid;

    void SetUp() override {
        grid = game.createGrid();
    }
};

// Test to verify correct initialization of Game object
TEST_F(GameTest, ConstructorInitializesCorrectly) { //Checks whether the game is being initialized with the correct values 
    EXPECT_EQ(game.getLevel(), 1);
    EXPECT_EQ(game.getMoney(), 300);
    EXPECT_EQ(game.getLives(), 5);
    EXPECT_FALSE(game.isGameOver());
    EXPECT_FALSE(game.isGamePassed());
}

// Test to verify the startCooldown method works correctly
TEST_F(GameTest, StartCooldownSetsCorrectValue) {
    game.startCooldown();
    EXPECT_FLOAT_EQ(game.getCoolDownTimer(), 5.0f);
}

// Test Tower Creation and Destruction
TEST_F(GameTest, TowerCreationAndDestruction) {   //Checks the tower mechanics by creating a tower to a specific location, checks whether it exists there and can be destroyed
    Grid* grid = game.createGrid();
    grid->createTower("bomb", 3, 3);
    EXPECT_NE(grid->getCells()[3][3]->getTower(), nullptr);

    grid->destroyTower(3, 3);
    EXPECT_EQ(grid->getCells()[3][3]->getTower(), nullptr);
}

// Test Enemy Spawning
TEST_F(GameTest, EnemySpawning) {      //Checks whether enemy can be created and does the enemyAmount increase by doing so
    Grid* grid = game.createGrid();
    int initialEnemyCount = grid->getEnemyAmount();
    EXPECT_EQ(initialEnemyCount, 0);

    grid->createEnemy("chicken");
    EXPECT_GT(grid->getEnemyAmount(), initialEnemyCount);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
