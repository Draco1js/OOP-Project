#include "Game.h"
#include <cstdio>
#include <new>

Game::Game() : currentState(GameState::MENU),
               exitGame(false),
               player1(150, GetScreenHeight() - 50 - 120, true),
               player2(550, GetScreenHeight() - 50 - 120, false),
               winner(0),
               debugMode(false),
               selectedButton(0),
               isMainTrackPaused(false),
               startDelay(5.0f)
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

    // Load menu background texture
    backgroundTexture = LoadGameTexture("assets/insbg.gif");
    
    // Load level background texture
    levelBackgroundTexture = LoadGameTexture("assets/lvlbg.png");

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
    SetMusicVolume(crowdSound, 0.05f);
    PlayMusicStream(crowdSound);

    
    // Load sound effects
    confirmSound = LoadSound("assets/sprites/Assets/sound/confirm.ogg");
    punchSound = LoadSound("assets/sprites/Assets/sound/Punch.ogg");
    kickSound = LoadSound("assets/sprites/Assets/sound/Kick.ogg");

    // Load main menu music
    mainTrack1 = LoadMusicStream("assets/sprites/Assets/sound/MainMenu.ogg");
    SetMusicVolume(mainTrack1, 0.2f);
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
    UnloadTexture(levelBackgroundTexture);

    // Unload crowd sound
    UnloadMusicStream(crowdSound);
    CloseAudioDevice();

    // Unload sound effects
    UnloadSound(punchSound);
    UnloadSound(kickSound);
    UnloadSound(confirmSound);

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
    // Toggle debug mode with Backslash key
    if (IsKeyPressed(KEY_BACKSLASH))
    {
        debugMode = !debugMode;
    }

    switch (currentState)
    {
    case GameState::MENU:
        UpdateMenu();
        break;
    case GameState::GAMEPLAY:
        UpdateGameplay();
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
        PlaySound(confirmSound);
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
        // Play start sound only at the beginning
        bool soundPlayed = false;
        if (!soundPlayed && startDelay < 3.0f)
        {
            PlaySound(startSound);
            soundPlayed = true;
        }
        
        startDelay -= GetFrameTime();

        // Add ability to skip timer with space key
        if (IsKeyPressed(KEY_SPACE))
        {
            soundPlayed = true; // dont let it playa gain 
            startDelay = 0.0f;
            StopSound(startSound);
        }

        if (startDelay <= 0.0f)
        {
            startDelay = 0.0f; // Ensure it doesn't go negative
            soundPlayed = false; // Reset for next round
        }
        return; // Skip gameplay logic until delay is over
    }

    // Check if a winner has been determined
    if (winner != 0)
    {
        // Handle restart when Enter is pressed
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            ResetGameplay(); // Reset the game state but stay in gameplay mode
        }
        
        // Add this code to handle ESC key when there's a winner
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ResetGameplay();
            currentState = GameState::MENU;
            PlayMusicStream(mainTrack1); // Restart menu music
        }
        
        return; // Skip the rest of gameplay logic if we have a winner
    }

    // Update players
    player1.Update();
    player2.Update();

    // Check for attack collisions
    CheckAttackCollisions();

    // Check for collisions between players
    if (CheckCollisionRecs(player1.GetRect(), player2.GetRect()))
    {
        // Handle collision (could implement pushing or damage)
    }

    // Check if any player has zero health and winner hasn't been determined yet
    if ((player1.GetHealth() <= 0 || player2.GetHealth() <= 0) && winner == 0)
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
    }

    // Return to menu and reset game state
    if (IsKeyPressed(KEY_ESCAPE))
    {
        ResetGameplay();
        currentState = GameState::MENU;
        PlayMusicStream(mainTrack1); // Restart menu music
    }

    // Update crowd sound
    UpdateMusicStream(crowdSound);
}

void Game::CheckAttackCollisions()
{
    // Track which players have been hit this frame
    static bool player1HitThisFrame = false;
    static bool player2HitThisFrame = false;
    
    // Reset hit flags at the beginning of each frame
    if (!player1.IsAttacking()) player1HitThisFrame = false;
    if (!player2.IsAttacking()) player2HitThisFrame = false;
    
    // Check if player1 is hitting player2
    if (player1.IsAttacking() && !player2HitThisFrame)
    {
        Rectangle p1AttackHitbox = player1.GetAttackHitbox();
        Rectangle p2Rect = player2.GetRect();

        if (CheckCollisionRecs(p1AttackHitbox, p2Rect))
        {
            // Player 1 hit player 2
            int damage = 3; // Reduced base damage for punch (was 5)

            // Different damage based on attack type
            if (player1.GetCurrentState() == Player::State::KICKING)
            {
                damage = 5; // Reduced damage for kick (was 8)
                PlaySound(kickSound); // Play kick sound
            }
            else if (player1.GetCurrentState() == Player::State::SPECIAL_ATTACK)
            {
                damage = 10; // Reduced special attack damage (was 15)
                PlaySound(kickSound); // Use kick sound for now
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
                player2HitThisFrame = true;
            }
        }
    }

    // Check if player2 is hitting player1
    if (player2.IsAttacking() && !player1HitThisFrame)
    {
        Rectangle p2AttackHitbox = player2.GetAttackHitbox();
        Rectangle p1Rect = player1.GetRect();

        if (CheckCollisionRecs(p2AttackHitbox, p1Rect))
        {
            // Player 2 hit player 1
            int damage = 3; // Reduced base damage for punch (was 5)

            // Different damage based on attack type
            if (player2.GetCurrentState() == Player::State::KICKING)
            {
                damage = 5; // Reduced damage for kick (was 8)
                PlaySound(kickSound); // Play kick sound
            }
            else if (player2.GetCurrentState() == Player::State::SPECIAL_ATTACK)
            {
                damage = 10; // Reduced special attack damage (was 15)
                PlaySound(kickSound); // Use kick sound for now
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
                player1HitThisFrame = true;
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

   
    // Draw button text
    DrawText("PLAY",
             static_cast<int>(playButton.x + playButton.width / 2 - MeasureText("PLAY", 20) / 2),
             static_cast<int>(playButton.y + playButton.height / 2 - 10),
             20, WHITE);

    DrawText("EXIT",
             static_cast<int>(exitButton.x + exitButton.width / 2 - MeasureText("EXIT", 20) / 2),
             static_cast<int>(exitButton.y + exitButton.height / 2 - 10),
             20, WHITE);

    // Create a semi-transparent panel for controls info
    DrawRectangle(screenWidth / 2 - 200, screenHeight - 140, 400, 120, ColorAlpha(BLACK, 0.7f));
    
    // Draw controls header
    DrawText("CONTROLS", 
             screenWidth / 2 - MeasureText("CONTROLS", 18) / 2,
             screenHeight - 135, 18, GOLD);
    
    // Draw keyboard controls with improved formatting
    DrawText("Navigation: UP/DOWN arrows or W/S",
             screenWidth / 2 - 180,
             screenHeight - 110, 15, WHITE);
             
    DrawText("Select: ENTER or SPACE",
             screenWidth / 2 - 180,
             screenHeight - 90, 15, WHITE);
             
    DrawText("Music Toggle: P",
             screenWidth / 2 - 180,
             screenHeight - 70, 15, WHITE);
             
    DrawText("Debug Mode: BACKSLASH (\\)",
             screenWidth / 2 - 180,
             screenHeight - 50, 15, WHITE);
             
    // Display debug mode status
    const char* debugStatus = debugMode ? "ON" : "OFF";
    DrawText(TextFormat("Debug Mode: %s", debugStatus),
             screenWidth / 2 + 50,
             screenHeight - 50, 15, debugMode ? GREEN : GRAY);
    
    // Display debug information if enabled
    if (debugMode)
    {
        // Create a semi-transparent background for debug info
        DrawRectangle(10, screenHeight - 130, 300, 120, ColorAlpha(BLACK, 0.5f));
        
        // Draw debug mode status
        DrawText("Debug Mode: ON (Press \\ to toggle)", 20, screenHeight - 120, 16, YELLOW);
        
        // Draw FPS counter
        DrawText(TextFormat("FPS: %d", GetFPS()), 20, screenHeight - 100, 16, GREEN);
        
        // Draw game state
        DrawText("Game State: MENU", 20, screenHeight - 80, 16, WHITE);
        
        // Draw selected button info
        DrawText(TextFormat("Selected Button: %s", selectedButton == 0 ? "PLAY" : "EXIT"), 
                 20, screenHeight - 60, 16, WHITE);
                 
        // Draw music state
        DrawText(TextFormat("Music: %s", isMainTrackPaused ? "PAUSED" : "PLAYING"), 
                 20, screenHeight - 40, 16, WHITE);
    }
}

void Game::DrawGameplay()
{
    // Draw level background with scaling to fit screen
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Calculate scaling factors to fit the screen
    float scaleX = (float)screenWidth / levelBackgroundTexture.width;
    float scaleY = (float)screenHeight / levelBackgroundTexture.height;
    float scale = scaleX > scaleY ? scaleX : scaleY;

    // Draw the scaled background
    DrawTextureEx(levelBackgroundTexture, {0, 0}, 0.0f, scale, WHITE);

    // Draw both players with debug mode
    player1.Draw(debugMode);
    player2.Draw(debugMode);

    // Draw health bars
    DrawHealthBars();

    // Draw ground line
    if (debugMode)
    {
        DrawLine(0, GetScreenHeight() - 50, GetScreenWidth(), GetScreenHeight() - 50, BLACK);
    }
    
    // Draw countdown text during start delay
    if (startDelay > 0.0f)
    {
        // Create a larger semi-transparent background for countdown and controls
        int countdownWidth = 500;
        int countdownHeight = 350;
        DrawRectangle(
            GetScreenWidth() / 2 - countdownWidth / 2,
            GetScreenHeight() / 2 - countdownHeight / 2,
            countdownWidth, countdownHeight,
            ColorAlpha(BLACK, 0.7f)
        );
        
        // Draw countdown timer
        char countdownText[32];
        std::snprintf(countdownText, sizeof(countdownText), "Starting in: %.1f", startDelay);
        DrawText(countdownText, 
                 GetScreenWidth() / 2 - MeasureText(countdownText, 24) / 2, 
                 GetScreenHeight() / 2 - 140, 
                 24, WHITE);
        
        // Draw skip instruction
        const char* skipText = "Press SPACE to skip";
        DrawText(skipText,
                 GetScreenWidth() / 2 - MeasureText(skipText, 16) / 2,
                 GetScreenHeight() / 2 - 100,
                 16, GOLD);
                 
        // Draw controls header
        DrawText("CONTROLS",
                 GetScreenWidth() / 2 - MeasureText("CONTROLS", 22) / 2,
                 GetScreenHeight() / 2 - 70,
                 22, WHITE);
        
        // Draw divider line
        DrawLine(
            GetScreenWidth() / 2 - countdownWidth / 2 + 20,
            GetScreenHeight() / 2 - 40,
            GetScreenWidth() / 2 + countdownWidth / 2 - 20,
            GetScreenHeight() / 2 - 40,
            LIGHTGRAY
        );
        
        // Player 1 controls (left column)
        DrawText("PLAYER 1", 
                 GetScreenWidth() / 2 - countdownWidth / 2 + 50, 
                 GetScreenHeight() / 2 - 20, 
                 20, RED);
        
        DrawText("Move: W A S D", 
                 GetScreenWidth() / 2 - countdownWidth / 2 + 50, 
                 GetScreenHeight() / 2 + 10, 
                 16, WHITE);
                 
        DrawText("Block: B", 
                 GetScreenWidth() / 2 - countdownWidth / 2 + 50, 
                 GetScreenHeight() / 2 + 35, 
                 16, WHITE);
                 
        DrawText("Punch: Q", 
                 GetScreenWidth() / 2 - countdownWidth / 2 + 50, 
                 GetScreenHeight() / 2 + 60, 
                 16, WHITE);
                 
        DrawText("Kick: E", 
                 GetScreenWidth() / 2 - countdownWidth / 2 + 50, 
                 GetScreenHeight() / 2 + 85, 
                 16, WHITE);
                 
        DrawText("Special: F", 
                 GetScreenWidth() / 2 - countdownWidth / 2 + 50, 
                 GetScreenHeight() / 2 + 110, 
                 16, WHITE);
        
        // Player 2 controls (right column)
        DrawText("PLAYER 2", 
                 GetScreenWidth() / 2 + 50, 
                 GetScreenHeight() / 2 - 20, 
                 20, BLUE);
        
        DrawText("Move: Arrow Keys", 
                 GetScreenWidth() / 2 + 50, 
                 GetScreenHeight() / 2 + 10, 
                 16, WHITE);
                 
        DrawText("Block: M", 
                 GetScreenWidth() / 2 + 50, 
                 GetScreenHeight() / 2 + 35, 
                 16, WHITE);
                 
        DrawText("Punch: ,", 
                 GetScreenWidth() / 2 + 50, 
                 GetScreenHeight() / 2 + 60, 
                 16, WHITE);
                 
        DrawText("Kick: .", 
                 GetScreenWidth() / 2 + 50, 
                 GetScreenHeight() / 2 + 85, 
                 16, WHITE);
                 
        DrawText("Special: /", 
                 GetScreenWidth() / 2 + 50, 
                 GetScreenHeight() / 2 + 110, 
                 16, WHITE);
    }
    
    // Draw winner text and return to menu instructions if we have a winner
    if (winner != 0)
    {
        // Create semi-transparent overlay
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), 
                      ColorAlpha(BLACK, 0.7f));
                      
        // Display winner text
        char winnerText[32];
        std::snprintf(winnerText, sizeof(winnerText), "PLAYER %d WINS!", winner);

        Color winnerColor = (winner == 1) ? RED : BLUE;
        DrawText(winnerText,
                 GetScreenWidth() / 2 - MeasureText(winnerText, 30) / 2,
                 GetScreenHeight() / 2 - 30, 30, winnerColor);

        // Display instruction to restart the game
        DrawText("Press ENTER to play again",
                 GetScreenWidth() / 2 - MeasureText("Press ENTER to play again", 20) / 2,
                 GetScreenHeight() / 2 + 20, 20, WHITE);
                 
        // Display instruction to return to menu
        DrawText("Press ESC to return to menu",
                 GetScreenWidth() / 2 - MeasureText("Press ESC to return to menu", 16) / 2,
                 GetScreenHeight() / 2 + 50, 16, LIGHTGRAY);
    }
    
    // Display debug information if enabled
    if (debugMode)
    {
        // Create a semi-transparent background for debug info
        DrawRectangle(10, screenHeight - 130, 300, 120, ColorAlpha(BLACK, 0.5f));
        
        // Draw debug mode status
        DrawText("Debug Mode: ON (Press \\ to toggle)", 20, screenHeight - 120, 16, YELLOW);
        
        // Draw FPS counter
        DrawText(TextFormat("FPS: %d", GetFPS()), 20, screenHeight - 100, 16, GREEN);
        
        // Draw game state
        DrawText("Game State: GAMEPLAY", 20, screenHeight - 80, 16, WHITE);
        
        // Draw player information
        char p1Info[64];
        char p2Info[64];
        std::snprintf(p1Info, sizeof(p1Info), "P1: x=%.1f, y=%.1f, vx=%.1f, vy=%.1f", 
                     player1.GetX(), player1.GetY(), player1.GetVelocityX(), player1.GetVelocityY());
        std::snprintf(p2Info, sizeof(p2Info), "P2: x=%.1f, y=%.1f, vx=%.1f, vy=%.1f", 
                     player2.GetX(), player2.GetY(), player2.GetVelocityX(), player2.GetVelocityY());
        
        DrawText(p1Info, 20, screenHeight - 60, 16, RED);
        DrawText(p2Info, 20, screenHeight - 40, 16, BLUE);
    }
    
    // Display debug mode toggle notification briefly
    static bool lastDebugMode = debugMode;
    static float notificationTimer = 0.0f;
    
    if (debugMode != lastDebugMode)
    {
        lastDebugMode = debugMode;
        notificationTimer = 2.0f; // Show notification for 2 seconds
    }
    
    if (notificationTimer > 0.0f)
    {
        notificationTimer -= GetFrameTime();
        const char* debugText = debugMode ? "Debug Mode: ON" : "Debug Mode: OFF";
        
        // Draw notification with background
        int textWidth = MeasureText(debugText, 24);
        DrawRectangle(screenWidth/2 - textWidth/2 - 10, 50, textWidth + 20, 40, ColorAlpha(BLACK, 0.7f));
        DrawText(debugText, screenWidth/2 - textWidth/2, 60, 24, YELLOW);
    }
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
             
    // Add special meter bars below health bars
    
    // Player 1 special meter
    float p1SpecialMeter = player1.GetSpecialMeter();
    float p1SpecialPercent = p1SpecialMeter / 200.0f;
    
    Rectangle p1SpecialBarBg = {
        player1HealthBar.x,
        player1HealthBar.y + player1HealthBar.height + 5,
        player1HealthBar.width,
        10.0f
    };
    
    Rectangle p1SpecialBarFg = p1SpecialBarBg;
    p1SpecialBarFg.width *= p1SpecialPercent;
    
    DrawRectangleRec(p1SpecialBarBg, DARKGRAY);
    DrawRectangleRec(p1SpecialBarFg, GOLD);
    
    // Player 2 special meter
    float p2SpecialMeter = player2.GetSpecialMeter();
    float p2SpecialPercent = p2SpecialMeter / 200.0f;
    
    Rectangle p2SpecialBarBg = {
        player2HealthBar.x,
        player2HealthBar.y + player2HealthBar.height + 5,
        player2HealthBar.width,
        10.0f
    };
    
    Rectangle p2SpecialBarFg = p2SpecialBarBg;
    p2SpecialBarFg.width *= p2SpecialPercent;
    
    DrawRectangleRec(p2SpecialBarBg, DARKGRAY);
    DrawRectangleRec(p2SpecialBarFg, GOLD);
}

void Game::ResetGameplay()
{
    // Calculate ground level position
    float groundY = GetScreenHeight() - 50 - 120; // Adjusted for new player height

    // Reset players at the correct ground level
    player1.Reset(150, groundY, true);
    player2.Reset(550, groundY, false);

    // Reset winner
    winner = 0;
    startDelay = 5.0f; // Reset start delay
}
