#include "Die.h"
#include "PlayerDot.h"
#include "TopBar.h"
#include <iostream>
#include <ctime>
#include <memory>
#include <algorithm>

Die::Die() {
    SetDieSide();
}

Die::~Die() {
    UnloadTexture(dieBackground);
    UnloadTexture(blackDot);
}

void Die::SetDieSpecifics(Vector2 diePos, float scale, Texture2D bg, Texture2D bld) {
    this->scale = scale;
    this->diePos = diePos;
    dieBackground = bg;
    blackDot = bld;
    dieSize = dieBackground.width * scale;
    dotSize = blackDot.width * scale;
    SetDotPositions();
}

void Die::GenerateSpecialDots() {
    for (auto it = dots.begin(); it != dots.end();) {
        if (std::rand() % 20 == 0) {
            // special dot
            redDots.push_back(*it);
            it = dots.erase(it);
        } else {
            it++;
        }
    }
}

void Die::Tick() {
    DrawTextureEx(dieBackground, diePos, 0.0f, scale, WHITE);
    if (dots.size() > 0) {
        for (auto &dot : dots) {
            DrawTextureEx(blackDot,
                          dot,
                          0.f,
                          scale,
                          BLACK);
        }
    }
    for (auto &dot : redDots) {
        DrawTextureEx(blackDot,
                      dot,
                      0.f,
                      scale,
                      RED);
    }
    if (bIsEnd) {
        DrawTextureEx(greenDot, endDot, 0.f, scale, WHITE);
    }
}

void Die::SetDieSide() {
    dieSide = std::rand() % 6;
}

bool Die::IsStart() {
    return bIsStart;
}

bool Die::IsEnd() {
    return bIsEnd;
}

void Die::SetStart(std::shared_ptr<PlayerDot> player) {
    bIsStart = true;
    size_t startIndex = std::rand() % dots.size();
    player->SetStartPosScale(dots[startIndex], scale);
    std::vector<Vector2>::iterator it = dots.begin() + startIndex;
    dots.erase(it);
}

void Die::SetEnd() {
    bIsEnd = true;
    size_t endIndex = std::rand() % dots.size();
    endDot = dots[endIndex];
    std::vector<Vector2>::iterator it = dots.begin() + endIndex;
    dots.erase(it);
    greenDot = LoadTexture("../die/GreenDotHD.png");
    // correct offset for larger dot
    endDot.x += (dotSize / 2) - (greenDot.width * scale / 2);
    endDot.y += (dotSize / 2) - (greenDot.height * scale / 2);
}

void Die::SetDotPositions() {
    switch (dieSide) {
    case 0:
        // Center Dot
        dots.push_back(Vector2{
            (dieSize / 2) - (dotSize / 2) + diePos.x,
            (dieSize / 2) - (dotSize / 2) + diePos.y});
        break;
    case 1:
        // Top Left Dot
        dots.push_back(Vector2{
            (dieSize / 5) - (dotSize / 2) + diePos.x,
            (dieSize / 5) - (dotSize / 2) + diePos.y});
        // Bottom Right Dot
        dots.push_back(Vector2{
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.x,
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.y});
        break;
    case 2:
        // Top Right Dot
        dots.push_back(Vector2{
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.x,
            (dieSize / 5) - (dotSize / 2) + diePos.y});
        // Center Dot
        dots.push_back(Vector2{
            (dieSize / 2) - (dotSize / 2) + diePos.x,
            (dieSize / 2) - (dotSize / 2) + diePos.y});
        // Bottom Left Dot
        dots.push_back(Vector2{
            (dieSize / 5) - (dotSize / 2) + diePos.x,
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.y});
        break;
    case 3:
        // Top Left Dot
        dots.push_back(Vector2{
            (dieSize / 5) - (dotSize / 2) + diePos.x,
            (dieSize / 5) - (dotSize / 2) + diePos.y});
        // Top Right Dot
        dots.push_back(Vector2{
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.x,
            (dieSize / 5) - (dotSize / 2) + diePos.y});
        // Bottom Left Dot
        dots.push_back(Vector2{
            (dieSize / 5) - (dotSize / 2) + diePos.x,
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.y});
        // Bottom Right Dot
        dots.push_back(Vector2{
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.x,
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.y});
        break;
    case 4:
        // Top Left Dot
        dots.push_back(Vector2{
            (dieSize / 5) - (dotSize / 2) + diePos.x,
            (dieSize / 5) - (dotSize / 2) + diePos.y});
        // Top Right Dot
        dots.push_back(Vector2{
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.x,
            (dieSize / 5) - (dotSize / 2) + diePos.y});
        // Center Dot
        dots.push_back(Vector2{
            (dieSize / 2) - (dotSize / 2) + diePos.x,
            (dieSize / 2) - (dotSize / 2) + diePos.y});
        // Bottom Left Dot
        dots.push_back(Vector2{
            (dieSize / 5) - (dotSize / 2) + diePos.x,
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.y});
        // Bottom Right Dot
        dots.push_back(Vector2{
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.x,
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.y});
        break;
    case 5:
        // Top Left Dot
        dots.push_back(Vector2{
            (dieSize / 5) - (dotSize / 2) + diePos.x,
            (dieSize / 5) - (dotSize / 2) + diePos.y});
        // Top Center Dot
        dots.push_back(Vector2{
            (dieSize / 2) - (dotSize / 2) + diePos.x,
            (dieSize / 5) - (dotSize / 2) + diePos.y});
        // Top Right Dot
        dots.push_back(Vector2{
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.x,
            (dieSize / 5) - (dotSize / 2) + diePos.y});
        // Bottom Left Dot
        dots.push_back(Vector2{
            (dieSize / 5) - (dotSize / 2) + diePos.x,
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.y});
        // Bottom Center Dot
        dots.push_back(Vector2{
            (dieSize / 2) - (dotSize / 2) + diePos.x,
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.y});
        // Bottom Right Dot
        dots.push_back(Vector2{
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.x,
            (dieSize - (dieSize / 5)) - (dotSize / 2) + diePos.y});
    }
}

Vector2 Die::GetEndDotPos() {
    return Vector2{endDot.x + ((greenDot.width * scale) / 2), endDot.y + ((greenDot.height * scale) / 2)};
}

float Die::GetEndDotRadius() {
    return (greenDot.width * scale) / 2;
}

void Die::CheckDotCollision(std::shared_ptr<PlayerDot> player) {
    // black dot collision
    for (auto it = dots.begin(); it != dots.end();) {
        Vector2 dotCenter = Vector2{it->x + ((blackDot.width * scale) / 2), it->y + ((blackDot.height * scale) / 2)};
        if (CheckCollisionCircles(player->GetPos(), player->GetDotRadius(), dotCenter, blackDot.width * scale / 2)) {
            dots.erase(it);
            TopBar::AddScore(dotValue);
            break;
        } else {
            it++;
        }
    }
    // red dot collision
    for (auto it = redDots.begin(); it != redDots.end();) {
        // std::cout << it->x << "," << it->y << std::endl;
        Vector2 dotCenter = Vector2{it->x + ((blackDot.width * scale) / 2), it->y + ((blackDot.height * scale) / 2)};
        // std::cout << dotCenter.x << "," << dotCenter.y << std::endl;
        if (CheckCollisionCircles(player->GetPos(), player->GetDotRadius(), dotCenter, blackDot.width * scale / 2)) {
            redDots.erase(it);
            TopBar::RemoveShot();
            break;
        } else {
            it++;
        }
    }
}