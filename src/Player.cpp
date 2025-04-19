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
            
            // If we were jumping, go back to idle
            if (currentState == FighterState::Jump) {
                setAnimation(FighterState::Idle);
            }
        } else {
            isOnGround = false;
        }
    }
    
    // Update sprite position
    sprite.setPosition(position);
    
    // Update animation state based on velocity
    if (!isAttacking && !isBlocking && isOnGround) {
        if (velocity.x > 0) {
            setAnimation(FighterState::WalkForward);
            facingRight = true;
        } else if (velocity.x < 0) {
            setAnimation(FighterState::WalkBackward);
            facingRight = false;
        } else {
            setAnimation(FighterState::Idle);
        }
    }
    
    // Call base class update to handle animation
    Fighter::update(deltaTime);
    
    // Reset attack/block flags if animation finished
    if (animations[currentState].isFinished()) {
        isAttacking = false;
        isBlocking = false;
    }
}

void Player::jump()
{
	if (isOnGround && !isAttacking && !isBlocking) {
		velocity.y = jumpForce;
		isOnGround = false;
		setAnimation(FighterState::Jump);
	}
}

void Player::moveLeft()
{
	if (!isAttacking && !isBlocking) {
		velocity.x = -moveSpeed;
		facingRight = false;
	}
}

void Player::moveRight()
{
	if (!isAttacking && !isBlocking) {
		velocity.x = moveSpeed;
		facingRight = true;
	}
}

void Player::stopMovingHorizontal()
{
	velocity.x = 0.0f;
}

void Player::punch()
{
	if (isOnGround && !isAttacking && !isBlocking) {
		isAttacking = true;
		setAnimation(FighterState::Punch);
		 // Create and store the attack (light punch by default)
		currentAttack = std::make_unique<Attack>(FighterState::Punch, AttackStrength::Medium);
		// Stop horizontal movement during attack
		velocity.x = 0;
	}
}

void Player::kick()
{
	if (isOnGround && !isAttacking && !isBlocking) {
		isAttacking = true;
		setAnimation(FighterState::Kick);
		// Create and store the attack (medium kick by default)
		currentAttack = std::make_unique<Attack>(FighterState::Kick, AttackStrength::Medium);
		// Stop horizontal movement during attack
		velocity.x = 0;
	}
}

void Player::block()
{
	if (isOnGround && !isAttacking && !isBlocking) {
		isBlocking = true;
		setAnimation(FighterState::Block);
		// Stop horizontal movement during block
		velocity.x = 0;
	}
}

// Add a new method to check for hit against another fighter
bool Player::checkAttackHit(Fighter& target)
{
	if (isAttacking && currentAttack) {
		return currentAttack->execute(*this, target);
	}
	return false;
}
