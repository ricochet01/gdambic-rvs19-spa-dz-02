#include "MouseHandler.h"
#include <SFML/Graphics.hpp>

MouseHandler::MouseHandler(sf::RenderWindow* window)
{
	this->window = window;

	// Default values
	this->x = 0;
	this->y = 0;
	this->left = false;
	this->right = false;
}

void MouseHandler::tick()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window); // Mouse position relative to the window
	// The x and y coordinates of the mouse
	this->x = mousePosition.x;
	this->y = mousePosition.y;

	this->left = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	this->right = sf::Mouse::isButtonPressed(sf::Mouse::Right);

	this->leftClick = false;
}

bool MouseHandler::isLeftPressed()
{
	return left;
}

bool MouseHandler::isRightPressed()
{
	return right;
}

bool MouseHandler::isAnyButtonPressed()
{
	return left || right;
}

bool MouseHandler::isLeftClicked()
{
	return leftClick;
}

void MouseHandler::setLeftClick(bool state)
{
	this->leftClick = state;
}

int MouseHandler::getX()
{
	return x;
}

int MouseHandler::getY()
{
	return y;
}
