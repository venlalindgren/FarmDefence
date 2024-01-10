#include "Button.hpp"

// constructor for the button class
// parameters: 
//      text to display on the button (type: string), 
//      size of the button's bounding rectangle (type: SFML's 2D vector of floats)
//      position of the button's top-left corner (type: SFML's 2D vector of floats)
//      fill color of the button (type: SFML color)
//      font used for the button's text (type: SFML font)
//      font size for the button's text (type: integer)
Button::Button(std::string text, sf::Vector2f size, sf::Vector2f position, sf::Color color, const sf::Font &font, int fontSize) {
    // set the private variable shape (type sf::RectangleShape )
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(color);
    // set the private variable buttonText (type sf::Text)
    buttonText.setString(text);
    buttonText.setFont(font);
    buttonText.setCharacterSize(fontSize);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(position.x + 5, position.y + 10);
    if (fontSize==10)
        buttonText.setPosition(position.x + 5, position.y + 5);
}

// checks if the mouse cursor is over the button
// takes window as a parameter, which is a reference to the SFML RenderWindow
// returns true if the mouse cursor is over the button, false otherwise
bool Button::isMouseOver(sf::RenderWindow& window) const {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

// retrieves the button's shape (rectangle)
// takes no parameters
// returns a copy of the private variable (SFML RectangleShape) representing the button's shape
sf::RectangleShape Button::getShape() {
    return shape;
}
    
// retrieves the button's text
// takes no parameters
// returns a copy of the private variable (SFML Text) representing the button's text
sf::Text Button::getText() {
    return buttonText;
}

