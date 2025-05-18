# OOP Project: 2D Fighting Game with Raylib

This is the repository for the final project of my CS-1004 Object Oriented Programming course, a street-fighter 2d clone made with raylib in cpp. This project was completed and presented on the 13th of May 2025.

### Personal Objectives Set
- Implement a 2D arcade fighter using Raylib.
- Demonstrate OOP concepts by creating reusable Player, Loader and Game classes.
- Achieve cross-platform compatibility (Windows, Linux, macOS).
- Set up CI/CD for automated builds and releases.

### Group Members and Work Division

- Aayan Sultan (24K-2015)
  - Setup player mechanics
  - Setup hitboxes
  - Setup Physics
  - Setup Fighting mechanics
  - CI/CD pipeline
- Zumair Shamsi (24K-2040)
  - Game Loop
  - Game Scenes
  - Window Creation
  - Main class architecture
- Devish Kumar (24K-2022)
  - Setup movement controls
  - Setup Fonts and Menus
  - VSCode configurations

### Media

- The main menu
  
  - ![Game Menu](/assets//README/Game_Menu.png)
- Start Timer
  
  - ![Starting timer](/assets//README/Starting_timer.png)
- Main level/arena
  
  - ![Level](/assets//README/Level.png)
- Debug mode
  
  - ![Debug Menu](/assets//README/Debug_menu.png)

- Demo Clip

https://github.com/user-attachments/assets/7cfad734-e43e-4d1b-888c-87083021f6af


### UML Diagram:

```mermaid
classDiagram
    direction LR

    Main --> Game : creates
    Game *-- Player : contains 2
    Game --> GameState : uses
    Player --> State : has

    class Main {
        +int main()
    }

    class Game {
        -GameState currentState
        -bool exitGame
        -Player player1, player2
        -int winner
        -float gameOverDelay
        -const float GAME_OVER_DELAY
        -Rectangle playButton, exitButton
        -int selectedButton
        -Texture2D backgroundTexture
        -Rectangle player1HealthBar, player2HealthBar

        +Game()
        +~Game()
        +void Update()
        +void Draw()
        +bool IsExitGame()
        +GameState GetCurrentState()
        +void SetCurrentState(GameState)
        -void UpdateMenu()
        -void UpdateGameplay()
        -void UpdateGameOver()
        -void DrawMenu()
        -void DrawGameplay()
        -void DrawGameOver()
        -void CheckAttackCollisions()
        -void ResetGameplay()
        -void DrawHealthBars()
    }

    class GameState {
        <<enumeration>>
        MENU
        GAMEPLAY
        GAMEOVER
    }

    class Player {
        -Rectangle rect
        -Color color
        -float speed
        -int health
        -bool isBlocking, isCrouching
        -bool isAttacking, isFacingRight
        -Rectangle attackHitbox
        -int attackCooldown, attackDuration
        -int attackTimer, playerNumber
        -State currentState
        -float specialMeter
        -std::string characterName

        +Player(float x, float y, bool isPlayer1)
        +void Update()
        +void Draw()
        +void Move(float direction)
        +void Jump(), Duck(bool), Block()
        +void Punch(), Kick(), SpecialAttack()
        +Rectangle GetRect() const
        +Rectangle GetAttackHitbox() const
        +int GetHealth() const
        +float GetWidth() const
        +bool IsAttacking(), IsBlocking()
        +bool IsFacingRight() const
        +State GetCurrentState() const
        +void TakeDamage(int, bool attackFromRight)
        +void SetCharacter(const std::string&)
        +std::string GetCharacterName() const
        +std::string GetAnimationKey() const
        -void HandleInput()
        -void UpdateAttack()
        -void UpdatePosition()
    }

    class State {
        <<enumeration>>
        IDLE
        WALKING
        JUMPING
        DUCKING
        BLOCKING
        PUNCHING
        KICKING
        SPECIAL_ATTACK
        HURT
    }

    class Main {
        +int main()
    }

    Game --> GameState
    Game *-- "2" Player : contains
    Game ..> Rectangle : uses
    Player --> State
    Main ..> Game : creates
```
