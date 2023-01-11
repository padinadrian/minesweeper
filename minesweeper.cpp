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
    const Grid& flags = minefield.Flags();
    const Grid& visibility = minefield.Visibility();
    
    const size_t width = mines.width();
    const size_t height = mines.height();
    const size_t rowSeparatorWidth = (4 * width) + 5;
    
    std::string tileOutput = " ";
    
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
            
            if (flags.at(x, y)) {
                tileOutput = "\033[1;41mF\033[0m";
            }
            
            else if (visibility.at(x, y)) {
                // The tile is visible - show the contents.
                int8_t tileValue = mines.at(x, y);
                
                switch (tileValue) {
                    case 0: { tileOutput = ' '; break; }
                    case 1: { tileOutput = "\033[34m1\033[0m"; break; }     // 1: Blue
                    case 2: { tileOutput = "\033[32m2\033[0m"; break; }     // 2: Green
                    case 3: { tileOutput = "\033[31m3\033[0m"; break; }     // 3: Red
                    case 4: { tileOutput = "\033[35m4\033[0m"; break; }     // 4: Magenta
                    case 5: { tileOutput = "\033[33m5\033[0m"; break; }     // 5: Yellow
                    case 6: { tileOutput = "\033[36m6\033[0m"; break; }     // 6: Cyan
                    case 7: { tileOutput = "\033[1;33m7\033[0m"; break; }   // 7: Bright yellow
                    case 8: { tileOutput = "\033[37m8\033[0m"; break; }     // 8: White
                    case -1: { tileOutput = "\033[30;41mX\033[0m"; break; } // Bomb: Dark red
                    default: { tileOutput = '?'; break; }
                }
            }
            else {
                // The tile is NOT visible - show hidden as "-".
                tileOutput = '-';
            }
            
            printf("| %s ", tileOutput.c_str());
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
    
    char c;
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
        printf("Flags remaining: %d\n", minefield.FlagsRemaining());
        printf("Pick a tile. Start with F to place a flag. Start with C to clear a flag.\n");
        printf("Input coordinates as (x y): ");
        fflush(stdout);
        
        // Testing use of getline
        std::getline(std::cin, input);
        printf("\n");
        
        // Normal input
        if (2 == sscanf(input.c_str(), "%d %d", &x, &y)) {
            tileValue = minefield.Update(Coordinates(x, y));
            
            // Game over if the tile is a bomb.
            gameLost = (tileValue == -1);
            gameWon = minefield.IsCleared();
        }
        
        // Place/clear a flag
        else if (3 == sscanf(input.c_str(), "%c %d %d", &c, &x, &y)) {
            if (c == 'F' || c == 'f') {
                minefield.PlaceFlag(Coordinates(x, y));
            }
            else if (c == 'C' || c == 'c') {
                minefield.ClearFlag(Coordinates(x, y));
            }
            else {
                printf("Sorry, I didn't understand your command.\n");
            }
        }
        
        // Unknown command
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