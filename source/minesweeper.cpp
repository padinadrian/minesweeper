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
#include "version.hpp"

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
        if (x > 9) {
            printf("|%zu ", x);
        }
        else {
            printf("| %zu ", x);
        }
    }
    printf("|\n");
    
    // Print each row
    // Ex)
    // -------------
    // | 1 | 2 |   |
    for (size_t y = 0; y < height; ++y) {
        
        // Print top bar
        for (size_t i = 0; i < rowSeparatorWidth; ++i) { printf("-"); }
        if (y > 9) {
            printf("\n %zu ", y);
        }
        else {
            printf("\n  %zu ", y);
        }
        
        // Print each column within a row
        for (size_t x = 0; x < width; ++x) {
            
            if (visibility.at(x, y) && (mines.at(x, y) == -1)) {
                tileOutput = "\033[30;41mX\033[0m";     // Bomb: Dark red
            }
            
            else if (flags.at(x, y)) {
                tileOutput = "\033[1;41mF\033[0m";     // Flag: Light red
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

/** Construct the Minefield object. */
Minefield CreateMinefield(int8_t difficulty)
{
    // Difficulty:      W   H   Mines
    // Beginner:        9   9   10
    // Intermediate:    16  16  40
    // Expert:          30  16  99
    if (difficulty == 1) {
        return Minefield(9, 9, 10);
    }
    else if (difficulty == 2) {
        return Minefield(16, 16, 40);
    }
    else {
        return Minefield(30, 16, 99);
    }
}

/** Play the game */
void PlayGame()
{
    // Choose difficulty
    std::string input = "";
    int8_t difficulty = 0;
    while (difficulty == 0) {
        printf("Choose your difficulty - Beginner (B), Intermediate (I), Expert (E): ");
        std::getline(std::cin, input);
        
        if (input.length() > 0) {
            if (input[0] == 'B' || input[0] == 'b') {
                difficulty = 1;
            }
            if (input[0] == 'I' || input[0] == 'i') {
                difficulty = 2;
            }
            if (input[0] == 'E' || input[0] == 'e') {
                difficulty = 3;
            }
        }
        printf("\n");
    }
    
    Minefield minefield = CreateMinefield(difficulty);
    
    // Play the game
    char c;
    int x = 0;
    int y = 0;
    bool gameWon = false;
    bool gameLost = false;
    int8_t tileValue = 0;
    
    while (!(gameWon || gameLost)) {
        // Show the minefield and ask the player to pick a tile.
        printf("\n");
        DisplayMinefield(minefield);
        printf("Flags remaining: %zu\n", minefield.FlagsRemaining());
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
int main(int argc, char *argv[]) {
    
    if (argc > 1) {
        std::string arg1(argv[1]);
        if (arg1 == "--version") {
            printf("Minesweeper version %s\n", minesweeper::VERSION_FULL.c_str());
        }
        else {
            printf("Minesweeper clone by Adrian Padin (padin.adrian@gmail.com)\n");
            printf("Copyright (C) 2023\n");
            printf("Version %s\n\n", minesweeper::VERSION_FULL.c_str());
            printf("Usage:\n\tminesweeper\n\n");
        }
        return 0;
    }
    
    printf("\n\nWelcome to Minesweeper!\n\n");
    fflush(stdout);
    
    PlayGame();
    
    // TODO: prompt to play again
    return 0;
}