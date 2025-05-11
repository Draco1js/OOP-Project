#pragma once
#include "raylib.h"
#include "Loader.h"
#include "Player.h"
#include<string>

class Game
{
public:
    enum class GameState
    {
        MENU,
        GAMEPLAY,
        GAMEOVER
    };

    Game();
    ~Game();

    GameState GetCurrentState();
    void SetCurrentState(GameState state);
    bool IsExitGame();

    void Update();
    void Draw();

private:
    // Reorder these declarations to match initialization order in constructor
    GameState currentState;
    bool exitGame;  // Move this before player1
    Player player1; // First player (WASD controls)
    Player player2; // Second player (Arrow keys)
    int winner;     // 1 for player1, 2 for player2, 0 for no winner yet
    float gameOverDelay;
    const float GAME_OVER_DELAY = 3.0f;

    // Menu variables
    Rectangle playButton;
    Rectangle exitButton;
    int selectedButton; // 0 = play, 1 = exit

    // Background texture
    Texture2D backgroundTexture;

    // Private methods for each state
    void UpdateMenu();
    void UpdateGameplay();
    void UpdateGameOver();
    void DrawMenu();
    void DrawGameplay();
    void DrawGameOver();
    void CheckAttackCollisions();
    void ResetGameplay();

    // Health bar properties
    Rectangle player1HealthBar;
    Rectangle player2HealthBar;
    void DrawHealthBars();
};
