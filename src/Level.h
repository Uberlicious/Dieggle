#ifndef LEVEL_H
#define LEVEL_H

#pragma once
#include "Die.h"
#include <memory>
#include <vector>

class Level {
public:
    static float BorderPadding;

public:
    Level(unsigned int level, unsigned int screenWidth, unsigned int screenHeight, float bP);
    ~Level();
    void InitLevel();
    void RenderDice();
    void GenerateSpecialDots();
    void CheckDotCollision(std::shared_ptr<PlayerDot> player);
    Die* GetEndDie();
    Die* GetStartDie();

private:
    Texture2D dieBackground = LoadTexture("../die/DieHD.png");
    Texture2D blackDot = LoadTexture("../die/BlankDotHD.png");
    unsigned int levelNumber{};
    int screenWidth{};
    int screenHeight{};
    unsigned int numDiceX{};
    unsigned int numDiceY{};
    float dieOffset{};
    unsigned int totalDice{};
    long double dieScale{1.0};
    std::vector<Die> dice;
    Die* pStartDie{nullptr};
    Die* pEndDie{nullptr};
    void CalculateDice();
    void GetStartEndDice();
};

#endif