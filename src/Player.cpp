
#include "Player.h"

Player::Player()
{
	x = 100;
	y = 100;
	width = 50;
	height = 50;
	speed = 5;
	health = 100;
	isPlayer1 = true;
	
	// Initialize gravity variables
	velocityY = 0;
	gravity = 0.5f;
	isOnGround = false;
	groundLevel = GetScreenHeight() - 50;
}

Player::Player(int startX, int startY, bool isFirstPlayer)
{
	x = startX;
	y = startY;
	width = 50;
	height = 50;
	speed = 5;
	health = 100;
	isPlayer1 = isFirstPlayer;
	
	// Initialize gravity variables
	velocityY = 0;
	gravity = 0.5f;
	isOnGround = false;
	groundLevel = GetScreenHeight() - 50;
}

void Player::HandleInput()
{
	if (isPlayer1) {
		// WASD controls for player 1
		if (IsKeyDown(KEY_D)) x += speed;
		if (IsKeyDown(KEY_A)) x -= speed;
		
		// Jump with W key
		if (IsKeyDown(KEY_W) && isOnGround) {
			Jump();
		}
	} else {
		// Arrow keys for player 2
		if (IsKeyDown(KEY_RIGHT)) x += speed;
		if (IsKeyDown(KEY_LEFT)) x -= speed;
		
		// Jump with Up key
		if (IsKeyDown(KEY_UP) && isOnGround) {
			Jump();
		}
	}
}

void Player::Jump()
{
	velocityY = -12.0f; // Negative velocity means upward movement
	isOnGround = false;
}

void Player::Update()
{
	HandleInput();
	
	// Apply gravity
	velocityY += gravity;
	y += velocityY;
	
	// Check if player has landed on the ground
	if (y + height >= groundLevel) {
		y = groundLevel - height;
		velocityY = 0;
		isOnGround = true;
	} else {
		isOnGround = false;
	}
	
	// Keep player within screen bounds
	const int screenWidth = GetScreenWidth();
	
	if (x < 0) x = 0;
	if (x + width > screenWidth) x = screenWidth - width;
	
	// Only restrict top boundary, not bottom (gravity handles that)
	if (y < 0) y = 0;
}

void Player::Draw()
{
	// Draw player with different colors based on which player it is
	Color playerColor = isPlayer1 ? RED : BLUE;
	DrawRectangle(x, y, width, height, playerColor);
}
