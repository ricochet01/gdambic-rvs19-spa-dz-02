#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../MouseHandler.h"

using namespace std;

class SpriteButton
{
private:
	int x, y;
	sf::Color color, hoverColor;
	sf::RenderWindow* window;

	sf::Texture texture; // The image itself
	sf::Sprite sprite; // Object which can be used to draw the texture
	void init(string spritePath);
public:
	SpriteButton(sf::RenderWindow* window, string spritePath, int x, int y);
	void render();
	bool hasPressed(MouseHandler* mouse);
};

