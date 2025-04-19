#pragma once
#include "Fighter.hpp"
#include <iostream>

class Player : public Fighter {
public:
    Player(float x, float y, bool isPlayer);
    void update(float deltaTime) override;
    void jump();
    void moveLeft();
    void moveRight();
    void stopMovingHorizontal();
    
private:
    sf::Vector2f velocity{0.0f, 0.0f};
    float gravity = 0.5f;
    float jumpForce = -12.0f;
    float moveSpeed = 200.0f;
    bool isOnGround = true;
    float groundLevel = 400.0f; // Default ground level
};

