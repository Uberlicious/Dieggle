#include <iostream>
#include <cstdlib>
#include "Level.h"
#include "Die.h"
#include <vector>
#include <memory>

Level::Level(unsigned int level, unsigned int screenWidth, unsigned int screenHeight, float bP)
    : levelNumber(level),
      screenWidth(screenWidth),
      screenHeight(screenHeight) {
    BorderPadding = bP;
    CalculateDice();
}

float Level::BorderPadding{};

Level::~Level() {
    pStartDie = nullptr;
    pEndDie = nullptr;
    UnloadTexture(dieBackground);
    UnloadTexture(blackDot);
}

void Level::InitLevel() {
    dice.resize(totalDice);
    std::cout << totalDice << std::endl;
    size_t row{};
    size_t col{};
    for (size_t i{0}; i < totalDice; i++) {
        if (col >= numDiceX && i < totalDice - 1) {
            row++;
            col = 0;
        }
        dice[i].SetDieSpecifics(Vector2{static_cast<float>(col) * static_cast<float>(dieOffset),
                                        static_cast<float>(row) * static_cast<float>(dieOffset) + static_cast<float>(BorderPadding)},
                                dieScale,
                                dieBackground,
                                blackDot);
        col++;
    }
    GetStartEndDice();
}

void Level::GetStartEndDice() {
    if (levelNumber == 1) {
        pStartDie = &dice[0];
        pEndDie = &dice[1];
        std::cout << "die " << pEndDie << std::endl;
        return;
    }
    pStartDie = &dice[std::rand() % numDiceX];
    pEndDie = &dice[(totalDice - numDiceX) + (std::rand() % numDiceX)];
}

Die* Level::GetEndDie() {
    return pEndDie;
}

Die* Level::GetStartDie() {
    return pStartDie;
}

void Level::CalculateDice() {
    dieScale /= levelNumber;
    numDiceY = levelNumber;
    numDiceX = numDiceY * 2;
    totalDice = numDiceX * numDiceY;
    std::cout << "total: " << totalDice << " numx: " << numDiceX << " numy: " << numDiceY << std::endl;
    dieOffset = dieBackground.width * dieScale;
}

void Level::RenderDice() {
    for (size_t i{0}; i < totalDice; i++) {
        dice[i].Tick();
    }
}

void Level::CheckDotCollision(std::shared_ptr<PlayerDot> player) {
    for (size_t i{0}; i < totalDice; i++) {
        dice[i].CheckDotCollision(player);
    }
}

void Level::GenerateSpecialDots() {
    for (size_t i{0}; i < totalDice; i++) {
        dice[i].GenerateSpecialDots();
    }
}