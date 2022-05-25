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
	int generationCount = 0; // Current generation of cells

	sf::RectangleShape cellShape; // The cell which we render
	sf::Clock timer; // Timer which determines when we update every cell
	float updateInvervalMs; // How much time is needed to pass to update the grid (in ms)

	// Private methods
	void init();
	void renderGridOutline();
	int getNeighborCount(int x, int y);
	void transferNewCells();
	void checkUserInput();
public:
	// Grid constants
	static const int CELL_SIZE = 32; // Cell size
	static const int CELL_FACTOR = 5; // Used for faster calculations (2^5)

	Grid(sf::RenderWindow* window, MouseHandler* mouse, int width, int height);
	void setCell(int x, int y, bool state);
	bool getCell(int x, int y);
	void setCopiedCell(int x, int y, bool state);
	bool getCopiedCell(int x, int y);
	void tick();
	void render();
};

