#include <SFML/Graphics.hpp>
#include "main.hpp"

#ifndef BOARD_H
#define BOARD_H

struct tile {
    bool uncovered = false;
    bool bomb = false;
    int numBombs = 0;
    bool flagged = false;
    bool mouseHovered = false;
};

class Board {
    int squareSize;
    sf::Vector2f initialisePosition;
    sf::Clock timer;
    int currentTime = 0;
    sf::Vector2i windowSize;
    public:
        bool started = false;
        int width;
        int height;
        int numBombsLeft;
        bool gameEnded = false;
        int smileyState = 0;
        int numBombs;
        struct tile **board;
        Board(int x, int y, int squareSize, sf::Vector2f initialisePosition, int numBombs, sf::Vector2i windowSize);
        // Frees the board
        void free();
        // Adds all the bombs to the board
        void initialise(int numBombs);
        // Draws the board to the render window
        void draw(sf::RenderWindow& window, sf::Texture numberTextures[NUM_TEXTURES], sf::Texture* uncoveredTexture);
        void drawBar(sf::RenderWindow& window, sf::Texture textures [NUM_TEXTURES]);
        void drawBorders(sf::RenderWindow& window, sf::Texture* uncoveredTexture);
        // Fills in all the numbers for empty cells
        void fillNums();
    
        bool inBounds(int x, int y);
        // Flood fills cells that have a number of 0
        void floodUncover(int x, int y);
        void toggleFlag(int x, int y);
        bool notFlagged(int x, int y);
        bool isUncovered(int x, int y);
        bool bombUncovered(int x, int y);
        void largeUncover(int x, int y);
        bool gameWon();
        void gameOver(bool won);
        void updateTime();
        void startTimer();
        void reset();
};




#endif