#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include "board.hpp"

Board::Board(int rows, int columns, int squareWidth, sf::Vector2f startPos, int numBombs, sf::Vector2i windowSizeCopy) {
    width = columns;
    height = rows;
    squareSize = squareWidth;
    initialisePosition = startPos;
    windowSize = windowSizeCopy;
    board = new struct tile*[rows];
    for (int i = 0; i < rows; i++) {
        board[i] = new struct tile[columns];
    }
    initialise(numBombs);
    fillNums();
    smileyState = 0;
}

void Board::free() {
    for (int i = 0; i < width; i++) {
        delete[] board[i];
    }
}

void Board::initialise(int numBombs) {
    if (numBombs > height * width) {
        std::cout << "Number of bombs exceeds number of cells" << std::endl;
        return;
    }
    numBombsLeft = numBombs;
    srand(time(0));
    int added = 0;
    while (added < numBombs) {
        int row = (int) (rand() % height);
        int col = (int) (rand() % width);
        if (!board[row][col].bomb) {
            board[row][col].bomb = true;
            added++;
        }
    }
}

void Board::draw(sf::RenderWindow& window, sf::Texture textures[10], sf::Texture* uncoveredTexture) {
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    square.setTextureRect(sf::IntRect(0, 0, 16, 16));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (board[i][j].mouseHovered) {
                square.setTexture(&textures[9]);
            } else if (board[i][j].flagged) {
                square.setTexture(&textures[10]);
            } else if (!board[i][j].uncovered) {
                square.setTexture(uncoveredTexture);
            } else if (board[i][j].bomb) {
                square.setTexture(&textures[12]);
            } else if (board[i][j].numBombs > 0) {
                square.setTexture(&textures[board[i][j].numBombs]);
            } else {
                square.setTexture(&textures[9]);
            }
            if (gameEnded) {
                if (board[i][j].flagged && !board[i][j].bomb) {
                    square.setTexture(&textures[13]);
                } else if (board[i][j].bomb && board[i][j].uncovered) {
                    square.setTexture(&textures[11]);
                } else if (board[i][j].bomb && !board[i][j].flagged) {
                    square.setTexture(&textures[12]);
                }
            }
            square.setPosition(initialisePosition.x + squareSize * i, initialisePosition.y + squareSize * j);
            window.draw(square);
        }
    }
}


void Board::toggleFlag(int x, int y) {
    if (!board[x][y].uncovered) {
        board[x][y].flagged = !board[x][y].flagged;
        numBombsLeft += (board[x][y].flagged) ? -1 : 1;
    }
}

bool Board::notFlagged(int x, int y) {
    return !board[x][y].flagged;
}

bool Board::isUncovered(int x, int y) {
    return board[x][y].uncovered;
}

bool Board::bombUncovered(int x, int y) {
    return board[x][y].bomb;
}
