# OOP Project: 2D Fighting Game with Raylib

## TODO:

- [x] FEATURE: Implement Scene Switching
- [x] FEATURE: Implement Players
- [x] FEATURE: Implement Enemies
- [x] FEATURE: Implement Animation state machine
- [x] FEATURE: Implement Fighting mechanics
- [x] FEATURE: Animations and spritesheets
- [x] FEATURE: CI/CD pipeline
- [x] BUG: Player 0 (blue) wins every time
- [x] BUG: Blocking is buggy when crouching, refactor to have separate key
- [x] BUG: Duck-punch does not work
- [x] BUG: Duck attack not working
- [x] BUG: Some animation states not resetting to idle

### Members:

- Aayan Sultan (24K-2015)
- Zumair Shamsi (24K-2040)
- Devish Kumar (24K-2022)

### Project Work Division:

- Devish:
  - Setup movement controls
  - Setup Fonts and Menus
  - VSCode configurations
- Zumair:
  - Game Loop
  - Game Scenes
  - Window Creation
  - Main class architecture
- Aayan:
  - Setup player mechanics
  - Setup hitboxes
  - Setup Physics
  - Setup Fighting mechanics
  - CI/CD pipeline

### Media

- The main menu
  - ![Game Menu](/assets//README/Game_Menu.png)
- Start Timer
  - ![Starting timer](/assets//README/Starting_timer.png)
- Main level/arena
  - ![Level](/assets//README/Level.png)
- Debug mode
  - ![Debug Menu](/assets//README/Debug_menu.png)

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
