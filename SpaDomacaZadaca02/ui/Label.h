#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class Label
{
private:
	string text;
	int x, y;

	sf::RenderWindow* window;
	sf::Text textObj;
	sf::Color color;
	void init(sf::Font* font);
public:
	Label(sf::RenderWindow* window, sf::Font* font, string text, int x, int y);
	void setText(string text);
	void render();
};