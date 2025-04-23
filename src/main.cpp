#include <raylib.h>
#include "Game.h"

int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "My Game");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    
    Game game;
    
    while (!WindowShouldClose() && !game.IsExitGame())
    {
        game.Update();
        
        BeginDrawing();
            ClearBackground(darkGreen);
            game.Draw();
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
