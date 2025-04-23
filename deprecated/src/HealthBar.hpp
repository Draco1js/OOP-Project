#pragma once
// #ifndef HEALTHBAR_HPP           // Simpler Alternative is To use Pragma Once
// #define HEALTHBAR_HPP

#include <SFML/Graphics.hpp>

class HealthBar
{
public:
    HealthBar(float x, float y, int maxHealth);
    void update(int currentHealth);
    void draw(sf::RenderWindow &window);

private:
    sf::RectangleShape barBackground;
    sf::RectangleShape barForeground;
};

// #endif // HEALTHBAR_HPP