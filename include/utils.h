#ifndef UTILS_H
#define UTILS_H

#include "renderer.h"
#include "settings.h"
#include <stdbool.h>
#include <string.h>

char* duplicateString(const char* str);

SDL_Texture* loadTexture(Renderer* renderer, const char* pFilePath);
bool addTexture(Renderer* renderer, SDL_Texture* tex);
bool addText(Renderer* renderer, Text* textobj);
SDL_Texture* createboardTexture(Renderer* renderer);
SDL_Texture* createTextTxture(Renderer* renderer, const char* text, SDL_Color* color);
Text* createText(Renderer* renderr, const char* text, SDL_Color* color, TTF_Font* font);



#endif // UTILS_H
