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


![UML](https://www.plantuml.com/plantuml/dpng/TLLDRnen4BtpAqOvLAI5IEMOGnG2A44gIKIJIZsQtGbOUEsLxUMZAFtj7S-qouQbKXJiPy_7U_Cmjo4g7wl2T3AZGe1xLI0SEW1zNdwFAY9abVTe8s_uQE6S0TpfI7S4UJPgZnvA_lZQXWOCQHjXgwr5pzivPb7PfK6-T5V5wEp9GIeY1tM2W8QECM_0BxYBbSV161OgMo-zgsmk6BOASOCFg4nStIb_zdZGFAQ_Bwn5zvANlnlhZTCvl9QvYX-GiLTRsOjIqp0HhUGMrso4pj1MT722fkkfdVCdJnlqnnCfUPA7iLQM6c_d6AqmMmzZ90b7pXWTjBEXUM6E0UF_CjQQfP3tJeUQAMeFYQTcayTN-hWVpYRFtuO__owVVapcAK0CAAw1mybWFAqOeiwSXopzv_sRSOeyKIBcjKDMtChHgJhS6PUjjLqsiH7P95kTW49Vm6PznVY3ZWktg-i8EAB8t6tj2Jokl8hQdS8lka1VO-AtnwfOOEqrKQ0hn7OzaCM-rwJUQqMt3IbvFe4hIsVHnlC6FLAynvWsv_1XW_r7OzTwDVt_ejOuLeLQOfU95wxwLpxjAQ6OHM8QtcgH_lXrUL0sDpYrPHNRtmhff8q_kw3J7CIL_Bxr-pT7PmhqkT-7Yy45Y6Y2NLtT76NeTAoB25ulLn7S6ySdIojjOPbkYttZIaKeb2NkuRf-HY6aIW6w1Tggn_VJx6G5RiFZgv_C0CIJCg7sII_qAai40bzvdG8Qf3oHqedHOQ5ywC62tyDmUQngd2kXMx4Cd3EdkLzsqER4fSL8YbEqB9g4LYoy48APst1zpfWv6xqpqfkC00hCA41d0igefJnxWj7wWO4S8pA_OrzSMQGzi4CEIJ-TlRDz2i9bc7o3fYHiho7pc6GMOkHGqgCWPfo9yaoerJsKZf8mBQ4pjMGFPVGl3E3LtkX5-edAtLRwb9GoA6sf3vi9Wtbb1MQPXQh8HCgXibLP4xgbHLMOFm00)

``` mermaid
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


###### To generate the UML diagram: https://www.plantuml.com/plantuml/uml/~h407374617274756d6c0a416c6963652d3e426f62203a204920616d207573696e67206865780a40656e64756d6c