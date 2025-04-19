#pragma once
#include <SFML/Graphics.hpp>

class Fighter
{
public:
    Fighter(float x, float y, bool isPlayer);
    virtual void update(float deltaTime) {}
    virtual void draw(sf::RenderWindow &window);
    virtual void takeDamage(int damage);
    bool isAlive() const;
    int getHealth() const;

    sf::Sprite &getSprite();

protected:
    int health;
    sf::Texture texture;
    sf::Sprite sprite;
    bool isPlayer;
};
