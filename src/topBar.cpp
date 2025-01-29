#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "board.hpp"

void Board::drawBar(sf::RenderWindow& window, sf::Texture textures [NUM_TEXTURES]) {
    // Draw the smiley button in the top middle
    sf::RectangleShape faceSquare(sf::Vector2f(50, 50));
    faceSquare.setTexture(&textures[14 + smileyState]);
    faceSquare.setPosition(windowSize.x / 2 - 25, BOARD_PADDING + NUMBER_PADDING / 2);
    window.draw(faceSquare);

    // Draw the number of bombs left in the top left.
    sf::RectangleShape numberSprite(sf::Vector2f(26, 46));
    for (int i = 0; i < 3; i++) {
        // numberSprite.setTextureRect(sf::IntRect(0, 0, 12, 22));
        int currentNumber = (numBombsLeft / (int) pow(10, i)) % 10;
        numberSprite.setTexture(&textures[19 + currentNumber]);
        numberSprite.setPosition(NUMBER_WIDTH * (2 - i) + BOARD_PADDING + NUMBER_PADDING, BOARD_PADDING + NUMBER_PADDING);
        window.draw(numberSprite);
    }

    // Timer in the top right
    for (int i = 0; i < 3; i++) {
        // numberSprite.setTextureRect(sf::IntRect(0, 0, 12, 22));
        int currentNumber = (currentTime / (int) pow(10, i)) % 10;
        numberSprite.setTexture(&textures[19 + currentNumber]);
        numberSprite.setPosition(windowSize.x - NUMBER_WIDTH * (i + 1) - BOARD_PADDING - NUMBER_PADDING, BOARD_PADDING + NUMBER_PADDING);
        window.draw(numberSprite);
    }
}

void Board::drawBorders(sf::RenderWindow& window, sf::Texture* uncoveredTexture) {
    // Vertical Bars
    sf::RectangleShape sideSprite(sf::Vector2f(BOARD_PADDING, windowSize.y));
    sideSprite.setTexture(uncoveredTexture);
    sideSprite.setTextureRect(sf::IntRect(0, 2, 16, 12));
    sideSprite.setPosition(0, 0);
    window.draw(sideSprite);
    sideSprite.setPosition(windowSize.x - BOARD_PADDING, 0);
    window.draw(sideSprite);

    // Horizontal Bars
    sf::RectangleShape barSprite(sf::Vector2f(windowSize.x - BOARD_PADDING * 2 + BAR_PIXEL_OVERLAP, BOARD_PADDING));
    barSprite.setTexture(uncoveredTexture);
    barSprite.setTextureRect(sf::IntRect(2, 0, 12, 16));
    // Top bar
    barSprite.setPosition(BOARD_PADDING - BAR_PIXEL_OVERLAP / 2, 0);
    window.draw(barSprite);
    // Bottom bar
    barSprite.setPosition(BOARD_PADDING - BAR_PIXEL_OVERLAP / 2, windowSize.y - BOARD_PADDING);
    window.draw(barSprite);
    // Middle bar
    barSprite.setPosition(BOARD_PADDING - BAR_PIXEL_OVERLAP / 2, initialisePosition.y - BOARD_PADDING);
    window.draw(barSprite);

    // Corner fix
    sf::RectangleShape hLineFix(sf::Vector2f(windowSize.x, BOARD_PADDING / 8));
    hLineFix.setTexture(uncoveredTexture);
    hLineFix.setTextureRect(sf::IntRect(0, 0, 2, 2));
    window.draw(hLineFix);
    hLineFix.setTextureRect(sf::IntRect(14, 14, 2, 2));
    hLineFix.setPosition(0, windowSize.y - BOARD_PADDING / 8);
    window.draw(hLineFix);
}


