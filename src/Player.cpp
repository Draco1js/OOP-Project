#include "Player.hpp"
#include <SFML/Window.hpp>

Player::Player(float x, float y, bool isPlayer)
	: Fighter(x, y, isPlayer) {}

void Player::update()
{
	handleInput();
}

void Player::handleInput()
{
	// Debug output to check if this method is being called
	std::cout << "handleInput called for " << (isPlayer ? "Player 1" : "Player 2") << std::endl;

	bool moved = false;
	sf::Vector2f currentPos = sprite.getPosition();

	// Print position before any movement
	std::cout << (isPlayer ? "Player 1" : "Player 2") << " position BEFORE input: "
			  << currentPos.x << ", " << currentPos.y << std::endl;

	if (isPlayer)
	{
		// Debug output to check key states
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			std::cout << "A key is pressed" << std::endl;
			currentPos.x -= 5.0f; // Move left
			moved = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			std::cout << "D key is pressed" << std::endl;
			currentPos.x += 5.0f; // Move right
			moved = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			std::cout << "W key is pressed" << std::endl;
			currentPos.y -= 5.0f; // Move up
			moved = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			std::cout << "S key is pressed" << std::endl;
			currentPos.y += 5.0f; // Move down
			moved = true;
		}

		if (moved)
		{
			sprite.setPosition(currentPos);
			std::cout << "Player 1 moved to: " << currentPos.x << ", " << currentPos.y << std::endl
					  << std::flush;
		}
	}
	else
	{
		// Debug output to check key states
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			std::cout << "Left key is pressed" << std::endl;
			currentPos.x -= 5.0f; // Move left
			moved = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			std::cout << "Right key is pressed" << std::endl;
			currentPos.x += 5.0f; // Move right
			moved = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			std::cout << "Up key is pressed" << std::endl;
			currentPos.y -= 5.0f; // Move up
			moved = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			std::cout << "Down key is pressed" << std::endl;
			currentPos.y += 5.0f; // Move down
			moved = true;
		}

		if (moved)
		{
			sprite.setPosition(currentPos);
			std::cout << "Player 2 moved to: " << currentPos.x << ", " << currentPos.y << std::endl
					  << std::flush;
		}
	}

	// Print position after movement logic
	std::cout << (isPlayer ? "Player 1" : "Player 2") << " position AFTER input: "
			  << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
}
