#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "MouseHandler.h"

using namespace std;

// Window properties
const int WIDTH = 1024;
const int HEIGHT = 768;
const int MAX_FPS = 60;

// Grid dimensions (in number of cells)
const int GRID_WIDTH = 24;
const int GRID_HEIGHT = 16;

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

	Grid grid(&window, GRID_WIDTH, GRID_HEIGHT); // Creating the grid
	MouseHandler mouse(&window, &grid); // User mouse input

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		window.clear();

		// Updating the grid
		grid.tick();
		// Drawing the grid
		grid.render();

		// Updating the mouse state; checking for user input
		mouse.tick();

		window.display();

		dt = deltaClock.restart();
	}

	return 0;
}