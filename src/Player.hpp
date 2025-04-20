#pragma once
#include "Fighter.hpp"
#include "Attack.hpp"
#include <memory>
#include <iostream>

class Player : public Fighter {
public:
    Player(float x, float y, bool isPlayer);
    void update(float deltaTime) override;

    // Movement methods
    void jump();
    void moveLeft();
    void moveRight();
    void stopMovingHorizontal();

    // Attack methods
    void punch();
    void kick();
    void block();

    // Check if current attack hits another fighter
    bool checkAttackHit(Fighter& target);

private:
    // Physics properties
    sf::Vector2f velocity{0.0f, 0.0f};
    float gravity = 0.5f;
    float jumpForce = -12.0f;
    float moveSpeed = 100.0f;
    float groundLevel = 400.0f; // Default ground level
    bool isOnGround = true;

    // State flags
    bool isAttacking = false;
    bool isBlocking = false;

    // Current attack being executed
    std::unique_ptr<Attack> currentAttack;
};

