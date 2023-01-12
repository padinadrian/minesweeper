/*
    Filename:       Grid.hpp
    Author:         Adrian Padin (padin.adrian@gmail.com)
    Initial date:   2023-01-10
    Description:    Grid class
*/

/* ===== Includes ===== */
#include <stdint.h>
#include <vector>
#include "Coordinates.hpp"

/* ===== Classes ===== */

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
