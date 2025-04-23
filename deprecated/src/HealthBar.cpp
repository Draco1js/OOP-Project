#include "HealthBar.hpp"

HealthBar::HealthBar(float x, float y, int maxHealth) {
    barBackground.setSize(sf::Vector2f(200, 20));
    barBackground.setFillColor(sf::Color::Red);
    barBackground.setPosition(sf::Vector2f(x, y));

    barForeground.setSize(sf::Vector2f(200, 20));
    barForeground.setFillColor(sf::Color::Green);
    barForeground.setPosition(sf::Vector2f(x, y));
}

void HealthBar::update(int currentHealth) {
    float healthPercentage = static_cast<float>(currentHealth) / 100.0f;
    barForeground.setSize(sf::Vector2f(200 * healthPercentage, 20));
}

void HealthBar::draw(sf::RenderWindow& window) {
    window.draw(barBackground);
    window.draw(barForeground);
}
