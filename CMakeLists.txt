cmake_minimum_required(VERSION 3.12)

# Define our project with the name "FarmDefence"
project(FarmDefence)

# Set the C++ standard globally as it applies to all targets
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(libs/sfml)

# Tell cmake what the main executable file for the project is, i.e. what has the main() function
add_executable(FarmDefence src/FarmDefence.cpp
                src/Cell.cpp
                src/Game.cpp
                src/Graphics.cpp
                src/Enemy.cpp
                src/Tower.cpp
                src/Grid.cpp
                src/BombTower.cpp
                src/YLaserTower.cpp
                src/XLaserTower.cpp
                src/ArrowTower.cpp
                src/Button.cpp
                src/Projectile.cpp
                src/Obstacle.cpp
)

# Link the SFML library into our executable
target_link_libraries(FarmDefence sfml-network sfml-system sfml-window sfml-graphics sfml-audio)



# Add GoogleTest subdirectory
add_subdirectory(libs/googletest)

# Define the test executable
add_executable(FarmDefence_tests
    tests/Test1.cpp
    src/Cell.cpp
    src/Game.cpp
    src/Graphics.cpp
    src/Enemy.cpp
    src/Tower.cpp
    src/Grid.cpp
    src/BombTower.cpp
    src/YLaserTower.cpp
    src/XLaserTower.cpp
    src/ArrowTower.cpp
    src/Button.cpp
    src/Projectile.cpp
    src/Obstacle.cpp
)

# Include the project's source directory and the Google Test include directories for the test executable
target_include_directories(FarmDefence_tests PRIVATE src)

# Link Google Test to the test executable
target_link_libraries(FarmDefence_tests gtest gtest_main)

# Link the SFML library into our test executable
target_link_libraries(FarmDefence_tests sfml-network sfml-system sfml-window sfml-graphics sfml-audio)



# Release model
set(CMAKE_BUILD_TYPE Release)

# Install the main executable
install(TARGETS FarmDefence DESTINATION bin)

