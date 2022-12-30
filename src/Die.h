#ifndef DIE_H
#define DIE_H

#pragma once
#include "raylib.h"
#include "PlayerDot.h"
#include <vector>
#include <memory>

class Die {
public:
    Die();
    ~Die();
    void SetDieSpecifics(Vector2 diePos, float scale, Texture2D bg, Texture2D bld);
    bool IsStart();
    bool IsEnd();
    void SetStart(std::shared_ptr<PlayerDot> player);
    void SetEnd();
    void Tick();
    void CheckDotCollision(std::shared_ptr<PlayerDot> player);
    Vector2 GetEndDotPos();
    float GetEndDotRadius();
    void GenerateSpecialDots();

private:
    void SetDieSide();
    void SetDotPositions();

private:
    Texture2D dieBackground{};
    Texture2D blackDot{};
    Texture2D greenDot{};
    Vector2 diePos{};
    unsigned int dotValue{1};
    float dieSize{};
    float scale{};
    unsigned int dieSide{};
    unsigned int dotSize{};
    std::vector<Vector2> dots{};
    std::vector<Vector2> redDots{};
    Vector2 endDot{};
    bool bIsStart{false};
    bool bIsEnd{false};
};

#endif