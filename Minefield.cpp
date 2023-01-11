/*
    Filename:       Minefield.cpp
    Author:         Adrian Padin (padin.adrian@gmail.com)
    Initial date:   2023-01-10
    Description:    Minefield class implementation
*/

/* ===== Includes ===== */
#include <cstdio>
#include <stdint.h>
#include <random>
#include "Minefield.hpp"

/** Constructor */
Minefield::Minefield(size_t width, size_t height, size_t numberOfMines) :
    mines(width, height),
    flags(width, height),
    visibility(width, height),
    numberOfMines(numberOfMines),
    flagsRemaining(numberOfMines)
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
        
        if (!CoordinatesListContainsQ(placed, mine)) {
            placed.push_back(mine);
            this->mines.set(mine, -1) ;     // Lay a mine
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
    // Input checking
    if (selected.x > this->mines.width() ||
        selected.y > this->mines.height()) {
        return 0;
    }
    
    // If the tile is flagged, ignore and do not update.
    if (this->flags.at(selected)) {
        return 'F';
    }
    
    // Check the true value of the selected tile.
    int8_t result = this->mines.at(selected);
    
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
            this->visibility.set(selected, 1);
            break;
        }
    }
    
    return result;
}

/** Place a flag to indicate a potential mine. */
int8_t Minefield::PlaceFlag(Coordinates selected)
{
    // Place a flag on the following conditions:
    // 1. Have not yet reached the limit of flags placed.
    // 2. Flag can only be placed on a hidden tile.
    // 3. Flag is not already placed at the selected tile.
    if ((this->flagsRemaining > 0) &&
        (this->visibility.at(selected)) == 0 &&
        (this->flags.at(selected)) == 0
    ) {
        this->flags.set(selected, 1);   // Place a flag.
        this->flagsRemaining -= 1;      // Deduct the number of remaining flags.
    }
    return 0;
}

/** Clear a previously-placed flag. */
int8_t Minefield::ClearFlag(Coordinates selected)
{
    // Only clear the flag if a flag is already placed.
    if (this->flags.at(selected)) {
        this->flags.set(selected, 0);   // Clear the flag.
        this->flagsRemaining += 1;      // Increment the number of remaining flags.
    }
    return 0;
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
    return (-1 == this->mines.at(selected));
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
        if (CoordinatesListContainsQ(visited, current)) {
            continue;
        }
        
        // We haven't seen these coordinates before - add them to the visited list.
        visited.push_back(current);
        
        // Reveal this tile.
        this->visibility.set(current, 1);
        
        // If the tile had a flag, clear it.
        this->ClearFlag(current);
        
        // If this tile is also a blank spot, find all neighbors and add them to the stack.
        // TODO: Check if we have seen the coordinates BEFORE adding to the stack!
        if (0 == this->mines.at(current)) {
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
