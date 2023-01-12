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
    /** Constructor. */
    Minefield(size_t width, size_t height, size_t numberOfMines);

    /** Reset the game field. */
    void Initialize();

    /** Update the field based on the user's selected tile. */
    int8_t Update(Coordinates selected);

    /** Reveal all tiles on the board, including mines. */
    void Reveal();

    /** Set or unset flags. */
    int8_t PlaceFlag(Coordinates selected);
    int8_t ClearFlag(Coordinates selected);

    /** Check if the minefield is cleared (i.e. game is won). */
    bool IsCleared() const;

    /** Getters */
    const Grid& Mines() const { return mines; }
    const Grid& Flags() const { return flags; }
    const Grid& Visibility() const { return visibility; }
    size_t FlagsRemaining() const { return flagsRemaining; }

private:
    void floodFill(Coordinates start);
    bool isMine(Coordinates selected);

private:
    Grid mines;
    Grid flags;
    Grid visibility;
    size_t numberOfMines;
    size_t flagsRemaining;
};
