#include "TopBar.h"
#include "Level.h"
#include "raylib.h"
#include <iostream>
#include <string>

TopBar::TopBar() {
    topBar = Rectangle(0.f, 0.f, static_cast<float>(GetScreenWidth()), Level::BorderPadding);
    powerMeterOutline = Rectangle(GetScreenWidth() / 2 - recWidth / 2, 3.f, recWidth, recHeight);
}

unsigned int TopBar::score = 0;
float TopBar::power = 0;
unsigned int TopBar::shots = 3;

TopBar::~TopBar() {
}

void TopBar::Tick() {
    DrawRectangleRec(topBar, BLACK);
    LevelText();
    ScoreText();
    DrawPowerMeter();
    DrawShots();
}

void TopBar::UpdateLevel(unsigned int level) {
    this->level = level;
}

void TopBar::LevelText() {
    int lTSize = MeasureText(TextFormat("Level: %i", level), 35);
    DrawText(TextFormat("Level: %i", level), GetScreenWidth() - lTSize - 5.f, 5.f, 35, RAYWHITE);
}

void TopBar::ScoreText() {
    DrawText(TextFormat("Score: %i", score), 5.f, 5.f, 35, RAYWHITE);
}

void TopBar::AddScore(unsigned int points) {
    score += points;
}

void TopBar::DrawPowerMeter() {
    powerMeter = Rectangle(GetScreenWidth() / 2 - recWidth / 2, 3.f, recWidth * power, recHeight);
    Color RedThis = RED;
    RedThis.r *= power;
    DrawRectangleRounded(powerMeter, 3.f, 5, RedThis);
    DrawRectangleRoundedLines(powerMeterOutline, 3.f, 5, 2.f, RAYWHITE);
}

void TopBar::Power(float powerLevel) {
    power = powerLevel;
}

unsigned int TopBar::GetShots() {
    return shots;
}

void TopBar::RemoveShot() {
    shots--;
}

void TopBar::AddShot() {
    shots++;
}

void TopBar::DrawShots() {
    DrawText(TextFormat("Shots: %i", shots), GetScreenWidth() - (GetScreenWidth() / 4), 5.f, 35, RAYWHITE);
}

unsigned int TopBar::GetScore() {
    return score;
}