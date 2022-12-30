#ifndef TOPBAR_H
#define TOPBAR_H

#pragma once
#include "raylib.h"
#include "Level.h"

class TopBar {
public:
    TopBar();
    ~TopBar();
    void Tick();
    static void AddScore(unsigned int points);
    static void Power(float power);
    static void RemoveShot();
    static void AddShot();
    static unsigned int GetShots();
    static unsigned int GetScore();
    void UpdateLevel(unsigned int level);

private:
    Rectangle topBar{};
    static unsigned int score;
    unsigned int level{1};
    static float power;
    static unsigned int shots;
    float recWidth{400.f};
    float recHeight{35.f};
    Rectangle powerMeter{};
    Rectangle powerMeterOutline{};
    void LevelText();
    void ScoreText();
    void DrawPowerMeter();
    void DrawShots();
};

#endif