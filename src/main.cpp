#include "raylib.h"
#include "Die.h"
#include "level.h"
#include "PlayerDot.h"
#include "TopBar.h"
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <memory>

typedef enum GameScreen { LOGO = 0,
                          GAMEPLAY,
                          ENDING } GameScreen;

int main(void) {
    std::srand(std::time(0));
    // Initialization
    //--------------------------------------------------------------------------------------
    // const int screenWidth = 768;
    const int screenWidth = 1536;
    // const int screenHeight = 424;  // 384 usable 40 for top bar
    const int screenHeight = 808;
    const float topBorderPadding = 40.f;

    InitWindow(screenWidth, screenHeight, "Dieggle");
    SetMouseOffset(-4.8, -5.2);
    InitAudioDevice();
    SetMasterVolume(.5f);
    Music bgMusic = LoadMusicStream("../sound/calm-background.mp3");
    bgMusic.looping = true;
    unsigned int levelNum{1};
    unsigned int currentLevel{0};
    unsigned int framesCounter{};
    GameScreen currentScreen = LOGO;
    // Rectangle gameOverRec = Rectangle(screenWidth / 2 - 200, screenHeight / 2, 400.f, 50.f);

    std::unique_ptr<Level> level(new Level{levelNum, screenWidth, screenHeight, topBorderPadding});
    std::shared_ptr<PlayerDot> player(new PlayerDot);
    TopBar topBar;
    Die* pStartDie{nullptr};
    Die* pEndDie{nullptr};

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        float dT = GetFrameTime();
        if (IsAudioDeviceReady() && !IsMusicStreamPlaying(bgMusic)) {
            PlayMusicStream(bgMusic);
        }
        UpdateMusicStream(bgMusic);

        switch (currentScreen) {
        case LOGO: {
            framesCounter++;
            if (framesCounter > 120) {
                currentScreen = GAMEPLAY;
                framesCounter = 0;
            }
        } break;
        // case ENDING: {
        //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), gameOverRec)) {
        //         levelNum = 1;
        //         currentScreen = GAMEPLAY;
        //     }
        // } break;
        default:
            break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        switch (currentScreen) {
        case LOGO: {
            ClearBackground(RAYWHITE);
            DrawRectangle(screenWidth / 2 - 128, screenHeight / 2 - 128, 256, 256, BLACK);
            DrawRectangle(screenWidth / 2 - 112, screenHeight / 2 - 112, 224, 224, RAYWHITE);
            DrawText("raylib", screenWidth / 2 - 44, screenHeight / 2 + 48, 50, BLACK);
        } break;
        case GAMEPLAY: {
            ClearBackground(BLACK);
            if (levelNum != currentLevel) {
                currentLevel = levelNum;
                level->InitLevel();
                pStartDie = level->GetStartDie();
                pStartDie->SetStart(player);
                pEndDie = level->GetEndDie();
                pEndDie->SetEnd();
                level->GenerateSpecialDots();
            }
            level->CheckDotCollision(player);
            level->RenderDice();
            player->Tick(dT);
            // Check End
            if (CheckCollisionCircles(player->GetPos(), player->GetDotRadius(), pEndDie->GetEndDotPos(), pEndDie->GetEndDotRadius())) {
                topBar.UpdateLevel(++levelNum);
                topBar.AddShot();
            }
            topBar.Tick();
            if (topBar.GetShots() <= 0) {
                currentScreen = ENDING;
            }

            if (levelNum != currentLevel) {
                player.reset(new PlayerDot);
                level.reset(new Level{levelNum, screenWidth, screenHeight, topBorderPadding});
            }
        } break;
        case ENDING: {
            ClearBackground(BLACK);
            int mT = MeasureText("GameOver!", 80);
            DrawText("Game Over!", screenWidth / 2 - mT / 2, screenHeight / 3, 80, RED);
            int mTs = MeasureText(TextFormat("Score: %i", TopBar::GetScore()), 60);
            DrawText(TextFormat("Score: %i", TopBar::GetScore()), screenWidth / 2 - mTs / 2, screenHeight / 3 + 80, 60, RAYWHITE);
            ShowCursor();
            if (levelNum != currentLevel) {
                player.reset(new PlayerDot);
                level.reset(new Level{levelNum, screenWidth, screenHeight, topBorderPadding});
            }
        } break;
        default:
            break;
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
