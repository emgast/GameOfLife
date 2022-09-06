#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Board.h"
#include <string>
#include "Util.h"

constexpr SDL_Color WHITE = { 255, 255, 255 };

bool paused = false;
bool running = true;

int targetFPS = 4;
float msPerFrame = 1000 / targetFPS;

constexpr int MIN_FPS = 1;
constexpr int MAX_FPS = 16;

constexpr int WINDOW_VERTICAL_PADDING = 173;

constexpr int WINDOW_WIDTH = BOARD_WIDTH_PIXELS;
constexpr int WINDOW_HEIGHT = BOARD_HEIGHT_PIXELS + WINDOW_VERTICAL_PADDING;


void RenderControls(SDL_Renderer* renderer, TTF_Font* font)
{
	SDL_Rect rect = { CELL_OFFSET_X, BOARD_HEIGHT_PIXELS + CELL_OFFSET_Y };

	std::string controlStrings[5] = {
		"Up/Down arrow: +/- speed (Current: " + std::to_string(targetFPS) + "/16 FPS)",
		"Left/Right arrow (while paused): Step back/forward",
		"Left-click/Right-click: Set cells alive/dead",
		"Spacebar: Toggle pause",
		"Escape: Quit"
	};

	for (int i = 0; i < 5; i++)
	{
		Util::DrawText(renderer, font, WHITE, controlStrings[i], &rect);
		rect.y += rect.h;
	}
}

void UpdateFrameRate(int delta)
{
	if (delta > 0)
	{
		if (targetFPS < MAX_FPS)
		{
			targetFPS++;
		}
	}
	else if (delta < 0)
	{
		if (targetFPS > MIN_FPS)
		{
			targetFPS--;
		}
	}

	msPerFrame = 1000 / targetFPS;
}

void HandleInput(Board* board)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		int x = 0, y = 0;

		if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			board->PaintCells(x, y, 1);
		}
		else if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			board->PaintCells(x, y, 0);
		}

		switch (event.type)
		{
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			// quit the application
			case SDLK_ESCAPE: 
				running = false;
				break;
			// pause the game
			case SDLK_SPACE: 
				paused = !paused;
				break;
			// increment target frame rate
			case SDLK_UP:
				UpdateFrameRate(1);
				break;
			// decrement target frame rate
			case SDLK_DOWN:
				UpdateFrameRate(-1);
				break;
			// go back one generation
			case SDLK_LEFT:
				if (paused)
				{
					board->Step(false);
				}
				break;
			// go forward one generation
			case SDLK_RIGHT:
				if (paused)
				{
					board->Step(true);
				}
				break;
			}
			break;
		case SDL_QUIT:
			running = false;
			break;
		}
	}
}

bool CapFrameRate(Uint32& lastUpdateTicks)
{
	Uint32 currentTicks = SDL_GetTicks();

	if (currentTicks - lastUpdateTicks > msPerFrame)
	{
		lastUpdateTicks = currentTicks;
		return true;
	}

	return false;
}

int main(int argc, char* argv[])
{
	/* Variable setup */
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	
	unsigned int ticks = SDL_GetTicks();

	/* Initialization */
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Failed to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init() < 0)
	{
		printf("Failed to init TTF: %s\n", TTF_GetError());
		return 1;
	}

	window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("Failed to create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		return 1;
	}

	font = TTF_OpenFont("fonts/RobotoMono-Medium.ttf", 24);
	if (!font)
	{
		printf("Failed to create font: %s\n", TTF_GetError());
		return 1;
	}

	Board board = Board(50);
	
	/* Main loop */
	while (running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		SDL_RenderClear(renderer);

		HandleInput(&board);

		bool executeThisFrame = CapFrameRate(ticks);
		if (executeThisFrame && !paused)
		{
			board.Update();
		}

		board.Render(renderer);

		RenderControls(renderer, font);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}