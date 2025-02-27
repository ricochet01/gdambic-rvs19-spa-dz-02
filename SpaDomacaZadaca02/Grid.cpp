#include "Grid.h"
#include <iostream>
#include "MouseHandler.h"
#include "WindowUtils.h"

using namespace std;

Grid::Grid(sf::RenderWindow* window, MouseHandler* mouse)
{
    this->window = window;
    this->mouse = mouse;
    this->cellSize = CELL_SIZES[cellSizeIndex];

    this->width = 768 / cellSize;
    this->height = width - 1;

    this->cells = new bool[width * height];
    // Creating a copy of the original grid to check for cells before updating them
    this->cellsCopy = new bool[width * height];

    init();
}

Grid::~Grid()
{
    delete[] cells, cellsCopy;
}

void Grid::init()
{
    // Creating a cell shape object
    this->cellShape = sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
    cellShape.setFillColor(sf::Color::White);

    reset();

    this->updateInvervalMs = 500; // (n / 1000) seconds
}

void Grid::renderGridOutline()
{
    sf::RectangleShape line;
    line.setFillColor(sf::Color(128, 128, 128, 255));
    line.setSize(sf::Vector2f(width * cellSize, 1));

    for (int y = 0; y <= height; y++) {
        line.setPosition(sf::Vector2f(0, y * cellSize));

        window->draw(line);
    }

    line.setSize(sf::Vector2f(1, height * cellSize));

    for (int x = 0; x <= width; x++) {
        line.setPosition(sf::Vector2f(x * cellSize, 0));

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

void Grid::checkUserInput()
{
    if (!mouse->isAnyButtonPressed() || !WindowUtils::isWindowFocused()) return;

    int mx = mouse->getX();
    int my = mouse->getY();

    // Getting the cell x and y
    int cellX = mx / cellSize;
    int cellY = my / cellSize;

    if (mouse->isLeftPressed()) {
        setCell(cellX, cellY, true);
        setCopiedCell(cellX, cellY, true);
    }
    else if (mouse->isRightPressed()) {
        setCell(cellX, cellY, false);
        setCopiedCell(cellX, cellY, false);
    }
}

void Grid::updateDimensions(int cellSize)
{
    int oldWidth = this->width;
    int oldHeight = this->height;

    // Updating the dimensions
    this->cellSize = cellSize;
    this->width = 768 / cellSize;
    this->height = width - 1;

    this->cellShape.setSize(sf::Vector2f(cellSize, cellSize));

    // New grid with a different cell size
    bool* newArray = new bool[width * height];
    bool* newArrayCopy = new bool[width * height];

    for (unsigned i = 0; i < width * height; i++) {
        newArray[i] = false;
        newArrayCopy[i] = false;
    }

    for (int y = 0; y < oldHeight; y++) {
        for (int x = 0; x < oldWidth; x++) {
            if (x >= width || y >= height) continue;

            bool oldCell = cells[x + y * oldWidth];
            bool oldCellCopy = cellsCopy[x + y * oldWidth];

            newArray[x + y * width] = oldCell;
            newArrayCopy[x + y * width] = oldCellCopy;
        }
    }

    // Copying the arrays
    this->cells = new bool[width * height];
    this->cellsCopy = new bool[width * height];

    for (int i = 0; i < width * height; i++) {
        cells[i] = newArray[i];
        cellsCopy[i] = newArrayCopy[i];
    }

    delete[] newArray, newArrayCopy;
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

void Grid::setUpdateInterval(int ms)
{
    this->updateInvervalMs = ms;
}

int Grid::getUpdateInterval()
{
    return updateInvervalMs;
}

void Grid::toggle(bool paused)
{
    this->paused = paused;
}

bool Grid::isPaused()
{
    return paused;
}

unsigned Grid::getGeneration()
{
    return generationCount;
}

int Grid::getCellSize()
{
    return cellSize;
}

void Grid::increaseCellSize()
{
    if (cellSizeIndex == NUMBER_OF_SIZES - 1) return;
    cellSizeIndex++;
    this->updateDimensions(CELL_SIZES[cellSizeIndex]);
}

void Grid::decreaseCellSize()
{
    if (cellSizeIndex == 0) return;
    cellSizeIndex--;
    this->updateDimensions(CELL_SIZES[cellSizeIndex]);
}

void Grid::reset()
{
    this->generationCount = 0;

    // Setting the default values of the array to be false
    int len = width * height; // Cell array length
    for (int i = 0; i < len; i++) {
        cells[i] = false;
        cellsCopy[i] = false;
    }
}

void Grid::tick()
{
    checkUserInput();

    sf::Time elapsed = timer.getElapsedTime();

    // Check if the correct time has passed
    if (elapsed.asMilliseconds() >= updateInvervalMs && !paused) { // Update the cells
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
                cellShape.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
                window->draw(cellShape);
            } // Otherwise, it will just be a black cell
        }
    }
}
