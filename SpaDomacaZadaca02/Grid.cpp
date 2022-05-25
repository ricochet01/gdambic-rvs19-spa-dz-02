#include "Grid.h"
#include <iostream>

using namespace std;

Grid::Grid(sf::RenderWindow* window, int width, int height)
{
    this->window = window;
    this->width = width;
    this->height = height;

    this->cells = new bool[width * height];
    // Creating a copy of the original grid to check for cells before updating them
    this->cellsCopy = new bool[width * height];

    init();
}

void Grid::init()
{
    // Creating a cell shape object
    this->cellShape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cellShape.setFillColor(sf::Color::White);
    
    // Setting the default values of the array to be false
    int len = width * height; // Cell array length
    for (int i = 0; i < len; i++) {
        cells[i] = false;
        cellsCopy[i] = false;
    }

    this->updateInvervalMs = 2000.f; // (n / 1000) seconds
}

void Grid::renderGridOutline()
{
    sf::RectangleShape line;
    line.setFillColor(sf::Color(128, 128, 128, 255));
    line.setSize(sf::Vector2f(width << CELL_FACTOR, 1));

    for (int y = 0; y <= height; y++) {
        line.setPosition(sf::Vector2f(0, y << CELL_FACTOR));

        window->draw(line);
    }

    line.setSize(sf::Vector2f(1, height << CELL_FACTOR));

    for (int x = 0; x <= width; x++) {
        line.setPosition(sf::Vector2f(x << CELL_FACTOR, 0));

        window->draw(line);
    }
}

int Grid::getNeighborCount(int xc, int yc)
{
    int numberOfNeighbors = 0;

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            if (x == 0 && y == 0) continue; // We don't check it's own cell

            if (getCell(xc + x, yc + y)) numberOfNeighbors++;
        }
    }

    return numberOfNeighbors;
}

void Grid::transferNewCells()
{
    for (int i = 0; i < width * height; i++) {
        cells[i] = cellsCopy[i];
    }
}

// Creates the cell in the copy array before updating the entire grid
void Grid::setCell(int x, int y, bool state)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    cells[x + y * width] = state;
}

bool Grid::getCell(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return cells[x + y * width];
}

void Grid::setCopiedCell(int x, int y, bool state)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    cellsCopy[x + y * width] = state;
}

bool Grid::getCopiedCell(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return cellsCopy[x + y * width];
}

void Grid::tick()
{
    sf::Time elapsed = timer.getElapsedTime();

    // Check if the correct time has passed
    if (elapsed.asMilliseconds() >= updateInvervalMs) { // Update the cells
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bool cell = getCell(x, y);
                int neighborCount = getNeighborCount(x, y);

                if (cell) { // Is the cell alive
                    if (neighborCount < 2 || neighborCount > 3) { // We kill the cell
                        setCopiedCell(x, y, false);
                    }
                }
                else { // Is the cell dead
                    if (neighborCount == 3) { // Create a new cell if it has exactly 3 neighbors
                        setCopiedCell(x, y, true);
                    }
                }
            }
        }

        generationCount++;
        transferNewCells();
        timer.restart(); // Reset the timer back to 0
    }
}

void Grid::render()
{
    renderGridOutline();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bool cell = getCopiedCell(x, y);

            if (cell) { // Draw the cell if it's alive
                cellShape.setPosition(sf::Vector2f(x << CELL_FACTOR, y << CELL_FACTOR));
                window->draw(cellShape);
            } // Otherwise, it will just be a black cell
        }
    }
}
