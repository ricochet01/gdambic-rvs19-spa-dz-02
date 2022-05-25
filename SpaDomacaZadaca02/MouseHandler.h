#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.h"

class MouseHandler
{
private:
	int x, y; // Mouse position
	sf::RenderWindow* window;
	Grid* grid;
public:
	MouseHandler(sf::RenderWindow* window, Grid* grid);
	void tick();
	int getX();
	int getY();
};

