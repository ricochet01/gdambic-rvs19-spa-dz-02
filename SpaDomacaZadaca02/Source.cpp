#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "Grid.h"
#include "MouseHandler.h"
#include "btn/TextButton.h"
#include "btn/SpriteButton.h"

using namespace std;

// Window properties
const int WIDTH = 1024;
const int HEIGHT = 768;
const int MAX_FPS = 60;

// Grid dimensions (in number of cells)
const int GRID_WIDTH = 24;
const int GRID_HEIGHT = 20;

// Window title
const string TITLE = "Game of Life";
// Disables resizing and adds a titlebar
const int WINDOW_SETTINGS = sf::Style::Titlebar | sf::Style::Close;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), TITLE, WINDOW_SETTINGS);
	window.setFramerateLimit(MAX_FPS);

	sf::Clock deltaClock; // Delta time used for normalizing animations
	sf::Time dt = deltaClock.restart(); // (making sure it's consistent at every framerate)

	sf::Font font; // Font
	if (!font.loadFromFile("font/consolaz.ttf")) {
		cout << "Error while opening font!" << endl;
	}

	MouseHandler mouse(&window); // User mouse input
	Grid grid(&window, &mouse, GRID_WIDTH, GRID_HEIGHT); // Creating the grid

	TextButton btn(&window, &font, "Hello world!", WIDTH - 224, 32);
	SpriteButton pause(&window, "gfx/pause.png", WIDTH - 200, 80);
	SpriteButton resume(&window, "gfx/resume.png", WIDTH - 200, 80);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonReleased:
				// Left click only counts when we release the left mouse button
				if (event.mouseButton.button == sf::Mouse::Left) {
					mouse.setLeftClick(true);
				}
				break;
			}
		}

		// Refreshing the window
		window.clear();

		// Updating the grid
		grid.tick();
		// Drawing the grid
		grid.render();

		// Drawing the button
		btn.render();
		pause.render();

		if (btn.hasPressed(&mouse)) {
			cout << "pressed button" << endl;
		}
		if (pause.hasPressed(&mouse)) {
			cout << "pressed pause!" << endl;
		}

		// Updating the mouse state; checking for user input
		mouse.tick();

		// Showing the graphics
		window.display();

		dt = deltaClock.restart();
	}

	return 0;
}