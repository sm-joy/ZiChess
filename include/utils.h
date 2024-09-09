#ifndef UTILS_H
#define UTILS_H

#include "Graphics.h"
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct {
    Uint32 FpsStartTime, frameCount, TimerStartTime;
    float fps;
} FPSCounter;

void FPSCounter_Init(FPSCounter* fpsCounter);
void FPSCounter_Update(FPSCounter* fpsCounter);
bool FPSCounter_SecondsPassed(FPSCounter* fpsCounter, int seconds);


SDL_Texture* loadTexture(RenderContext* rc, const char* pFilePath);
bool addTexture(RenderContext* rc, SDL_Texture* tex);
SDL_Texture* createboardTexture(RenderContext* rc);
char* getAbsolutePath(const char* relativePath);
int randint(int min, int max);

#endif // UTILS_H
