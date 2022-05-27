#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../MouseHandler.h"

using namespace std;

class TextButton
{
private:
	string text;
	int x, y;
	sf::Color color, hoverColor;
	sf::RenderWindow* window;
	sf::Text textObj;

	void init(sf::Font* font);
public:
	TextButton(sf::RenderWindow* window, sf::Font* font, string text, int x, int y);
	void render();
	bool hasPressed(MouseHandler* mouse);
};

