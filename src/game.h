#pragma once
#include "grid.h"
#include "blocks.cpp"

class Game
{
public:
	Game();
	~Game();
	void Draw();
	void HandleInput();
	void MoveBlockDown();
	bool gameOver;
	int score;
	Music music;

private:
	std::vector<Block> GetAllBlocks();
	bool IsBlockOutside();
	Block GetRandomBlock();
	void RotateBlock();
	void MoveBlockLeft();
	void MoveBlockRight();
	void LockBlock();
	bool BlockFits();
	void Reset();
	void UpdateScore(int linesCleared, int moveDownPoints);
	std::vector<Block> blocks;
	Block currentBlock;
	Grid grid;
	Sound rotateSound;
	Sound clearSound;
	Block nextBlock;
};