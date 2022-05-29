#pragma once
#include <SFML/Graphics.hpp>
#include "MouseHandler.h"

class Grid
{
private:
	int width, height; // Grid dimensions
	bool* cells; // Array of cells
	bool* cellsCopy; // Copy of the original cells
	sf::RenderWindow* window; // Main window
	MouseHandler* mouse; // User's mouse

	unsigned generationCount = 0; // Current generation of cells
	bool paused = false; // Is the execution paused

	sf::RectangleShape cellShape; // The cell which we render
	sf::Clock timer; // Timer which determines when we update every cell
	int updateInvervalMs; // How much time is needed to pass to update the grid (in ms)

	// Private methods
	void init();
	void renderGridOutline();
	int getNeighborCount(int x, int y);
	void transferNewCells();
	void checkUserInput();
public:
	// Grid constants
	static const int CELL_SIZE = 24; // Cell size

	Grid(sf::RenderWindow* window, MouseHandler* mouse, int width, int height);
	void setCell(int x, int y, bool state);
	bool getCell(int x, int y);

	void setCopiedCell(int x, int y, bool state);
	bool getCopiedCell(int x, int y);

	void setUpdateInterval(int ms);
	int getUpdateInterval();

	void toggle(bool paused);
	bool isPaused();

	unsigned getGeneration();

	void tick();
	void render();
};

