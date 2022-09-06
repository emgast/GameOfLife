#include <iostream>
#include "Board.h"
#include "Util.h"



Board::Board(int chanceCellStartAlive)
{
	seed = time(NULL);
	srand(seed);

	generationIndex = 0;
	data = std::vector<Generation>();

	Generation firstGen = Generation();

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			firstGen[x][y] = Util::AttemptRandomChance(chanceCellStartAlive);
		}
	}

	data.push_back(firstGen);
}


void Board::DebugPrint()
{
	std::string generationString = "Generation: " + std::to_string(generationIndex);
	std::string horizontalPadding = std::string(BOARD_WIDTH * 2 - generationString.length() / 2, '-');
	std::string output = horizontalPadding;

	if (generationString.length() % 2 == 0)
	{
		output += "--";
	}
	else
	{
		output += "-";
	}

	output += generationString + horizontalPadding + "\n";

	Generation currentGen = data[generationIndex];

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		output += "|";
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			std::string value = " ";

			if (currentGen[x][y] == 0)
			{
				value += "   ";
			}
			else if (currentGen[x][y] > 0 && currentGen[x][y] < 10)
			{
				value += std::to_string(currentGen[x][y]) + "  ";
			}
			else if (currentGen[x][y] > 9)
			{
				value += std::to_string(currentGen[x][y]) + " ";
			}

			output += value;
		}
		output += "|\n";
	}

	output += std::string(BOARD_WIDTH * 4 + 2, '-') + "\n\n";

	printf(output.c_str());
}


void Board::Update()
{
	Generation currentGen = data[generationIndex];
	Generation nextGen = Generation();

	int leftX = 0, rightX = 0, topY = 0, bottomY = 0, nextAge = 0;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			leftX = x > 0 ? -1 : 0;
			rightX = x < BOARD_WIDTH - 1 ? 1 : 0;
			topY = y > 0 ? -1 : 0;
			bottomY = y < BOARD_HEIGHT - 1 ? 1 : 0;

			Uint8 aliveNeighbors = 0;

			for (int j = topY; j <= bottomY; j++)
			{
				for (int k = leftX; k <= rightX; k++)
				{
					aliveNeighbors += currentGen[x + k][y + j] ? 1 : 0;
				}
			}

			aliveNeighbors -= currentGen[x][y] ? 1 : 0;

			if ((currentGen[x][y] && aliveNeighbors == 2) || aliveNeighbors == 3)
			{
				nextAge = currentGen[x][y] + 1;
			}
			else
			{
				nextAge = 0;
			}

			nextGen[x][y] = nextAge > CELL_AGE_LIMIT ? CELL_AGE_LIMIT : nextAge;
		}
	}

	data.push_back(nextGen);
	generationIndex++;

	if (DEBUG_ENABLED) DebugPrint();
}


void Board::Render(SDL_Renderer* renderer)
{
	Generation currentGen = data[generationIndex];

	Uint8 r = 0, g = 0, b = 0;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			r = ((float)currentGen[x][y] / CELL_AGE_LIMIT) * 255;
			b = r > 0 ? 255 - r : 0;

			Uint16 cellX = (GRID_CELL_SIZE * x) + CELL_OFFSET_X;
			Uint16 cellY = (GRID_CELL_SIZE * y) + CELL_OFFSET_Y;

			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			SDL_Rect rect = { cellX, cellY, CELL_RECT_SIZE, CELL_RECT_SIZE };
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_Rect rect = { 0, 0, BOARD_WIDTH_PIXELS, BOARD_HEIGHT_PIXELS };
	SDL_RenderDrawRect(renderer, &rect);
}


void Board::PaintCells(int mouseX, int mouseY, int value)
{
	Uint16 cellX = mouseX / GRID_CELL_SIZE;
	Uint16 cellY = mouseY / GRID_CELL_SIZE;

	if (cellX >= BOARD_WIDTH || cellY >= BOARD_HEIGHT) return;

	std::vector<Generation>::iterator eraseStart = data.begin() + generationIndex + 1;
	std::vector<Generation>::iterator eraseStop = data.end();

	data.erase(eraseStart, eraseStop);

	data[generationIndex][cellX][cellY] = value;
}


void Board::Step(bool stepForward)
{
	if (stepForward)
	{
		if (generationIndex < data.size() - 1)
		{
			generationIndex++;
		}
	}
	else
	{
		if (generationIndex > 0)
		{
			generationIndex--;
		}
	}

	if (DEBUG_ENABLED)
	{
		DebugPrint();
	}
}

