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

``` uml
@startuml "Raylib Game UML"

class Game {
  - GameState currentState
  - bool exitGame
  - Player player1
  - Player player2
  - int winner
  - Rectangle playButton
  - Rectangle exitButton
  - int selectedButton
  - Texture2D backgroundTexture
  - Rectangle player1HealthBar
  - Rectangle player2HealthBar
  
  + Game()
  + ~Game()
  + void Update()
  + void Draw()
  + bool IsExitGame()
  
  - void UpdateMenu()
  - void UpdateGameplay()
  - void UpdateGameOver()
  - void DrawMenu()
  - void DrawGameplay()
  - void DrawGameOver()
  - void CheckAttackCollisions()
  - void ResetGameplay()
  - void DrawHealthBars()
}

enum GameState {
  MENU
  GAMEPLAY
  GAMEOVER
}

class Player {
  - Rectangle rect
  - Color color
  - float speed
  - int health
  - bool isBlocking
  - bool isCrouching
  - bool isAttacking
  - Rectangle attackHitbox
  - int attackCooldown
  - int attackDuration
  - int attackTimer
  - int playerNumber
  
  + Player(float x, float y, Color color, int playerNumber)
  + void Update(Player& opponent)
  + void Draw()
  + Rectangle GetRect()
  + Rectangle GetAttackHitbox()
  + bool IsAttacking()
  + void TakeDamage(int amount)
  + int GetHealth()
  + void Reset(float x, float y)
  
  - void HandleInput()
  - void UpdateAttack()
  - void UpdatePosition()
}

Game --> GameState
Game *-- "2" Player
Game ..> Rectangle

note right of Game
  Main game class that manages:
  - Game states (menu, gameplay, game over)
  - Players and their interactions
  - UI elements (health bars, buttons)
  - Game loop (update and draw)
endnote

note right of Player
  Player class that handles:
  - Movement and controls
  - Attack mechanics
  - Health and damage
  - Collision detection
endnote

class Main {
  + {static} int main()
}

Main ..> Game : creates

note bottom of Main
  Entry point that:
  - Initializes raylib window
  - Creates game instance
  - Runs game loop
  - Handles cleanup
endnote

@enduml
```