#pragma once
#include <vector>
#include <raylib.h>
#include "constants.h"

class Grid
{
public:
	Grid();
	void Initalize();
	void Print();
	void Draw();
	bool IsCellOutside(int row, int col);
	bool IsCellEmpty(int row, int col);
	int ClearFullRows();
	int grid[GAME_ROWS][GAME_COLS];

private:
	bool IsRowFull(int row);
	void ClearRow(int row);
	void MoveRowDown(int row, int numRows);
	int numRows;
	int numCols;
	int cellSize;
	std::vector<Color> colors;
};