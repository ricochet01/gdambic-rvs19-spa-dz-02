#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "Grid.h"
#include "MouseHandler.h"
#include "btn/TextButton.h"
#include "btn/SpriteButton.h"
#include "ui/Label.h"

using namespace std;

// Window properties
const int WIDTH = 1024;
const int HEIGHT = 768;
const int MAX_FPS = 60;

// Grid dimensions (in number of cells)
const int GRID_WIDTH = 32;
const int GRID_HEIGHT = 30;

// Window title
const string TITLE = "Game of Life";
// Disables resizing and adds a titlebar
const int WINDOW_SETTINGS = sf::Style::Titlebar | sf::Style::Close;

const short NUMBER_OF_INTERVALS = 5;
const int INTERVAL_UPDATES[NUMBER_OF_INTERVALS] = {100, 250, 500, 1000, 2000};

void setInterval(Label& intervalLabel, int index, Grid& grid) {
	int interval = INTERVAL_UPDATES[index];
	intervalLabel.setText("Update: " + to_string(interval) + " ms");
	grid.setUpdateInterval(interval);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), TITLE, WINDOW_SETTINGS);
	window.setFramerateLimit(MAX_FPS);

	sf::Clock deltaClock; // Delta time used for normalizing animations
	sf::Time dt = deltaClock.restart(); // (making sure it's consistent at every framerate)

	sf::Font font; // Font
	if (!font.loadFromFile("font/consolaz.ttf")) { // Loading the font from the file
		cout << "Error while opening font!" << endl;
		return 1;
	}

	MouseHandler mouse(&window); // User mouse input
	Grid grid(&window, &mouse, GRID_WIDTH, GRID_HEIGHT); // Creating the grid

	// Buttons to control the flow
	SpriteButton pause(&window, "gfx/pause.png", WIDTH - 168, 80);
	SpriteButton resume(&window, "gfx/resume.png", WIDTH - 168, 80);
	SpriteButton resetGrid(&window, "gfx/reset1.png", WIDTH - 128, 80);

	SpriteButton timeForwards(&window, "gfx/advance-forward.png", WIDTH - 85, 80);
	SpriteButton timeBackwards(&window, "gfx/advance-backwards.png", WIDTH - 210, 80);

	int updateIntervalIndex = NUMBER_OF_INTERVALS - 1;
	unsigned previousGeneration = grid.getGeneration();

	// Displays for important info
	Label mainLabel(&window, &font, "Controls:", WIDTH - 195, 32);
	Label genLabel(&window, &font, "Generation: " + to_string(grid.getGeneration()), WIDTH - 250, 135);
	Label intervalLabel(&window, &font, "Update: " + to_string(INTERVAL_UPDATES[updateIntervalIndex]) + " ms",
		WIDTH - 250, 165);
	Label ctrlLabel(&window, &font, "Left click to make\na cell alive,\nright click to\nkill it.", WIDTH - 250, 240);

	setInterval(intervalLabel, updateIntervalIndex, grid);

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
			case sf::Event::LostFocus:
				grid.toggle(true);
				break;
			}
		}

		// Refreshing the window
		window.clear();

		// Updating the grid
		grid.tick();
		// Drawing the grid
		grid.render();

		// Drawing the buttons
		SpriteButton* executionBtn = grid.isPaused() ? &resume : &pause; // Which button to use
		executionBtn->render();

		timeForwards.render();
		timeBackwards.render();
		resetGrid.render();

		/* Button press checks */
		if (executionBtn->hasPressed(&mouse)) {
			grid.toggle(!grid.isPaused());
		}

		// Time execution delay increase
		if (timeForwards.hasPressed(&mouse)) {
			if (updateIntervalIndex < NUMBER_OF_INTERVALS - 1) {
				updateIntervalIndex++;
				setInterval(intervalLabel, updateIntervalIndex, grid);
			}
		}

		// Time execution delay decrease
		if (timeBackwards.hasPressed(&mouse)) {
			if (updateIntervalIndex > 0) {
				updateIntervalIndex--;
				setInterval(intervalLabel, updateIntervalIndex, grid);
			}
		}

		if (resetGrid.hasPressed(&mouse)) {
			grid.reset();
		}

		// Updating the generation display label
		if (previousGeneration != grid.getGeneration()) {
			previousGeneration = grid.getGeneration();
			genLabel.setText("Generation: " + to_string(previousGeneration));
		}

		// Drawing the text labels
		mainLabel.render();
		genLabel.render();
		intervalLabel.render();
		ctrlLabel.render();

		// Updating the mouse state; checking for user input
		mouse.tick();

		// Showing the graphics
		window.display();

		dt = deltaClock.restart();
	}

	return 0;
}