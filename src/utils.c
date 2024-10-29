#include "../include/Utils.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <SDL.h>

int randint(int min, int max) {
    if (min > max) {
        return min;
    }

    //xorshift32 algorithm
    uint32_t seed = (uint32_t)time(NULL);
    seed ^= (seed << 13);
    seed ^= (seed >> 17);
    seed ^= (seed << 5);

    uint32_t range = max - min + 1;
    return ((int)(seed % range) + min);
}

int Utils_Abs(int x) {
    return (x < 0) ? -x : x;
}


char* getAbsolutePath(const char* relativePath) {
    if (!relativePath) {
        LogNullError("Relative Path", NULL);
        return NULL;
    }

    char* basePath = SDL_GetBasePath();
    if (basePath == NULL) {
        fprintf(stderr, "Error getting working directory path!\n");
        return NULL;
    }

    size_t size = strlen(relativePath) + strlen(basePath) + 1;
    char* fullPath = (char*)malloc(size * sizeof(char));
    if (fullPath == NULL) {
        fprintf(stderr, "Error allocating memory for absolute path!\n");
        SDL_free(basePath);
        return NULL;
    }

    strcpy_s(fullPath, size, basePath);
    strcat_s(fullPath, size, relativePath);
    SDL_free(basePath);

    return fullPath;
}

void Clock_Init(Clock* clock) {
    clock->fpsStartTime = SDL_GetTicks();
    clock->timerStartTime = clock->fpsStartTime;
    clock->lastFrameTime = clock->fpsStartTime;

    clock->frameCount = 0;
    clock->fps = 0.0f;
    clock->dt = 0.0f;
}

void Clock_Update(Clock* clock, int targetFps) {
    clock->frameCount++;

    uint32_t currentTime = SDL_GetTicks();
    uint32_t timedelta = currentTime - clock->lastFrameTime;
    clock->dt = timedelta / 1000.0f;

    clock->lastFrameTime = currentTime;

    // Calculate FPS
    uint32_t frameTime = currentTime - clock->fpsStartTime;
    if (frameTime >= 1000) {
        clock->fps = (float)clock->frameCount * 1000.0f / frameTime;
        clock->frameCount = 0;
        clock->fpsStartTime = currentTime;
    }

    if (targetFps > 0) {
        uint32_t targetFrameTime = 1000 / targetFps;
        if (timedelta < targetFrameTime) {
            uint32_t delay = targetFrameTime - timedelta;
            SDL_Delay(delay);
            //printf("%02d ", delay);
        }
    }
}


bool Clock_TimePassed(Clock* clock, int milliseconds) {
    uint32_t currentTime = SDL_GetTicks();

    if ((currentTime - clock->timerStartTime) >= (uint32_t)milliseconds) {
        clock->timerStartTime = currentTime;
        return true;
    }
    return false;
}




//debug

void LogNullErrorEx(const char* baseFunctionName, const char* objectName, const char* additionalMsg) {
    fprintf(stderr, "[%s] %s is NULL!\n", baseFunctionName, objectName);
    if (additionalMsg) fprintf(stderr, "%s\n", additionalMsg);
}