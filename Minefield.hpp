/*
    Filename:       Minefield.hpp
    Author:         Adrian Padin (padin.adrian@gmail.com)
    Initial date:   2023-01-10
    Description:    Minefield class description
*/

/* ===== Includes ===== */
#include <stdint.h>
#include "Grid.hpp"

/* ===== Classes ===== */

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
