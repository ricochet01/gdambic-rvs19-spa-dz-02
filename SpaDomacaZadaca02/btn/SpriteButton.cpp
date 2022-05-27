#include "SpriteButton.h"

SpriteButton::SpriteButton(sf::RenderWindow* window, string spritePath, int x, int y)
{
    this->window = window;
    this->x = x;
    this->y = y;
    init(spritePath);
}

void SpriteButton::init(string spritePath)
{
    texture.loadFromFile(spritePath);
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(x, y));

    this->color = sf::Color(255, 255, 255, 255);
    this->hoverColor = sf::Color(128, 128, 128, 255);
}

void SpriteButton::render()
{
    window->draw(sprite);
}

bool SpriteButton::hasPressed(MouseHandler* mouse)
{
    // Sprite rectangle
    sf::FloatRect textRect = sprite.getGlobalBounds();
    // Is the mouse pointer inside the sprite rectangle
    bool hoveringOver = textRect.contains(sf::Vector2f(mouse->getX(), mouse->getY()));

    // Change the color if the user's hovering over it
    if (hoveringOver) sprite.setColor(hoverColor);
    else sprite.setColor(color);

    return hoveringOver && mouse->isLeftClicked();
}
