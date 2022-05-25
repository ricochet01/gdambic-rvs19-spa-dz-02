#include "MouseHandler.h"
#include "Grid.h"
#include <SFML/Graphics.hpp>

MouseHandler::MouseHandler(sf::RenderWindow* window, Grid* grid)
{
	this->window = window;
	this->grid = grid;

	this->x = 0;
	this->y = 0;
}

void MouseHandler::tick()
{
	bool left = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool right = sf::Mouse::isButtonPressed(sf::Mouse::Right);

	if (!(left || right)) return;

	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window); // Mouse position relative to the window
	// The x and y coordinates of the mouse
	this->x = mousePosition.x;
	this->y = mousePosition.y;

	// Getting the cell x and y
	int cellX = x >> Grid::CELL_FACTOR;
	int cellY = y >> Grid::CELL_FACTOR;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		grid->setCell(cellX, cellY, true);
		grid->setCopiedCell(cellX, cellY, true);
	} else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		grid->setCell(cellX, cellY, false);
		grid->setCopiedCell(cellX, cellY, false);
	}
}

int MouseHandler::getX()
{
	return x;
}

int MouseHandler::getY()
{
	return y;
}
