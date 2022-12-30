#ifndef PLAYERDOT_H
#define PLAYERDOT_H

#pragma once
#include "raylib.h"
#include <vector>

class PlayerDot {
public:
    PlayerDot();
    ~PlayerDot();
    void SetStartPosScale(Vector2 startPos, float scale);
    void Tick(float deltaTime);
    Vector2 GetPos();
    float GetDotRadius();

private:
    void GetAimLines(Vector2 mousePos);
    void DrawAimLines();
    void DrawRecursiveReflectionLines(Vector2 origin, Vector2 direction);
    Vector2 ClampLine(Vector2 line);
    float LinePower(Vector2 line);
    Vector2 NormalizeLine(Vector2 line);
    void MoveBall(float deltaTime);

private:
    Vector2 pos{};
    Vector2 center{};
    Texture2D playerDot = LoadTexture("../die/BlueDotHD.png");
    bool mouseDown{false};
    std::vector<Vector2> aimLinePoints{};
    float scale{};
    float aimLineMax{500.f};
    float linePower{};
    float multiplier{2.f};
    Vector2 velocity{};
    float drag{50.f};
    unsigned int lineCount{};
    Vector2 lastMousePos{};
    bool ballMoving{false};
    Vector2 intersection{};
};

#endif