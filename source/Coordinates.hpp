/*
    Filename:       Coordinates.hpp
    Author:         Adrian Padin (padin.adrian@gmail.com)
    Initial date:   2023-01-10
    Description:    Coordinates class
*/

/* ===== Includes ===== */
#include <stdint.h>

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

/* ===== Functions ===== */

/** Returns true if the cooridn */
inline bool CoordinatesListContainsQ(const std::vector<Coordinates> coordinates, const Coordinates selected)
{
    for (auto it = coordinates.begin(); it != coordinates.end(); ++it) {
        if (it->x == selected.x && it->y == selected.y) {
            return true;
        }
    }
    return false;
}
