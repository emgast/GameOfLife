#include "Util.h"
#include <stdlib.h>

bool Util::AttemptRandomChance(int chance)
{
    return rand() % 100 < chance ? true : false;
}

void Util::DrawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color fontColor, std::string text, SDL_Rect* rect)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), fontColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    rect->w = surface->w;
    rect->h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
