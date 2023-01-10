/**
    Filename:       minesweeper.cpp
    Author:         padin.adrian@gmail.com
    Date:           2022-01-07
    Description:    Minesweeper clone
*/

/* ===== Includes ===== */

#include <cstdio>
#include <stdint.h>
#include <vector>
#include <random>
#include <iostream>

/* ===== Typedefs ===== */


/* ===== Classes ===== */

/**
 * Simple coordinates pair
 */
struct Coordinates {
    uint8_t x;
    uint8_t y;
    // Constructor - needed for vector::emplace_back
    Coordinates(uint8_t x, uint8_t y) : x(x), y(y) {}
    
    // Get coordinates relative to this one.
    Coordinates Top()           { return Coordinates(x    , y + 1); }
    Coordinates TopRight()      { return Coordinates(x + 1, y + 1); }
    Coordinates Right()         { return Coordinates(x + 1, y    ); }
    Coordinates BottomRight()   { return Coordinates(x + 1, y - 1); }
    Coordinates Bottom()        { return Coordinates(x    , y - 1); }
    Coordinates BottomLeft()    { return Coordinates(x - 1, y - 1); }
    Coordinates Left()          { return Coordinates(x - 1, y    ); }
    Coordinates TopLeft()       { return Coordinates(x - 1, y + 1); }
};

/** Returns true if the cooridn */
bool ListContainsQ(const std::vector<Coordinates> coordinates, const Coordinates selected)
{
    for (auto it = coordinates.begin(); it != coordinates.end(); ++it) {
        if (it->x == selected.x && it->y == selected.y) {
            return true;
        }
    }
    return false;
}

/**
 * 2D array with built-in value getter and setter functions.
 */
class Grid {
public:
    Grid(size_t w, size_t h) : w(w), h(h), grid(w * h, 0) {}
    
    size_t width() const { return w; }
    
    size_t height() const { return h; }
    
    int8_t at(size_t x, size_t y) const {
        size_t index = x + (y * this->h);
        return grid[index];
    }
    
    int8_t at(const Coordinates coords) const {
        return this->at(coords.x, coords.y);
    }
    
    int8_t set(size_t x, size_t y, int8_t val) {
        size_t index = x + (y * this->h);
        grid[index] = val;
        return val;
    }
    
    int8_t set(const Coordinates coords, int8_t val) {
        return this->set(coords.x, coords.y, val);
    }

private:
    size_t w;
    size_t h;
    std::vector<int8_t> grid;
};

/**
 * Represents the minefield - where the mines are and which tiles are visible.
 */
class Minefield {
public:
    Minefield(size_t width, size_t height, size_t numberOfMines);
    void Initialize();
    void Reveal();
    int8_t Update(Coordinates selected);
    bool IsCleared() const;

    const Grid& Mines() const { return mines; }
    const Grid& Visibility() const { return visibility; }

private:
    void floodFill(Coordinates start);
    bool isMine(Coordinates selected);

private:
    Grid mines;
    Grid visibility;
    size_t numberOfMines;
};

/** Constructor */
Minefield::Minefield(size_t width, size_t height, size_t numberOfMines) :
    mines(width, height),
    visibility(width, height),
    numberOfMines(numberOfMines)
{
    Initialize();
}

/** Initialize the minefield by setting the mines. */
void Minefield::Initialize()
{
    const size_t width = this->mines.width();
    const size_t height = this->mines.height();
    
    // Random number generator
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> xrange(0, width - 1);
    std::uniform_int_distribution<int> yrange(0, height - 1);
    
    std::vector<Coordinates> placed;
    
    // Randomly generate some mines within the given range.
    while (placed.size() < numberOfMines) {
        
        Coordinates mine(
            xrange(generator),
            yrange(generator)
        );
        
        if (!ListContainsQ(placed, mine)) {
            placed.push_back(mine);
            this->mines.set(mine, -1) ;     // Lay a mine
            
            // Debugging
            printf("Laying a mine at %d %d\n", mine.x, mine.y);
            fflush(stdout);
        }
    }
    
    // Set numbers based on adjacent mines
    const size_t leftBound = 0;
    const size_t rightBound = this->mines.width() - 1;
    const size_t bottomBound = 0;
    const size_t topBound = this->mines.height() - 1;
    
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            
            int8_t counter = 0;
            Coordinates current(x, y);
            
            // Don't overwrite the mine spaces.
            if (!this->isMine(current)) {
                if (x > leftBound) {
                    if (this->isMine(current.Left())) { counter++; };
                    if (y > bottomBound) {
                        if (this->isMine(current.BottomLeft())) { counter++; };
                    }
                    if (current.y < topBound) {
                        if (this->isMine(current.TopLeft())) { counter++; };
                    }
                }
                if (x < rightBound) {
                    if (this->isMine(current.Right())) { counter++; };
                    if (y > bottomBound) {
                        if (this->isMine(current.BottomRight())) { counter++; };
                    }
                    if (y < topBound) {
                        if (this->isMine(current.TopRight())) { counter++; };
                    }
                }
                if (y > bottomBound) {
                    if (this->isMine(current.Bottom())) { counter++; };
                }
                if (y < topBound) {
                    if (this->isMine(current.Top())) { counter++; };
                }
                this->mines.set(current, counter);
            }
        }
    }
}

/** Show all mines (i.e. set visibility to true for all tiles). */
void Minefield::Reveal()
{
    const size_t width = this->visibility.width();
    const size_t height = this->visibility.height();
    
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            this->visibility.set(x, y, 1);
        }
    }
}

/** Update the minefield visibility based on the selected tile.
    Returns the number of bombs adjacent to the tile.
    Returns -1 if the tile is a bomb.
*/
int8_t Minefield::Update(Coordinates selected)
{
    int8_t result = this->mines.at(selected.x, selected.y);
    
    // Debugging
    printf("Update selected: %d %d\n", selected.x, selected.y);
    printf("Update result: %d\n", result);
    fflush(stdout);
    
    switch (result) {
        // Bomb - game over!
        case -1: {
            this->Reveal();
            break;
        }
        
        // Blank spot - fill in surrounding spots.
        case 0: {
            this->floodFill(selected);
            break;
        }
        
        // Normal spot - just reveal the number.
        default: {
            this->visibility.set(selected.x, selected.y, 1);
            break;
        }
    }
    
    return result;
}

/** Return true if the minefield is completely cleared. */
bool Minefield::IsCleared() const
{
    // Add up the number of invisibile tiles.
    size_t counter = 0;
    const size_t width = this->visibility.width();
    const size_t height = this->visibility.height();
    
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            counter += (1 - visibility.at(x, y));
        }
    }
    
    return (counter == this->numberOfMines);
}

/** Check if a particular tile is a mine. */
bool Minefield::isMine(Coordinates selected)
{
    // Debugging
    // printf("Checking for mine at %d %d\n", selected.x, selected.y);
    // printf("result: %d\n", this->mines.at(selected.x, selected.y));
    // fflush(stdout);
    
    return (-1 == this->mines.at(selected.x, selected.y));
}

/** Reveal all adjacent blank spaces when a blank space is revealed. */
void Minefield::floodFill(Coordinates start) {
    // Depth-first search using tail-recursive pattern
    std::vector<Coordinates> visited;
    std::vector<Coordinates> remaining;
    Coordinates current(0, 0);
    
    const size_t leftBound = 0;
    const size_t rightBound = this->mines.width() - 1;
    const size_t bottomBound = 0;
    const size_t topBound = this->mines.height() - 1;
    
    // Starting point
    remaining.push_back(start);
    
    // Keep going until the stack is empty.
    while (!remaining.empty()) {
        // Take the top coordinates off the stack.
        current = remaining.back();
        remaining.pop_back();
        
        // Check if we have seen these coordinates before.
        if (ListContainsQ(visited, current)) {
            continue;
        }
        
        // We haven't seen these coordinates before - add them to the visited list.
        visited.push_back(current);
        
        // Reveal this tile.
        this->visibility.set(current.x, current.y, 1);
        
        // If this tile is also a blank spot, find all neighbors and add them to the stack.
        // TODO: Check if we have seen the coordinates BEFORE adding to the stack!
        if (0 == this->mines.at(current.x, current.y)) {
            if (current.x > leftBound) {
                remaining.emplace_back(current.Left());
                if (current.y > bottomBound) {
                    remaining.emplace_back(current.BottomLeft());
                }
                if (current.y < topBound) {
                    remaining.emplace_back(current.TopLeft());
                }
            }
            if (current.x < rightBound) {
                remaining.emplace_back(current.Right());
                if (current.y > bottomBound) {
                    remaining.emplace_back(current.BottomRight());
                }
                if (current.y < topBound) {
                    remaining.emplace_back(current.TopRight());
                }
            }
            if (current.y > bottomBound) {
                remaining.emplace_back(current.Bottom());
            }
            if (current.y < topBound) {
                remaining.emplace_back(current.Top());
            }
        }
    }
}

/* ===== Functions ===== */

/** Display the minefield. */
void DisplayMinefield(const Minefield& minefield)
{
    const Grid& mines = minefield.Mines();
    const Grid& visibility = minefield.Visibility();
    
    const size_t width = mines.width();
    const size_t height = mines.height();
    const size_t rowSeparatorWidth = (4 * width) + 1;
    
    char printChar;
    int8_t tileValue = 0;
    int8_t tileVisible = 0;
    
    // Debugging
    // for (size_t x = 0; x < width; ++x) {
    //     for (size_t y = 0; y < height; ++y) {
    //         printf("Tile value at %d %d: %d\n", x, y, mines.at(x, y));
    //     }
    // }
    // fflush(stdout);
    
    // Print each row
    // Ex)
    // -------------
    // | 1 | 2 |   |
    for (size_t y = 0; y < height; ++y) {
        
        // Print top bar
        for (size_t i = 0; i < rowSeparatorWidth; ++i) { printf("-"); }
        printf("\n");
        
        // Print each column within a row
        for (size_t x = 0; x < width; ++x) {
            
            tileVisible = visibility.at(x, y);
            
            if (tileVisible) {
                // The tile is visible - show the contents.
                tileValue = mines.at(x, y);
                
                if (tileValue == 0) { printChar = ' '; }
                else if (tileValue > 0 && tileValue < 9) { printChar = '0' + tileValue; }
                else if (tileValue == -1) { printChar = '*'; }
                else { printChar = '?'; }
            }
            else {
                // The tile is NOT visible - show hidden as "-".
                printChar = '-';
            }
            
            printf("| %c ", printChar);
        }
        printf("|\n");
    }
    
    // Print bottom bar
    for (size_t i = 0; i < rowSeparatorWidth; ++i) { printf("-"); }
    printf("\n\n");
    fflush(stdout);
}

/** Play the game */
void PlayGame() {
    
    // TODO: Pick size and number of mines
    // For now we just do the equivalent of "Beginner" mode.
    // From https://datagenetics.com/blog/june12012/index.html
    // Beginner:        9   9   10
    // Intermediate:    16  16  40
    // Expert:          30  16  99
    Minefield minefield(9, 9, 10);
    
    int x = 0;
    int y = 0;
    bool gameWon = false;
    bool gameLost = false;
    int8_t tileValue = 0;
    
    // Debugging
    // minefield.Reveal();
    
    while (!(gameWon || gameLost)) {
        // Show the minefield and ask the player to pick a tile.
        DisplayMinefield(minefield);
        printf("Pick a tile. Input coordinates as (x y): ");
        fflush(stdout);
        scanf("%d %d", &x, &y);
        
        tileValue = minefield.Update(Coordinates(x, y));
        
        // Game over if the tile is a bomb.
        gameLost = (tileValue == -1);
        gameWon = minefield.IsCleared();
        
        printf("Game won: %d\n", gameWon);
        printf("Game lost: %d\n", gameLost);
        printf("Game won or lost: %d\n", (gameWon || gameLost));
        printf("Game not won or lost: %d\n", !(gameWon || gameLost));
        fflush(stdout);
    }
    
    printf("Game won: %d\n", gameWon);
    printf("Game lost: %d\n", gameLost);
    
    // Show the minefield one last time and tell the player if they won or lost.
    DisplayMinefield(minefield);
    if (gameWon) {
        printf("You win!\n");
    }
    else {
        printf("Game over!\n");
    }
    fflush(stdout);
}

/** Main */
int main() {
    std::cout << "Hello" << std::endl;
    printf("Welcome to Minesweeper!\n");
    fflush(stdout);
    
    PlayGame();
    
    // Debugging
    // Minefield minefield(9, 9);
    // minefield.Reveal();
    // DisplayMinefield(minefield);
    
    // TODO: prompt to play again
}