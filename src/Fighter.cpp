#include "Fighter.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Fighter::Fighter(float x, float y, bool isPlayer) : health(100), isPlayer(isPlayer), texture(isPlayer ? "assets/sprites/player1.png" : "assets/sprites/player2.png"), sprite(texture)
{
	sprite.setPosition(sf::Vector2f(x, y));
	std::cout << "Fighter created at position: " << x << ", " << y << std::endl;
}

void Fighter::takeDamage(int damage)
{
	health -= damage;
}

bool Fighter::isAlive() const
{
	return health > 0;
}

int Fighter::getHealth() const
{
	return health;
}

sf::Sprite &Fighter::getSprite()
{
	return sprite;
}
void Fighter::draw(sf::RenderWindow &window)
{
	// Remove excessive debug output
	window.draw(sprite);
}
