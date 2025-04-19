#include "Player.hpp"
#include <SFML/Window.hpp>
#include <iostream>

Player::Player(float x, float y, bool isPlayer)
	: Fighter(x, y, isPlayer) {
	groundLevel = y; // Set the ground level to the initial y position
}

void Player::update(float deltaTime)
{
    // Get current position
    sf::Vector2f position = sprite.getPosition();
    
    // Apply horizontal velocity
    position.x += velocity.x * deltaTime;
    
    // Apply gravity if not on ground or if jumping
    if (!isOnGround || velocity.y < 0) {
        velocity.y += gravity * deltaTime * 60.0f; // Scale gravity by deltaTime
        position.y += velocity.y;
        
        // Check if landed on ground
        if (position.y >= groundLevel) {
            position.y = groundLevel;
            velocity.y = 0;
            isOnGround = true;
        } else {
            isOnGround = false;
        }
    }
    
    // Update sprite position
    sprite.setPosition(position);
}

void Player::jump()
{
	if (isOnGround) {
		velocity.y = jumpForce;
		isOnGround = false;
	}
}

void Player::moveLeft()
{
	velocity.x = -moveSpeed;
}

void Player::moveRight()
{
	velocity.x = moveSpeed;
}

void Player::stopMovingHorizontal()
{
	velocity.x = 0.0f;
}
