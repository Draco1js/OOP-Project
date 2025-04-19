#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "HealthBar.hpp"

class Game {
public:
    Game();
    void run();
private:
    void processEvents();
    void update();
    void render();
    void handleInput();
    void checkCollisions();

    sf::RenderWindow window;
    Player player1, player2;
    HealthBar healthBar1, healthBar2;
    sf::Clock clock;
};

#endif