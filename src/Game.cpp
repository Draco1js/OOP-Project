#include "Game.h"
#include <cstdio>
#include <new>

Game::Game() : currentState(GameState::MENU),
               exitGame(false),
               player1(100, 400, true),
               player2(600, 400, false),
               winner(0),
               gameOverDelay(0.0f),
               selectedButton(0),
               isMainTrackPaused(false),
               startDelay(3.0f) // Initialize 3-second delay
{
    // Initialize menu buttons
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    playButton = {
        static_cast<float>(screenWidth / 2 - 100),
        static_cast<float>(screenHeight / 2 - 50),
        200.0f,
        50.0f};

    exitButton = {
        static_cast<float>(screenWidth / 2 - 100),
        static_cast<float>(screenHeight / 2 + 50),
        200.0f,
        50.0f};

    // Convert all assets to code on first run (optional)
    // ConvertAllAssetsToCode("assets");

    // Simple one-liner to load texture with code fallback
    backgroundTexture = LoadGameTexture("assets/insbg.gif");

    // Initialize health bars
    player1HealthBar = {
        20.0f,  // x position (left side)
        20.0f,  // y position (top)
        200.0f, // width
        20.0f   // height
    };

    player2HealthBar = {
        static_cast<float>(screenWidth - 220), // x position (right side)
        20.0f,                                 // y position (top)
        200.0f,                                // width
        20.0f                                  // height
    };

    // Load crowd sound
    InitAudioDevice();
    crowdSound = LoadMusicStream("assets/sprites/Assets/sound/Crowd.ogg");
    SetMusicVolume(crowdSound, 0.1f); // Set volume to 10%
    PlayMusicStream(crowdSound);

    // Load sound effects
    punchSound = LoadSound("assets/sprites/Assets/sound/Punch.ogg");
    kickSound = LoadSound("assets/sprites/Assets/sound/Kick.ogg");

    // Load main menu music
    mainTrack1 = LoadMusicStream("assets/sprites/Assets/sound/MainMenu.ogg");
    SetMusicVolume(mainTrack1, 0.2f); // Set volume to 10%
    PlayMusicStream(mainTrack1);

    // Load start and end sound effects
    startSound = LoadSound("assets/sprites/Assets/sound/Start.ogg");
    endSound = LoadSound("assets/sprites/Assets/sound/End.ogg");
}

// Destructor
Game::~Game()
{
    // Unload textures
    UnloadTexture(backgroundTexture);

    // Unload crowd sound
    UnloadMusicStream(crowdSound);
    CloseAudioDevice();

    // Unload sound effects
    UnloadSound(punchSound);
    UnloadSound(kickSound);

    // Unload main menu music
    UnloadMusicStream(mainTrack1);

    // Unload start and end sound effects
    UnloadSound(startSound);
    UnloadSound(endSound);
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
    // Update main menu music
    UpdateMusicStream(mainTrack1);

    // Handle music pause/resume with 'P' key
    if (IsKeyPressed(KEY_P))
    {
        isMainTrackPaused = !isMainTrackPaused;
        if (isMainTrackPaused)
        {
            PauseMusicStream(mainTrack1);
        }
        else
        {
            ResumeMusicStream(mainTrack1);
        }
    }

    // Keyboard navigation
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedButton = 1; // Select exit button
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedButton = 0; // Select play button
    }

    // Button activation with Enter or Space
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        if (selectedButton == 0)
        {
            StopMusicStream(mainTrack1); // Stop main menu music
            currentState = GameState::GAMEPLAY;
        }
        else
        {
            exitGame = true;
        }
    }

    // Mouse interaction (keep existing functionality)
    Vector2 mousePos = GetMousePosition();

    // Check if play button is clicked
    if (CheckCollisionPointRec(mousePos, playButton))
    {
        selectedButton = 0; // Highlight on hover
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            StopMusicStream(mainTrack1); // Stop main menu music
            currentState = GameState::GAMEPLAY;
        }
    }

    // Check if exit button is clicked
    if (CheckCollisionPointRec(mousePos, exitButton))
    {
        selectedButton = 1; // Highlight on hover
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            exitGame = true;
        }
    }
}

void Game::UpdateGameplay()
{
    // Handle 3-second start delay
    if (startDelay > 0.0f)
    {
        if (startDelay == 3.0f) // Play start sound only once
        {
            PlaySound(startSound);
        }

        char countdownText[32];
        std::snprintf(countdownText, sizeof(countdownText), "Starting in: %.1f", startDelay);
        DrawText(countdownText, GetScreenWidth() / 2 - MeasureText(countdownText, 20) / 2, GetScreenHeight() / 2, 20, WHITE);
        startDelay -= GetFrameTime();

        if (startDelay <= 0.0f)
        {
            startDelay = 0.0f; // Ensure it doesn't go negative
        }
        return; // Skip gameplay logic until delay is over
    }

    player1.Update();
    player2.Update();

    // Check for attack collisions
    CheckAttackCollisions();

    // Check for collisions between players
    if (CheckCollisionRecs(player1.GetRect(), player2.GetRect()))
    {
        // Handle collision (could implement pushing or damage)
    }

    // Check if any player has zero health
    if (player1.GetHealth() <= 0 || player2.GetHealth() <= 0)
    {
        // Play end sound
        PlaySound(endSound);

        // Set the winner based on who has health remaining
        if (player1.GetHealth() <= 0)
        {
            winner = 2; // Player 2 wins
        }
        else
        {
            winner = 1; // Player 1 wins
        }

        // Start the delay timer instead of immediately changing state
        gameOverDelay = GAME_OVER_DELAY;
    }

    // Update game over delay timer if active
    if (gameOverDelay > 0)
    {
        char timerText[32];
        std::snprintf(timerText, sizeof(timerText), "Timer: %.1f", gameOverDelay);
        DrawText(timerText, 200, 200, 12, WHITE);
        gameOverDelay -= GetFrameTime();
        if (gameOverDelay <= 0)
        {
            // Now transition to game over screen after delay
            currentState = GameState::GAMEOVER;
        }
        gameOverDelay -= 1.0f;
    }

    // Return to menu and reset game state
    if (IsKeyPressed(KEY_ESCAPE))
    {
        ResetGameplay();
        currentState = GameState::MENU;
    }

    // Update crowd sound
    UpdateMusicStream(crowdSound);
}

void Game::CheckAttackCollisions()
{
    // Check if player1 is hitting player2
    if (player1.IsAttacking())
    {
        Rectangle p1AttackHitbox = player1.GetAttackHitbox();
        Rectangle p2Rect = player2.GetRect();

        if (CheckCollisionRecs(p1AttackHitbox, p2Rect))
        {
            // Player 1 hit player 2
            int damage = 5;

            // Different damage based on attack type
            if (player1.GetCurrentState() == Player::State::KICKING)
            {
                damage = 7;
                PlaySound(kickSound); // Play kick sound
            }
            else
            {
                PlaySound(punchSound); // Play punch sound
            }

            // Check if attack is from behind (can't block attacks from behind)
            bool attackFromBehind = (player1.IsFacingRight() != player2.IsFacingRight());

            // Only apply damage if not blocking or if hit from behind
            if (!player2.IsBlocking() || attackFromBehind)
            {
                // Pass the direction the attack is coming from (player1's facing direction)
                player2.TakeDamage(damage, player1.IsFacingRight());
            }
        }
    }

    // Check if player2 is hitting player1
    if (player2.IsAttacking())
    {
        Rectangle p2AttackHitbox = player2.GetAttackHitbox();
        Rectangle p1Rect = player1.GetRect();

        if (CheckCollisionRecs(p2AttackHitbox, p1Rect))
        {
            // Player 2 hit player 1
            int damage = 5;

            // Different damage based on attack type
            if (player2.GetCurrentState() == Player::State::KICKING)
            {
                damage = 7;
                PlaySound(kickSound); // Play kick sound
            }
            else
            {
                PlaySound(punchSound); // Play punch sound
            }

            // Check if attack is from behind (can't block attacks from behind)
            bool attackFromBehind = (player2.IsFacingRight() != player1.IsFacingRight());

            // Only apply damage if not blocking or if hit from behind
            if (!player1.IsBlocking() || attackFromBehind)
            {
                // Pass the direction the attack is coming from (player2's facing direction)
                player1.TakeDamage(damage, player2.IsFacingRight());
            }
        }
    }
}

void Game::UpdateGameOver()
{
    if (IsKeyPressed(KEY_ENTER))
    {
        ResetGameplay();
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
    DrawText("GAME MENU", screenWidth / 2 - MeasureText("GAME MENU", 40) / 2, 100, 40, WHITE);

    // Draw buttons with selection highlight
    Color playButtonColor = selectedButton == 0 ? SKYBLUE : BLUE;
    Color exitButtonColor = selectedButton == 1 ? PINK : RED;

    DrawRectangleRec(playButton, playButtonColor);
    DrawRectangleRec(exitButton, exitButtonColor);

    // Draw selection indicator (triangle)
    if (selectedButton == 0)
    {
        DrawTriangle(
            {playButton.x - 20, playButton.y + playButton.height / 2},
            {playButton.x - 5, playButton.y + playButton.height / 2 - 10},
            {playButton.x - 5, playButton.y + playButton.height / 2 + 10},
            WHITE);
    }
    else
    {
        DrawTriangle(
            {exitButton.x - 20, exitButton.y + exitButton.height / 2},
            {exitButton.x - 5, exitButton.y + exitButton.height / 2 - 10},
            {exitButton.x - 5, exitButton.y + exitButton.height / 2 + 10},
            WHITE);
    }

    // Draw button text
    DrawText("PLAY",
             static_cast<int>(playButton.x + playButton.width / 2 - MeasureText("PLAY", 20) / 2),
             static_cast<int>(playButton.y + playButton.height / 2 - 10),
             20, WHITE);

    DrawText("EXIT",
             static_cast<int>(exitButton.x + exitButton.width / 2 - MeasureText("EXIT", 20) / 2),
             static_cast<int>(exitButton.y + exitButton.height / 2 - 10),
             20, WHITE);

    // Draw keyboard controls hint
    DrawText("Use UP/DOWN arrows or W/S to navigate",
             screenWidth / 2 - MeasureText("Use UP/DOWN arrows or W/S to navigate", 15) / 2,
             screenHeight - 60, 15, WHITE);
    DrawText("Press ENTER or SPACE to select",
             screenWidth / 2 - MeasureText("Press ENTER or SPACE to select", 15) / 2,
             screenHeight - 40, 15, WHITE);
}

void Game::DrawGameplay()
{
    // Draw both players
    player1.Draw();
    player2.Draw();

    // Draw health bars
    DrawHealthBars();

    // Draw ground line
    DrawLine(0, GetScreenHeight() - 50, GetScreenWidth(), GetScreenHeight() - 50, BLACK);
}

void Game::DrawGameOver()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2,
             screenHeight / 2 - 60, 40, RED);

    // Using snprintf instead of sprintf
    char winnerText[32];
    std::snprintf(winnerText, sizeof(winnerText), "PLAYER %d WINS!", winner);

    Color winnerColor = (winner == 1) ? RED : BLUE;
    DrawText(winnerText,
             screenWidth / 2 - MeasureText(winnerText, 30) / 2,
             screenHeight / 2, 30, winnerColor);

    DrawText("Press ENTER to return to menu",
             screenWidth / 2 - MeasureText("Press ENTER to return to menu", 20) / 2,
             screenHeight / 2 + 60, 20, WHITE);
}

void Game::DrawHealthBars()
{
    // Get current health values
    int p1Health = player1.GetHealth();
    int p2Health = player2.GetHealth();

    // Calculate health percentages
    float p1HealthPercent = static_cast<float>(p1Health) / 200.0f;
    float p2HealthPercent = static_cast<float>(p2Health) / 200.0f;

    // Draw player 1 health bar (background)
    DrawRectangleRec(player1HealthBar, DARKGRAY);

    // Draw player 1 health bar (foreground - current health)
    Rectangle p1CurrentHealth = player1HealthBar;
    p1CurrentHealth.width *= p1HealthPercent;
    DrawRectangleRec(p1CurrentHealth, RED);

    // Draw player 2 health bar (background)
    DrawRectangleRec(player2HealthBar, DARKGRAY);

    // Draw player 2 health bar (foreground - current health)
    Rectangle p2CurrentHealth = player2HealthBar;
    p2CurrentHealth.width *= p2HealthPercent;
    DrawRectangleRec(p2CurrentHealth, BLUE);

    // Draw health text
    char p1HealthText[32];
    char p2HealthText[32];
    std::snprintf(p1HealthText, sizeof(p1HealthText), "P1: %d/200", p1Health);
    std::snprintf(p2HealthText, sizeof(p2HealthText), "P2: %d/200", p2Health);

    DrawText(p1HealthText,
             static_cast<int>(player1HealthBar.x + 5),
             static_cast<int>(player1HealthBar.y + 2),
             16, WHITE);

    DrawText(p2HealthText,
             static_cast<int>(player2HealthBar.x + 5),
             static_cast<int>(player2HealthBar.y + 2),
             16, WHITE);
}

void Game::ResetGameplay()
{
    // Use the Reset method instead of placement new
    player1.Reset(100, 400, true);
    player2.Reset(600, 400, false);

    // Reset winner and delay timer
    winner = 0;
    gameOverDelay = 0.0f;
    startDelay = 3.0f; // Reset start delay
}
