#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "Grid.h"
#include "WindowUtils.h"
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
// int GRID_WIDTH = 48;
// int GRID_HEIGHT = 47;

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

void updateGeneration(Label& genLabel, int gen) {
	string genText = gen > 1000000 ? "a lot" : to_string(gen);
	genLabel.setText("Generation: " + to_string(gen));
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
	Grid grid(&window, &mouse); // Creating the grid

	// Buttons to control the flow
	SpriteButton pause(&window, "gfx/pause.png", WIDTH - 168, 80);
	SpriteButton resume(&window, "gfx/resume.png", WIDTH - 168, 80);
	SpriteButton resetGrid(&window, "gfx/reset1.png", WIDTH - 128, 80);

	SpriteButton cellIncrease(&window, "gfx/plus.png", WIDTH - 95, 80);
	SpriteButton cellDecrease(&window, "gfx/minus.png", WIDTH - 200, 80);

	SpriteButton timeForwards(&window, "gfx/advance-forward.png", WIDTH - 60, 80);
	SpriteButton timeBackwards(&window, "gfx/advance-backwards.png", WIDTH - 235, 80);

	int updateIntervalIndex = NUMBER_OF_INTERVALS - 1;
	unsigned previousGeneration = grid.getGeneration();

	// Displays for important info
	Label mainLabel(&window, &font, "Controls:", WIDTH - 195, 32);
	Label genLabel(&window, &font, "Generation: " + to_string(grid.getGeneration()), WIDTH - 250, 135);
	Label intervalLabel(&window, &font, "Update: " + to_string(INTERVAL_UPDATES[updateIntervalIndex]) + " ms",
		WIDTH - 250, 165);

	string labelText = "------------------\nLeft click to make\na cell alive,\nright click to\nkill it.";
	Label ctrlLabel(&window, &font, labelText, WIDTH - 250, 240);

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
			case sf::Event::GainedFocus:
				WindowUtils::setWindowFocus(true);
				break;
			case sf::Event::LostFocus:
				WindowUtils::setWindowFocus(false);
				grid.toggle(true); // Pause the simulation
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
		cellDecrease.render();
		cellIncrease.render();

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

		if (cellIncrease.hasPressed(&mouse)) {
			grid.increaseCellSize();
		}

		if (cellDecrease.hasPressed(&mouse)) {
			grid.decreaseCellSize();
		}

		// Updating the generation display label
		if (previousGeneration != grid.getGeneration()) {
			previousGeneration = grid.getGeneration();
			updateGeneration(genLabel, previousGeneration);
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