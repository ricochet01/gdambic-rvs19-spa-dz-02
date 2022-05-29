#include "Label.h"

Label::Label(sf::RenderWindow* window, sf::Font* font, string text, int x, int y)
{
	this->window = window;
	this->x = x;
	this->y = y;
	setText(text);
	init(font);
}

void Label::init(sf::Font* font)
{
	this->color = sf::Color(255, 255, 255, 255);
	textObj.setFont(*font);
	textObj.setPosition(sf::Vector2f(x, y));
	textObj.setFillColor(color);
	textObj.setCharacterSize(24);
}

void Label::setText(string text) {
	this->text = text;
	textObj.setString(text);
}

void Label::render()
{
	window->draw(textObj);
}
