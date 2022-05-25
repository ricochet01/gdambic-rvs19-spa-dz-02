#pragma once
#include <SFML/Graphics.hpp>

class MouseHandler
{
private:
	int x, y; // Mouse position
	bool left, right; // Are the mouse buttons pressed down
	sf::RenderWindow* window;
public:
	MouseHandler(sf::RenderWindow* window);
	void tick();

	bool isLeftPressed();
	bool isRightPressed();
	bool isAnyButtonPressed();

	int getX();
	int getY();
};

