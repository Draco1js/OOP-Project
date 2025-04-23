#pragma once
#include "raylib.h"
#include "Player.h"

class Game
{
private:
    enum class GameState
    {
        MENU,
        GAMEPLAY,
        GAMEOVER
    };

    GameState currentState;
    Player player1; // First player (WASD controls)
    Player player2; // Second player (Arrow keys)
    bool exitGame;
    
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

public:
    Game();
    ~Game();
    
    void Update();
    void Draw();
    
    GameState GetCurrentState();
    void SetCurrentState(GameState state);
    bool IsExitGame();
};
