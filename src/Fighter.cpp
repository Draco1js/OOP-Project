#include "Fighter.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Fighter::Fighter(float x, float y, bool isPlayer)
	: health(100), isPlayer(isPlayer), texture(isPlayer ? "assets/sprites/player2.png" : "assets/sprites/player1.png"), sprite(texture)
{
	// // Create a simple shape as a fallback
	// sf::Color playerColor = isPlayer ? sf::Color::Blue : sf::Color::Red;

	// shape.setSize(sf::Vector2f(70, 120));
	// shape.setFillColor(playerColor);
	// shape.setPosition(sf::Vector2f(x, y));


	sprite.setPosition(sf::Vector2f(x, y));

	// std::cout << "Fighter created with color: " << (isPlayer ? "Blue" : "Red") << " at position: " << x << ", " << y << std::endl << std::flush;
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
	// We still need to return a sprite for collision detection
	// This is a temporary solution until we refactor collision detection
	return sprite;
}
void Fighter::draw(sf::RenderWindow &window)
{
	// Print the sprite position before drawing
	std::cout << "Drawing fighter at position: " << sprite.getPosition().x << ", " 
			  << sprite.getPosition().y << std::endl;
	
	// Draw the sprite
	window.draw(sprite);
}
