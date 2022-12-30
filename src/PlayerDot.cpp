#include "PlayerDot.h"
#include "Level.h"
#include "TopBar.h"
#include <iostream>
#include <math.h>
#include <algorithm>

PlayerDot::PlayerDot() {
}

PlayerDot::~PlayerDot() {
    UnloadTexture(playerDot);
}

void PlayerDot::SetStartPosScale(Vector2 startPos, float scale) {
    pos = startPos;
    this->scale = scale;
}

void PlayerDot::Tick(float deltaTime) {
    center = Vector2{pos.x + (playerDot.width * scale) / 2, pos.y + (playerDot.height * scale) / 2};

    if (ballMoving) {
        MoveBall(deltaTime);
    } else {
        ShowCursor();
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionCircles(mousePos, playerDot.width / 2, center, (playerDot.width * scale) / 2)) {
            mouseDown = true;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mouseDown) {
            mouseDown = false;
            if (linePower > 0) {
                ballMoving = true;
                TopBar::Power(0.f);
            }
        }
        if (mouseDown) {
            if (mousePos.x != lastMousePos.x && mousePos.y != lastMousePos.y) {
                aimLinePoints.clear();
                GetAimLines(mousePos);
                lastMousePos = mousePos;
            }
            TopBar::Power(linePower);
            DrawAimLines();
        }
    }
    DrawTextureEx(playerDot, pos, 0.f, scale, WHITE);
}

void PlayerDot::DrawAimLines() {
    // std::vector<Vector2>::iterator it = aimLinePoints.begin();
    for (size_t i{1}; i < aimLinePoints.size(); i++) {
        Vector2 prevLine = aimLinePoints[i - 1];
        DrawLineEx(prevLine, aimLinePoints[i], 5.f, GREEN);
    }
}

void PlayerDot::MoveBall(float deltaTime) {
    HideCursor();
    pos.x += velocity.x * linePower * multiplier * deltaTime;
    pos.y += velocity.y * linePower * multiplier * deltaTime;

    // reflect off play area borders
    if (pos.x <= 0.f || (pos.x + (playerDot.width * scale)) >= static_cast<float>(GetScreenWidth())) {
        velocity.x *= -1.0f;
    } else if (pos.y <= Level::BorderPadding || (pos.y + (playerDot.height * scale)) >= static_cast<float>(GetScreenHeight())) {
        velocity.y *= -1.0f;
    }
    // std::cout << "Pos: " << pos.x << "," << pos.y << std::endl;

    // decay velocity
    Vector2 velNorm = NormalizeLine(velocity);
    velocity.x -= drag * velNorm.x * deltaTime;
    velocity.y -= drag * velNorm.y * deltaTime;
    float lineMag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
    if (lineMag < 1) {
        ballMoving = false;
        velocity = {};
        TopBar::RemoveShot();
    }
}

Vector2 PlayerDot::ClampLine(Vector2 line) {
    float lineMag = sqrt((line.x * line.x) + (line.y * line.y));
    float lineNorm = std::min(lineMag, aimLineMax) / lineMag;
    return Vector2{lineNorm * line.x, lineNorm * line.y};
}

Vector2 PlayerDot::NormalizeLine(Vector2 line) {
    float lineMag = sqrt((line.x * line.x) + (line.y * line.y));
    return Vector2{line.x / lineMag, line.y / lineMag};
}

float PlayerDot::LinePower(Vector2 line) {
    float lineMag = sqrt((line.x * line.x) + (line.y * line.y));
    return (lineMag / aimLineMax);
}

void PlayerDot::GetAimLines(Vector2 mousePos) {
    velocity = ClampLine(Vector2{center.x - mousePos.x, center.y - mousePos.y});
    linePower = LinePower(velocity);
    Vector2 direction{};
    direction.x = velocity.x + center.x;
    direction.y = velocity.y + center.y;
    aimLinePoints.push_back(center);
    DrawRecursiveReflectionLines(center, direction);
}

void PlayerDot::DrawRecursiveReflectionLines(Vector2 origin, Vector2 direction) {
    if (
        !direction.x ||
        !direction.y ||
        !origin.x ||
        !origin.y ||
        (direction.x == intersection.x && direction.y == intersection.y)) return;
    bool reflect{false};
    float dotOffset = (playerDot.width * scale / 2);
    Vector2 reflection{};
    const float sW = static_cast<float>(GetScreenWidth());
    const float sH = static_cast<float>(GetScreenHeight());
    const float bP = Level::BorderPadding;
    // std::cout << "Reflection Origin: " << origin.x << "," << origin.y << ": " << direction.x << "," << direction.y << std::endl;
    // std::cout << "Intersection: " << intersection.x << "," << intersection.y << std::endl;
    //  Screen Left Collision
    try {
        if (CheckCollisionLines(
                origin,
                direction,
                Vector2{0.f, bP},
                Vector2{0.f, sH},
                &intersection)) {
            reflect = true;
            // DrawCircleV(intersection, 10.f, RED);
            reflection = Vector2{-direction.x, (direction.y - intersection.y) + intersection.y};
            aimLinePoints.push_back(intersection);
        }
        // Screen Right Collision
        if (CheckCollisionLines(
                origin,
                direction,
                Vector2{sW, bP},
                Vector2{sW, sH},
                &intersection)) {
            reflect = true;
            // DrawCircleV(intersection, 10.f, RED);
            reflection = Vector2{sW - (direction.x - sW), (direction.y - intersection.y) + intersection.y};
            aimLinePoints.push_back(intersection);
        }
        // Screen Top Collision
        if (CheckCollisionLines(
                origin,
                Vector2{direction.x, direction.y + playerDot.height / 2},
                Vector2{0.f, bP},
                Vector2{sW, bP},
                &intersection)) {
            reflect = true;
            // DrawCircleV(intersection, 10.f, RED);
            reflection = Vector2{(direction.x - intersection.x) + intersection.x, (intersection.y - direction.y) + intersection.y};
            aimLinePoints.push_back(intersection);
        }
        // Screen Bottom Collision
        if (CheckCollisionLines(
                origin,
                direction,
                Vector2{0.f, sH},
                Vector2{sW, sH},
                &intersection)) {
            reflect = true;
            // DrawCircleV(intersection, 10.f, RED);
            reflection = Vector2{(direction.x - intersection.x) + intersection.x, (intersection.y - direction.y) + intersection.y};
            aimLinePoints.push_back(intersection);
        }
    } catch (std::exception& ex) {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
    if (reflect) {
        DrawRecursiveReflectionLines(origin, reflection);
        return;
    } else {
        aimLinePoints.push_back(direction);
        return;
    }
}

Vector2 PlayerDot::GetPos() {
    return center;
}

float PlayerDot::GetDotRadius() {
    return (playerDot.width * scale) / 2;
}