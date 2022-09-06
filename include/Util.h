#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

static class Util
{
public:
	static bool AttemptRandomChance(int chance);
	static void DrawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color fontColor, std::string text, SDL_Rect* rect);
};