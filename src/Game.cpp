#include "Game.h"

Game::Game() : currentState(GameState::MENU), exitGame(false), selectedButton(0)
{
    // Initialize menu buttons
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    playButton = {
        static_cast<float>(screenWidth/2 - 100), 
        static_cast<float>(screenHeight/2 - 50), 
        200.0f, 
        50.0f
    };
    
    exitButton = {
        static_cast<float>(screenWidth/2 - 100), 
        static_cast<float>(screenHeight/2 + 50), 
        200.0f, 
        50.0f
    };
    
    // Load background texture
    backgroundTexture = LoadTexture("assets/insbg.gif");
    
    // Initialize player 1 at left side
    player1 = Player(100, 400, true); // x, y, isPlayer1=true
    
    // Initialize player 2 at right side
    player2 = Player(600, 400, false); // x, y, isPlayer1=false
}

// Destructor
Game::~Game() 
{
    // Unload textures
    UnloadTexture(backgroundTexture);
}

Game::GameState Game::GetCurrentState()
{
	return currentState;
}

void Game::SetCurrentState(GameState state)
{
	currentState = state;
}

bool Game::IsExitGame()
{
	return exitGame;
}

void Game::Update()
{
	switch (currentState)
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
}

void Game::UpdateMenu()
{
    // Keyboard navigation
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedButton = 1; // Select exit button
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedButton = 0; // Select play button
    }
    
    // Button activation with Enter or Space
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (selectedButton == 0) {
            currentState = GameState::GAMEPLAY;
        } else {
            exitGame = true;
        }
    }
    
    // Mouse interaction (keep existing functionality)
    Vector2 mousePos = GetMousePosition();
    
    // Check if play button is clicked
    if (CheckCollisionPointRec(mousePos, playButton)) {
        selectedButton = 0; // Highlight on hover
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::GAMEPLAY;
        }
    }
    
    // Check if exit button is clicked
    if (CheckCollisionPointRec(mousePos, exitButton)) {
        selectedButton = 1; // Highlight on hover
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            exitGame = true;
        }
    }
}

void Game::UpdateGameplay()
{
    player1.Update();
    player2.Update();
    
    // Check for collisions between players
    if (CheckCollisionRecs(player1.GetRect(), player2.GetRect()))
    {
        // Handle collision (could implement pushing or damage)
    }
    
    // Example condition to go to game over
    if (IsKeyPressed(KEY_ESCAPE))
    {
        currentState = GameState::MENU;
    }
}

void Game::UpdateGameOver()
{
    if (IsKeyPressed(KEY_ENTER))
    {
        currentState = GameState::MENU;
    }
}

void Game::Draw()
{
    switch (currentState)
    {
    case GameState::MENU:
        DrawMenu();
        break;
    case GameState::GAMEPLAY:
        DrawGameplay();
        break;
    case GameState::GAMEOVER:
        DrawGameOver();
        break;
    }
}

void Game::DrawMenu()
{
    // Draw background with scaling to fit screen
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Calculate scaling factors to fit the screen
    float scaleX = (float)screenWidth / backgroundTexture.width;
    float scaleY = (float)screenHeight / backgroundTexture.height;
    
    // Draw the scaled background
    DrawTextureEx(backgroundTexture, {0, 0}, 0.0f, 
                 scaleX > scaleY ? scaleX : scaleY, WHITE);
    
    // Draw title
    DrawText("GAME MENU", screenWidth/2 - MeasureText("GAME MENU", 40)/2, 100, 40, WHITE);
    
    // Draw buttons with selection highlight
    Color playButtonColor = selectedButton == 0 ? SKYBLUE : BLUE;
    Color exitButtonColor = selectedButton == 1 ? PINK : RED;
    
    DrawRectangleRec(playButton, playButtonColor);
    DrawRectangleRec(exitButton, exitButtonColor);
    
    // Draw selection indicator (triangle)
    if (selectedButton == 0) {
        DrawTriangle(
            {playButton.x - 20, playButton.y + playButton.height/2},
            {playButton.x - 5, playButton.y + playButton.height/2 - 10},
            {playButton.x - 5, playButton.y + playButton.height/2 + 10},
            WHITE
        );
    } else {
        DrawTriangle(
            {exitButton.x - 20, exitButton.y + exitButton.height/2},
            {exitButton.x - 5, exitButton.y + exitButton.height/2 - 10},
            {exitButton.x - 5, exitButton.y + exitButton.height/2 + 10},
            WHITE
        );
    }
    
    // Draw button text
    DrawText("PLAY", 
             static_cast<int>(playButton.x + playButton.width/2 - MeasureText("PLAY", 20)/2), 
             static_cast<int>(playButton.y + playButton.height/2 - 10), 
             20, WHITE);
    
    DrawText("EXIT", 
             static_cast<int>(exitButton.x + exitButton.width/2 - MeasureText("EXIT", 20)/2), 
             static_cast<int>(exitButton.y + exitButton.height/2 - 10), 
             20, WHITE);
             
    // Draw keyboard controls hint
    DrawText("Use UP/DOWN arrows or W/S to navigate", 
             screenWidth/2 - MeasureText("Use UP/DOWN arrows or W/S to navigate", 15)/2, 
             screenHeight - 60, 15, WHITE);
    DrawText("Press ENTER or SPACE to select", 
             screenWidth/2 - MeasureText("Press ENTER or SPACE to select", 15)/2, 
             screenHeight - 40, 15, WHITE);
}

void Game::DrawGameplay()
{
    // Draw both players
    player1.Draw();
    player2.Draw();
    
    // Draw ground line
    DrawLine(0, GetScreenHeight() - 50, GetScreenWidth(), GetScreenHeight() - 50, BLACK);
    DrawText("Press ESC to return to menu", 10, 40, 20, WHITE);
}

void Game::DrawGameOver()
{
    DrawText("GAME OVER", GetScreenWidth()/2 - MeasureText("GAME OVER", 40)/2, 
             GetScreenHeight()/2 - 20, 40, RED);
    DrawText("Press ENTER to return to menu", GetScreenWidth()/2 - MeasureText("Press ENTER to return to menu", 20)/2, 
             GetScreenHeight()/2 + 40, 20, WHITE);
}
