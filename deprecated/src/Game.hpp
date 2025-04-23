#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "HealthBar.hpp"
#include "Animation.hpp"

class Game {
public:
    Game();
    ~Game();
    void run();
private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void checkCollisions();

    sf::RenderWindow window;
    Player player1, player2;
    HealthBar healthBar1, healthBar2;
    sf::Clock clock;
    
    // Background elements
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;  // Will be initialized in the constructor
};

