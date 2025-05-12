#pragma once
#include "raylib.h"
#include "Loader.h"
#include "Player.h"
#include <string>

class Game
{
public:
    enum class GameState
    {
        MENU,
        GAMEPLAY
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
    bool debugMode; // Debug mode toggle

    // Menu variables
    Rectangle playButton;
    Rectangle exitButton;
    int selectedButton; // 0 = play, 1 = exit

    // Background textures
    Texture2D backgroundTexture;  // Menu background
    Texture2D levelBackgroundTexture;  // Level background

    // Background crowd sound
    Music crowdSound;

    // Sound effects
    Sound punchSound; // Sound effect for punches
    Sound kickSound;  // Sound effect for kicks
    Sound startSound; // Sound effect for game start
    Sound endSound;   // Sound effect for game end

    // Music for the main menu
    Music mainTrack1;
    bool isMainTrackPaused = false; // Tracks if main menu music is paused

    // Timer for 3-second pause before gameplay
    float startDelay = 3.0f;

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
