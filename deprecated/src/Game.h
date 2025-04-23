// Prevents Header From Including More Than Once In  A Program
#pragma once

// Includes Raylib Library
#include "raylib.h"

// Game Class
class Game
{

// Private Access Modifier Used To Ensure Encapsulation    
private:

    const int screenWidth = 800; // will not change
    const int screenHeight = 450;   // will not change
    bool exitGame;  // Flag To Track If User Exits

    // Game states Made To Track Status Of Game
    enum class GameState
    {
        MENU,   // Tracks Menu For Update And Draw
        GAMEPLAY,   // Tracks GAMEPLAY For Update And Draw
        GAMEOVER    // Tracks GAMEOVER For Update And Draw
    };

    // Instance Which Will Carry Status
    GameState currentState;

    // Private methods for each state
    void UpdateMenu();
    void DrawMenu();
    void UpdateGameplay();
    void DrawGameplay();
    void UpdateGameOver();
    void DrawGameOver();

public:
// Constructor
    Game();

    //Destructor
    ~Game();

    // Main Logic Functions
    void Initialize();
    void RunGameLoop();
    void Cleanup();
};