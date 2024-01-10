#ifndef FARM_DEFENCE_BUTTON_CLASS
#define FARM_DEFENCE_BUTTON_CLASS

#include <SFML/Graphics.hpp>

/*
Buttons are clickable objects. A player can play the game using buttons.
*/

class Button {
 public:
    Button();
    ~Button() = default;
    Button(std::string text, sf::Vector2f size, sf::Vector2f position, sf::Color color, const sf::Font &font, int fontSize);
    bool isMouseOver(sf::RenderWindow& window) const;
    sf::RectangleShape getShape();
    sf::Text getText();

 private:
    sf::RectangleShape shape;
    sf::Text buttonText;
};


#endif
