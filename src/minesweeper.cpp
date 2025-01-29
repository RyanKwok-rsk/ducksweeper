#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.hpp"

static int getNumNeighbours(Board *board, int x, int y);
static int getNumNeighbourFlags(Board *board, int x, int y);

bool Board::inBounds(int x, int y) {
    return x >= 0 && x < height && y >= 0 && y < width;
}

void Board::fillNums() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board[i][j].numBombs = getNumNeighbours(this, i, j);
        }
    }
}

static int getNumNeighbours(Board *board, int x, int y) {
    int count = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (board->inBounds(i, j) && board->board[i][j].bomb) {
                count++;
            }
        }
    }
    return count;
}

void Board::floodUncover(int x, int y) {
    board[x][y].uncovered = true;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (this->inBounds(i, j) && !board[i][j].uncovered && board[x][y].numBombs == 0) {
                if (board[i][j].numBombs == 0) {
                    floodUncover(i, j);
                    continue;
                }
                board[i][j].uncovered = true;
            }
        }
    }
}

void Board::largeUncover(int x, int y) {
    board[x][y].uncovered = true;
    if (getNumNeighbourFlags(this, x, y) != board[x][y].numBombs) {
        return;
    }
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (inBounds(i, j) && board[i][j].bomb && !board[i][j].flagged) {
                gameEnded = true;
                gameOver(false);
            }
            if (inBounds(i, j) && !board[i][j].uncovered && !board[i][j].flagged) {
                floodUncover(i, j);
                board[i][j].uncovered = true;
            }
        }
    }
    
}

static int getNumNeighbourFlags(Board *board, int x, int y) {
    int count = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (board->inBounds(i, j) && board->board[i][j].flagged) {
                count++;
            }
        }
    }
    return count;
}

bool Board::gameWon() {
    int numUncovered = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!board[i][j].uncovered && !board[i][j].bomb) {
                numUncovered++;
            }
        }
    }
    return numUncovered == 0;
}

void Board::gameOver(bool won) {
    started = false;
    if (won) {
        std::cout << "Game Won" << std::endl;
        smileyState = 3;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (board[i][j].bomb && !board[i][j].uncovered && !board[i][j].flagged) {
                    board[i][j].flagged = true;
                    numBombsLeft--;
                }
            }
        }
    } else {
        std::cout << "Game Lost" << std::endl;
        smileyState = 4;
        // for (int i = 0; i < width; i++) {
        //     for (int j = 0; j < height; j++) {
        //         if (board[i][j].bomb && !board[i][j].flagged) {
        //             board[i][j].uncovered = true;
        //         }
        //     }
        // }
    }
}

void Board::updateTime() {
    currentTime = timer.getElapsedTime().asSeconds();
}

void Board::startTimer() {
    timer = sf::Clock();
}