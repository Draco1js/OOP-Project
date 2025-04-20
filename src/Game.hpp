#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "HealthBar.hpp"

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
};

