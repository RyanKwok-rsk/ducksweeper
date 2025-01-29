// Minesweeper made by Ryan Kwok

#include <SFML/Graphics.hpp>
#include <iostream>
#include "main.hpp"
#include "board.hpp"

void mouseHoveredOn(Board *board, int x, int y, int radius);
void mouseHoveredOff(Board *board, int x, int y, int radius);
bool onResetButton(int x, int y, sf::RenderWindow& window);
int getNumber(std::string prompt);

int main()
{
    const int topBarHeight = 26;
    const int boardWidth = getNumber("Board Width: ");
    const int boardHeight = getNumber("Board Height: ");
    const int squareSize = 34;
    const int numBombs = getNumber("Number of bombs: ");

    const int windowWidth = boardWidth * squareSize + BOARD_PADDING * 2;
    const int windowHeight = boardHeight * squareSize + BOARD_PADDING * 3 + NUMBER_HEIGHT + NUMBER_PADDING * 2;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Ducksweeper - Ryan Kwok", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setSize(sf::Vector2u(windowWidth, windowHeight));
    window.requestFocus();

    const std::string imagePath = "textures/minesweeperDuck.png";
    //------------------------------------------------------------------
    // Square Textures
    //------------------------------------------------------------------
    sf::Texture uncoveredTexture;
    uncoveredTexture.loadFromFile(imagePath, sf::IntRect(0, 1, 68, 16));
    sf::Texture textures[NUM_TEXTURES];
    for (int i = 1; i <= 8; i++) {
        if (!textures[i].loadFromFile(imagePath, sf::IntRect(17 * (i - 1), 68, 16, 16))) {
            return 0;
        }
    }
    // Uncovered square
    textures[0].loadFromFile(imagePath, sf::IntRect(0, 51, SQUARE_TEXTURE_W, SQUARE_TEXTURE_W));
    // Empty Square
    textures[9].loadFromFile(imagePath, sf::IntRect(17, 51, SQUARE_TEXTURE_W, SQUARE_TEXTURE_W));
    // Flag
    textures[10].loadFromFile(imagePath, sf::IntRect(34, 51, SQUARE_TEXTURE_W, SQUARE_TEXTURE_W));
    // Bomb red
    textures[11].loadFromFile(imagePath, sf::IntRect(17 * 6, 51, SQUARE_TEXTURE_W, SQUARE_TEXTURE_W));
    // Bomb white
    textures[12].loadFromFile(imagePath, sf::IntRect(17 * 5, 51, SQUARE_TEXTURE_W, SQUARE_TEXTURE_W));
    // Bomb incorrect
    textures[13].loadFromFile(imagePath, sf::IntRect(17 * 7, 51, SQUARE_TEXTURE_W, SQUARE_TEXTURE_W));

    // Smileys
    for (int i = 0; i < 5; i++) {
        textures[14 + i].loadFromFile(imagePath, sf::IntRect(27 * i, 24, SMILEY_TEXTURE_W, SMILEY_TEXTURE_W));
    }

    // Numbers
    // Zero
    textures[19].loadFromFile(imagePath, sf::IntRect(126, 0, 13, 23));
    // From 1 to 9
    for (int i = 0; i < 9; i++) {
        textures[20 + i].loadFromFile(imagePath, sf::IntRect(14 * i, 0, 13, 23));
    }

    for (int i = 0; i < NUM_TEXTURES; i++) {
        textures[i].setSmooth(false);
    }

    const sf::Vector2i windowSize(window.getSize().x, window.getSize().y);

    sf::Vector2i initialPosition(windowSize.x / 2 - (boardWidth / 2 * squareSize), BOARD_PADDING * 2 + NUMBER_HEIGHT + NUMBER_PADDING * 2);

    Board board(boardWidth, boardHeight, squareSize, (sf::Vector2f) initialPosition, numBombs, windowSize);


    bool rightClicked = false;
    bool leftClicked = false;
    int currentRadius = 0;
    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
    sf::Vector2i coords = (localPosition - initialPosition) / squareSize;    

    while (window.isOpen())
    {
        sf::Event event;
        if (board.started) {
            board.updateTime(); 
        }

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();


            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                leftClicked = true;
            }

            if (leftClicked) {
                localPosition = sf::Mouse::getPosition(window);
                if (onResetButton(localPosition.x, localPosition.y, window)) {
                    board.smileyState = 1;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (leftClicked) {
                    localPosition = sf::Mouse::getPosition(window);
                    if (onResetButton(localPosition.x, localPosition.y, window)) {
                        board = Board(boardWidth, boardHeight, squareSize, (sf::Vector2f) initialPosition, numBombs, windowSize);                        
                    }
                }
            }

            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            //     board.smileyState = 1;
            // }

            // if (sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
            //     board = Board(boardWidth, boardHeight, squareSize, (sf::Vector2f) initialPosition, numBombs);
            //     board.smileyState = 0;
            // }

            if (board.gameEnded) {
                goto drawing;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !rightClicked) {
                rightClicked = true;
                localPosition = sf::Mouse::getPosition(window);
                coords = (localPosition - initialPosition) / squareSize;
                if (localPosition.y < initialPosition.y) continue;
                if (board.inBounds(coords.x, coords.y)) {
                    board.toggleFlag(coords.x, coords.y);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (leftClicked) {
                    localPosition = sf::Mouse::getPosition(window);

                    coords = (localPosition - initialPosition) / squareSize;
                    if (localPosition.y < initialPosition.y) {
                        coords.y = -10;
                    }
                    if (board.inBounds(coords.x, coords.y) && board.notFlagged(coords.x, coords.y)) {
                        if (!board.started) {
                            board.startTimer();
                            board.started = true;
                        }
                        if (board.isUncovered(coords.x, coords.y)) {
                            board.largeUncover(coords.x, coords.y);
                        } else {
                            board.floodUncover(coords.x, coords.y);
                        }
                        if (board.bombUncovered(coords.x, coords.y) || board.gameWon()) {
                            board.gameOver(board.gameWon());
                            board.gameEnded = true;
                        }
                    }
                }
                rightClicked = false;
                leftClicked = false;
            }
        }

        if (leftClicked) {
            localPosition = sf::Mouse::getPosition(window);
            coords = (localPosition - initialPosition) / squareSize;
            if (localPosition.y < initialPosition.y) {
                coords.y = -10;
            }
            if (board.inBounds(coords.x, coords.y) && board.notFlagged(coords.x, coords.y)) {
                if (board.isUncovered(coords.x, coords.y)) {
                    mouseHoveredOn(&board, coords.x, coords.y, 1);
                    currentRadius = 1;
                } else {
                    mouseHoveredOn(&board, coords.x, coords.y, 0);
                    currentRadius = 0;
                }
            }
        }

        drawing:
        window.clear(sf::Color(200, 200, 200));
        board.draw(window, textures, &textures[0]);
        board.drawBar(window, textures);
        board.drawBorders(window, textures);
        mouseHoveredOff(&board, coords.x, coords.y, currentRadius);
        window.display();
    }
    return 0;
}

void mouseHoveredOn(Board *board, int x, int y, int radius) {
    for (int i = x - radius; i <= x + radius; i++) {
        for (int j = y - radius; j <= y + radius; j++) {
            if (board->inBounds(i, j) && !board->board[i][j].uncovered && !board->board[i][j].flagged) {
                board->board[i][j].mouseHovered = true;
            }
        }
    }
}

void mouseHoveredOff(Board *board, int x, int y, int radius) {
    for (int i = x - radius; i <= x + radius; i++) {
        for (int j = y - radius; j <= y + radius; j++) {
            if (board->inBounds(i, j) && !board->board[i][j].uncovered && !board->board[i][j].flagged) {
                board->board[i][j].mouseHovered = false;
            }
        }
    }
}

int getNumber(std::string prompt) {
    std::cout << prompt;
    int number;
    std::cin >> number;
    return number;
}

bool onResetButton(int x, int y, sf::RenderWindow& window) {
    return 
    y > BOARD_PADDING && 
    y < SMILEY_TEXTURE_W * 2 + BOARD_PADDING && 
    x > window.getSize().x / 2 - SMILEY_TEXTURE_W && 
    x < window.getSize().x / 2 + SMILEY_TEXTURE_W;
}