#include "../include/Utils.h"

int randint(int min, int max) {
    if (min > max) {
        return min;
    }

    //xorshift32 algorithm
    Uint32 seed = (uint32_t)time(NULL);
    seed ^= (seed << 13);
    seed ^= (seed >> 17);
    seed ^= (seed << 5);

    Uint32 range = max - min + 1;
    return ((int)(seed % range) + min);
}



bool addTexture(RenderContext* rc, SDL_Texture* texture) {
    if (texture == NULL) {
        return false;
    }

    SDL_Texture** newptr = (SDL_Texture**)realloc(rc->loadedTextures, (rc->numTextures + 1) * sizeof(SDL_Texture*));
    if (newptr == NULL) {
        fprintf(stderr, "Error Loading Texture! Realloc Failed Error: %s\n", SDL_GetError());
        return false;
    }

    rc->loadedTextures = newptr;
    rc->loadedTextures[rc->numTextures++] = texture;
    return true;
}




SDL_Texture* loadTexture(RenderContext* rc, const char* pFilePath) {
    if (pFilePath == NULL) {
        fprintf(stderr, "Error! Texture file path is null!\n");
        return NULL;
    }

    SDL_Texture* texture = IMG_LoadTexture(rc->renderer, pFilePath);
    if (!addTexture(rc, texture)) {
        fprintf(stderr, "Error Loading '%s' Texture! Error: %s\n", pFilePath, SDL_GetError());
    }
    return texture;
}


SDL_Texture* createboardTexture(RenderContext* rc) {
    int squareSize = 74;
    int boardSize = squareSize * 8;

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    if (format == NULL) {
        fprintf(stderr, "Failed to allocate pixel format: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface* surface = SDL_CreateRGBSurface(0, boardSize, boardSize, 32,
        format->Rmask, format->Gmask, format->Bmask, format->Amask);
    SDL_FreeFormat(format);

    if (surface == NULL) {
        fprintf(stderr, "Failed to create surface: %s\n", SDL_GetError());
        return NULL;
    }

    Uint32 color1 = SDL_MapRGB(surface->format, 180, 136, 97), color2 = SDL_MapRGB(surface->format, 239, 217, 180);

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            SDL_Rect rect = { x * squareSize, y * squareSize, squareSize, squareSize };
            Uint32 color = ((x + y) % 2 == 0) ? color1 : color2;
            SDL_FillRect(surface, &rect, color);
        }
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rc->renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
    }
    else {
        addTexture(rc, texture); // for autometic free
    }

    return texture;
}


char* getAbsolutePath(const char* relativePath) {
    if (relativePath == NULL) {
        fprintf(stderr, "Error! Relative path is null!\n");
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

void FPSCounter_Init(FPSCounter* fpsCounter) {
    fpsCounter->FpsStartTime = fpsCounter->TimerStartTime = SDL_GetTicks();
    fpsCounter->frameCount = 0;
    fpsCounter->fps = 0.0f;
}

void FPSCounter_Update(FPSCounter* fpsCounter) {
    fpsCounter->frameCount++;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 TimerStartTime = currentTime - fpsCounter->FpsStartTime;

    if (TimerStartTime >= 1000) {
        fpsCounter->fps = (float)fpsCounter->frameCount * 1000.0f / TimerStartTime;
        fpsCounter->frameCount = 0;
        fpsCounter->FpsStartTime = currentTime;
    }
}

bool FPSCounter_SecondsPassed(FPSCounter* fpsCounter, int seconds) {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 TimerStartTime = currentTime - fpsCounter->TimerStartTime;
    if (TimerStartTime >= (seconds * 1000)) {
        fpsCounter->TimerStartTime = currentTime;
        return true;
    }
    return false;
}




