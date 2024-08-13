#ifndef UTILS_H
#define UTILS_H

#include "renderer.h"
#include "settings.h"
#include <stdbool.h>
#include <string.h>

char* duplicateString(const char* str);

SDL_Texture* loadTexture(Renderer* renderer, const char* pFilePath);
bool addTexture(Renderer* renderer, SDL_Texture* tex);
SDL_Texture* createboardTexture(Renderer* renderer);



#endif // UTILS_H
