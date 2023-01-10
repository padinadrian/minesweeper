/*
    Filename:       minesweeper.cpp
    Author:         Adrian Padin (padin.adrian@gmail.com)
    Initial date:   2023-01-07
    Description:    Minesweeper clone
*/

/* ===== Includes ===== */
#include <cstdio>
#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include "Minefield.hpp"

/* ===== Functions ===== */

/** Display the minefield. */
void DisplayMinefield(const Minefield& minefield)
{
    const Grid& mines = minefield.Mines();
    const Grid& visibility = minefield.Visibility();
    
    const size_t width = mines.width();
    const size_t height = mines.height();
    const size_t rowSeparatorWidth = (4 * width) + 5;
    
    char printChar;
    int8_t tileValue = 0;
    int8_t tileVisible = 0;
    
    // Print top numbers
    printf("    ");
    for (size_t x = 0; x < width; ++x) {
        printf("| %d ", x % 10);
    }
    printf("|\n");
    
    // Print each row
    // Ex)
    // -------------
    // | 1 | 2 |   |
    for (size_t y = 0; y < height; ++y) {
        
        // Print top bar
        for (size_t i = 0; i < rowSeparatorWidth; ++i) { printf("-"); }
        printf("\n  %d ", y % 10);
        
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
    std::string input;
    
    while (!(gameWon || gameLost)) {
        // Show the minefield and ask the player to pick a tile.
        printf("\n");
        DisplayMinefield(minefield);
        printf("Pick a tile. Input coordinates as (x y): ");
        fflush(stdout);
        
        // Testing use of getline
        std::getline(std::cin, input);
        printf("\n");
        
        int parseResults = sscanf(input.c_str(), "%d %d", &x, &y);
        if (2 == parseResults) {
            tileValue = minefield.Update(Coordinates(x, y));
        
            // Game over if the tile is a bomb.
            gameLost = (tileValue == -1);
            gameWon = minefield.IsCleared();
        }
        else {
            printf("Sorry, I didn't understand your command.\n");
        }
    }
    
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
    printf("Welcome to Minesweeper!\n");
    fflush(stdout);
    
    PlayGame();
    
    // TODO: prompt to play again
}