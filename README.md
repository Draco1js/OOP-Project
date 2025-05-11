# OOP Project

## TODO:

- [x] FEATURE: Implement Scene Switching
- [x] FEATURE: Implement Players
- [x] FEATURE: Implement Enemies
- [ ] FEATURE: Implement Animation state machine
- [x] FEATURE: Implement Fighting mechanics
- [x] BUG: Player 0 (blue) wins every time
- [x] BUG: Blocking is buggy when crouching, refactor to have separate key
- [x] BUG: Duck-punch does not work
- [ ] FEATURE: when attacking top-left text should be removed.
- [ ] FEATURE: Duck attack not working
- [ ] FEATURE: Animations and spritesheets

### Members:

- Aayan Sultan (24K-2015)
- Zumair Shamsi (24K-2040)
- Devish Kumar (24K-????)


### Project Work Division:

- Devish:
	- Setup movement controls
	- Setup Fonts and Menus
- Zumair:
	- Game Loop
	- Game Scenes
	- Window Creation
- Aayan:
	- Setup player mechanics
	- Setup hitboxes
	- Setup Physics
	- Setup Fighting mechanics
	- Todo: Setup Animation state machine


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