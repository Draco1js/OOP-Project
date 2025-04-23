#pragma once
#include <raylib.h>

class Player {
	public:
		Player();
		Player(int startX, int startY, bool isFirstPlayer);
		void Update();
		void Draw();
		void HandleInput();
		
		Rectangle GetRect() const { return {(float)x, (float)y, (float)width, (float)height}; }
		
		// Jump method
		void Jump();
	private:
		int x;
		int y;
		int width;
		int height;
		int speed;
		int health;
		bool isPlayer1;
		
		// Gravity related variables
		float velocityY;
		float gravity;
		bool isOnGround;
		int groundLevel;
};
