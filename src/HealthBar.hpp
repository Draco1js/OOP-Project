#ifndef HEALTHBAR_HPP
#define HEALTHBAR_HPP

#include <SFML/Graphics.hpp>

class HealthBar {
public:
    HealthBar(float x, float y, int maxHealth);
    void update(int currentHealth);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape barBackground;
    sf::RectangleShape barForeground;
};

#endif // HEALTHBAR_HPP