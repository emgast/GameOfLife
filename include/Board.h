#pragma once
#include <string>
#include <SDL.h>
#include <array>
#include <vector>

constexpr int DEBUG_ENABLED = 0;

constexpr Uint8 BOARD_WIDTH = 25;
constexpr Uint8 BOARD_HEIGHT = 25;

constexpr Uint8 CELL_OFFSET_X = 5;
constexpr Uint8 CELL_OFFSET_Y = 5;

constexpr Uint8 CELL_RECT_SIZE = 25;
constexpr Uint8 CELL_RECT_GAP = 10;

constexpr Uint8 CELL_AGE_LIMIT = 16;

constexpr Uint8 GRID_CELL_SIZE = CELL_RECT_SIZE + CELL_RECT_GAP;

constexpr Uint16 BOARD_WIDTH_PIXELS = (BOARD_WIDTH * GRID_CELL_SIZE) + (CELL_OFFSET_X * 2) - CELL_RECT_GAP;
constexpr Uint16 BOARD_HEIGHT_PIXELS = (BOARD_HEIGHT * GRID_CELL_SIZE) + (CELL_OFFSET_Y * 2) - CELL_RECT_GAP;

using Generation = std::array<std::array<Uint8, BOARD_WIDTH>, BOARD_HEIGHT>;

class Board
{
private:
	unsigned int seed;
	int generationIndex;

	std::vector<Generation> data;

public: 
	Board(int chanceCellStartAlive);
	void DebugPrint();
	void Update();
	void Render(SDL_Renderer* renderer);
	void PaintCells(int mouseX, int mouseY, int value);
	void Step(bool stepForward);
};

