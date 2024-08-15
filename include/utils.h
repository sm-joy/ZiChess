#ifndef UTILS_H
#define UTILS_H

#include "renderer.h"
#include <stdbool.h>
#include <string.h>
#include <time.h>


SDL_Texture* loadTexture(Renderer* renderer, const char* pFilePath);
bool addTexture(Renderer* renderer, SDL_Texture* tex);
SDL_Texture* createboardTexture(Renderer* renderer);
char* getAbsolutePath(const char* relativePath);
int randint(int min, int max);

#endif // UTILS_H
