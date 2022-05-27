#include "TextButton.h"

TextButton::TextButton(sf::RenderWindow* window, sf::Font* font, string text, int x, int y)
{
    this->window = window;
    this->text = text;
    this->x = x;
    this->y = y;

    init(font);
}

void TextButton::init(sf::Font* font)
{
    this->color = sf::Color(255, 255, 255, 255);
    this->hoverColor = sf::Color(128, 128, 128, 255);

    textObj.setFont(*font);
    textObj.setString(text);
    textObj.setCharacterSize(24);
    textObj.setFillColor(color);
    textObj.setPosition(sf::Vector2f(x, y));
}

// Checking if the user has pressed and/or hovered over the button
bool TextButton::hasPressed(MouseHandler* mouse)
{
    // Text rectangle
    sf::FloatRect textRect = textObj.getGlobalBounds();
    // Is the mouse pointer inside the text rectangle
    bool hoveringOver = textRect.contains(sf::Vector2f(mouse->getX(), mouse->getY()));

    // Change the color if the user's hovering over it
    if (hoveringOver) textObj.setFillColor(hoverColor);
    else textObj.setFillColor(color);

    return hoveringOver && mouse->isLeftClicked();
}

void TextButton::render()
{
    window->draw(textObj);
}
