#pragma once
#include <raylib.h>
#include "Loader.h"
#include <string>

class Player
{
public:
	// Add player states
	enum class State
	{
		IDLE,
		WALKING,
		JUMPING,
		DUCKING,
		PUNCHING,
		KICKING,
		SPECIAL_ATTACK,
		BLOCKING,
		HURT
	};

	// Constructor
	Player(float x, float y, bool isPlayer1);

	// Update and draw
	void Update();
	void Draw(bool debugMode = false);

	// Actions
	void Move(float direction);
	void Jump();
	void Duck(bool shouldDuck);
	void Block(); // Updated to take no parameters
	void Punch();
	void Kick();
	void SpecialAttack();

	// Getters
	Rectangle GetRect() const;
	Rectangle GetAttackHitbox() const;
	int GetHealth() const;
	float GetWidth() const;
	bool IsAttacking() const;
	bool IsBlocking() const;
	std::string GetAnimationKey() const; // Add this method

	// Character methods
	void SetCharacter(const std::string &name); // Add this method
	std::string GetCharacterName() const;		// Add this method

	// Take damage
	void TakeDamage(int amount, bool attackFromRight);

	// Add these to the public section
	void Reset(float newX, float newY, bool isFirstPlayer);
	State GetCurrentState() const;
	bool IsFacingRight() const;

	// Add these getter methods to the Player class
	float GetX() const { return x; }
	float GetY() const { return y; }
	float GetVelocityX() const { return velocityX; }
	float GetVelocityY() const { return velocityY; }

private:
	float x, y;
	float velocityX, velocityY;
	float width, height;
	float originalHeight; // Store original height for ducking
	int health;
	bool isPlayer1;
	bool facingRight;
	bool isJumping;
	bool isDucking;
	bool isBlocking;
	State currentState;
	float attackTimer;
	float specialMeter; // For special attacks
	float stunTimer;	// For hit stun
	const float GROUND_HEIGHT = 50.0f;

	// Animation variables
	std::string characterName; // Add this
	int currentFrame;		   // Add this
	float frameTimer;		   // Add this

	// Constants
	const float MOVE_SPEED = 6.0f; // Increased from 5.0f for better movement with larger sprites
	const float JUMP_FORCE = -20.0f; // Increased from -15.0f for higher jumps
	const float GRAVITY = 1.0f; // Increased from 0.8f for better physics with larger sprites
	const float ATTACK_DURATION = 0.3f;
	const float DUCK_HEIGHT_RATIO = 0.6f; // Height when ducking
};
