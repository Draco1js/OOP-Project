#include "Game.h"

// Constructor
Game::Game() : exitGame(false), currentState(GameState::MENU) {}

// Destructor
Game::~Game() {}

// Function Defination Of Initialize Method
void Game::Initialize()
{
    InitWindow(screenWidth, screenHeight, "2D Fighting Game");      //InitWindow:  Initialize window and OpenGL context
    SetExitKey(KEY_NULL);  // Disable automatic exit with ESC key
    SetTargetFPS(60);       //SetTargetFPS: Set target FPS (maximum)
}

// Function Defination Of RunGameLoop Method
void Game::RunGameLoop()
{
    while (!WindowShouldClose() && !exitGame)       //WindowShouldClose: Check if application should close (KEY_ESCAPE pressed or windows close icon clicked)
    {
        // Update
        switch (currentState)       // Takes Current state, then selects appropriate status and updates it
        {
        case GameState::MENU:
            UpdateMenu();
            break;
        case GameState::GAMEPLAY:
            UpdateGameplay();
            break;
        case GameState::GAMEOVER:
            UpdateGameOver();
            break;
        }

        // Draw
        BeginDrawing();     // BeginDrawing: Setup canvas (framebuffer) to start drawing
        ClearBackground(RAYWHITE);      //ClearBackground: Clear image background with given color

        switch (currentState)
        {
        case GameState::MENU:       // Takes Current state, then selects appropriate status and draws it
            DrawMenu();
            break;
        case GameState::GAMEPLAY:
            DrawGameplay();
            break;
        case GameState::GAMEOVER:
            DrawGameOver();
            break;
        }

        EndDrawing();
    }
}

// Function Defination For Cleanup
void Game::Cleanup()
{
    CloseWindow();      //CloseWindow: Close window and unload OpenGL context      
}

// Function Defination For UpdateMenu
void Game::UpdateMenu()
{
    // Check for button clicks
    if (IsKeyPressed(KEY_ENTER))        // IsKeyPressed: Check if ENTER key has been pressed once
    {
        currentState = GameState::GAMEPLAY;
    }
    if (IsKeyPressed(KEY_ESCAPE))       // IsKeyPressed: Check if ESCAPE key has been pressed once
    {
        exitGame = true;
    }
}

// Function Defination For DrawMenu
void Game::DrawMenu()
{
    DrawText("2D FIGHTING GAME", 200, 100, 40, BLACK);      // DrawText: Draw text (using default font)
    DrawText("Press ENTER to start", 250, 200, 20, DARKGRAY);
    DrawText("Press ESC to exit", 250, 230, 20, DARKGRAY);
}

// Function Defination For UpdateGameplay
void Game::UpdateGameplay()
{
    // Will add gameplay logic later

    // For now, just a way to return to menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        currentState = GameState::MENU;     // First ESCAPE key Press takes to MENU then second ESCAPE key press exits the game
    }
}

// Function Defination For DrawGameplay
void Game::DrawGameplay()
{
    DrawText("GAMEPLAY SCREEN", 250, 200, 20, BLACK);   // For Check(for now)
    DrawText("Press ESC to return to menu", 200, 250, 20, DARKGRAY);        
}

// Function Defination For UpdateGameOver
void Game::UpdateGameOver()
{
    if (IsKeyPressed(KEY_ENTER))        // YAHAN MSLA LG RHA HAI: ENTER KEY TAKES BACK TO MENU???
    {
        currentState = GameState::MENU;
    }
}

// // Function Defination For DrawGameOver
void Game::DrawGameOver()
{
    DrawText("GAME OVER", 250, 200, 40, RED);
    DrawText("Press ENTER to return to menu", 200, 250, 20, DARKGRAY);
}