#include "raylib.h"
#include "Player.hpp"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "2-Player Fighting Game");
    SetTargetFPS(60);
    // Players
    Player p1, p2;
    p1.Init("assets/player1.png", (Vector2){100, screenHeight - 128}, KEY_A, KEY_D, KEY_W, KEY_F);
    p2.Init("assets/player2.png", (Vector2){600, screenHeight - 128}, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_K);

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();
        // Update
        p1.Update(delta);
        p2.Update(delta);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        p1.Draw();
        p2.Draw();

        // Health bars
        DrawRectangle(50, 25, p1.health * 2, 20, RED);
        DrawRectangle(550, 25, p2.health * 2, 20, BLUE);

        EndDrawing();
    }

    // Unload
    p1.Unload();
    p2.Unload();
    CloseWindow();
    return 0;
}