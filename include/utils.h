#ifndef UTILS_H
#define UTILS_H

#include "Graphics.h"
#include <stdbool.h>
#include <string.h>
#include <time.h>


typedef struct {
    Uint32 fpsStartTime, frameCount, timerStartTime, lastFrameTime;
    double fps, dt;
} Clock;

void Clock_Init(Clock* clock);
void Clock_Update(Clock* clock, int targetFps);
bool Clock_TimePassed(Clock* clock, int miliSeconds);

SDL_Texture* loadTexture(RenderContext* rc, const char* pFilePath);
bool addTexture(RenderContext* rc, SDL_Texture* tex);
SDL_Texture* createboardTexture(RenderContext* rc);
char* getAbsolutePath(const char* relativePath);
int randint(int min, int max);

#endif // UTILS_H