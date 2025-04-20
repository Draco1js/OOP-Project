#include "Player.hpp"
#include <SFML/Window.hpp>
#include <iostream>

Player::Player(float x, float y, bool isPlayer)
    : Fighter(x, y, isPlayer)
{
    groundLevel = y; // Set the ground level to the initial y position

}

void Player::update(float deltaTime)
{
    // Very obvious debug statement that should appear if the function is called
    std::cout << "==== PLAYER UPDATE CALLED ====" << std::endl << std::flush;
    
    // Get current position
    sf::Vector2f position = sprite.getPosition();

    // Debug output BEFORE applying movement
    std::cout << "Player " << (isPlayer ? "1" : "2") << " velocity: " << velocity.x 
              << ", deltaTime: " << deltaTime << std::endl << std::flush;

    // Apply horizontal velocity - adjust the scaling factor to make movement more noticeable
    position.x += velocity.x * deltaTime;
    
    // Apply gravity if not on ground or if jumping
    if (!isOnGround || velocity.y < 0)
    {
        velocity.y += gravity * deltaTime * 60.0f; // Scale gravity by deltaTime
        position.y += velocity.y;

        // Check if landed on ground
        if (position.y >= groundLevel)
        {
            position.y = groundLevel;
            velocity.y = 0;
            isOnGround = true;

            // If we were jumping, go back to idle
            if (currentState == FighterState::Jump)
            {
                setAnimation(FighterState::Idle);
            }
        }
        else
        {
            isOnGround = false;
        }
    }

    // Update sprite position
    sprite.setPosition(position);

    // Update animation state based on velocity and current state
    if (!isAttacking && !isBlocking && isOnGround)
    {
        // Fix facing direction logic based on player type
        if (velocity.x > 0)
        {
            // Moving right
            if (isPlayer)
            {
                setAnimation(FighterState::WalkForward);
                facingRight = true; // Player 1 faces right when moving right
            }
            else
            {
                setAnimation(FighterState::WalkBackward);
                facingRight = true; // FIXED: Player 2 faces right when moving right
            }
        }
        else if (velocity.x < 0)
        {
            // Moving left
            if (isPlayer)
            {
                setAnimation(FighterState::WalkBackward);
                facingRight = false; // Player 1 faces left when moving left
            }
            else
            {
                setAnimation(FighterState::WalkForward);
                facingRight = false; // FIXED: Player 2 faces left when moving left
            }
        }
        else
        {
            setAnimation(FighterState::Idle);
            // Keep current facing direction when idle
        }
    }

    // Call base class update to handle animation
    Fighter::update(deltaTime);

    // Reset attack/block flags if animation finished
    if (AnimationManager::isAnimationFinished(currentAnimationName))
    {
        if (isAttacking)
        {
            isAttacking = false;
            setAnimation(FighterState::Idle);
        }
        if (isBlocking)
        {
            isBlocking = false;
            setAnimation(FighterState::Idle);
        }
    }
}

void Player::jump()
{
    if (isOnGround && !isAttacking && !isBlocking)
    {
        velocity.y = jumpForce;
        isOnGround = false;
        setAnimation(FighterState::Jump);
    }
}

void Player::moveLeft()
{
    if (!isAttacking && !isBlocking)
    {
        velocity.x = -moveSpeed;
        facingRight = false; // Both players face left when moving left
        std::cout << "Moving left, velocity set to: " << velocity.x << std::endl << std::flush;
    }
}

void Player::moveRight()
{
    if (!isAttacking && !isBlocking)
    {
        velocity.x = moveSpeed;
        facingRight = true; // Both players face right when moving right
        std::cout << "Moving right, velocity set to: " << velocity.x << std::endl << std::flush;
    }
}

void Player::stopMovingHorizontal()
{
    velocity.x = 0.0f;
}

void Player::punch()
{
    if (isOnGround && !isAttacking && !isBlocking)
    {
        isAttacking = true;
        setAnimation(FighterState::Punch);
        // Create and store the attack (medium punch by default)
        currentAttack = std::make_unique<Attack>(FighterState::Punch, AttackStrength::Medium);
        // Stop horizontal movement during attack
        velocity.x = 0;
    }
}

void Player::kick()
{
    if (isOnGround && !isAttacking && !isBlocking)
    {
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
    if (isOnGround && !isAttacking && !isBlocking)
    {
        isBlocking = true;
        setAnimation(FighterState::Block);
        // Stop horizontal movement during block
        velocity.x = 0;
    }
}

// Add a new method to check for hit against another fighter
bool Player::checkAttackHit(Fighter &target)
{
    if (isAttacking && currentAttack)
    {
        return currentAttack->execute(*this, target);
    }
    return false;
}
