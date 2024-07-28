#include "../include/utils.h"


char* duplicateString(const char* str) {
    int len = strlen(str) + 1;
    char* destStr = (char*)malloc(len);
    if (destStr == NULL) return NULL;
    strcpy_s(destStr, len-1, str);
    return destStr;
}

bool addTexture(Renderer* renderer, SDL_Texture* texture) {
    if (texture == NULL) {
        return false;
    }

    SDL_Texture** newptr = (SDL_Texture**)realloc(renderer->loadedTextures, (renderer->numTextures + 1) * sizeof(SDL_Texture*));
    if (newptr == NULL) {
        fprintf(stderr, "Error Loading Texture! Realloc Failed Error: %s\n", SDL_GetError());
        return false;
    }

    renderer->loadedTextures = newptr;
    renderer->loadedTextures[renderer->numTextures++] = texture;
    return true;
}

bool addText(Renderer* renderer, Text* textobj) {
    if (textobj == NULL) {
        return false;
    }

    Text** newptr = (Text**)realloc(renderer->createdTexts, (renderer->numTexts + 1) * sizeof(Text*));
    if (newptr = NULL) {
        fprintf(stderr, "Error Creating Text! Realloc Failed: %s\n", SDL_GetError());
        return false;
    }

    renderer->createdTexts = newptr;
    renderer->createdTexts[renderer->numTexts++] = textobj;
    return true;
}


SDL_Texture* loadTexture(Renderer* renderer, const char* pFilePath) {
    SDL_Texture* texture = IMG_LoadTexture(renderer->renderer, pFilePath);
    if (!addTexture(renderer, texture)) {
        fprintf(stderr, "Error Loading '%s' Texture! Error: %s\n", pFilePath, SDL_GetError());
    }
    return texture;
}


SDL_Texture* createboardTexture(Renderer* renderer) {
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

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer->renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
    }
    else {
        addTexture(renderer, texture); // for autometic free
    }

    return texture;
}

SDL_Texture* createTextTxture(Renderer* renderer, const char* text, SDL_Color* color) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(renderer->font, text, *color);
    if (textSurface == NULL) {
        fprintf(stderr, "Cannot Render ttf_rendertext_blended! Error: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer->renderer, textSurface);
    if (textTexture == NULL) {
        fprintf(stderr, "Cannot convert surface to texture! Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return NULL;
    }

    SDL_FreeSurface(textSurface);
    return textTexture;
}

Text* createText(Renderer* renderer, const char* text, SDL_Color* color, TTF_Font* font)
{
    Text* textobj = NULL;
    textobj = (Text*)malloc(sizeof(Text));

    if (textobj == NULL) {
        fprintf(stderr, "Malloc Failed while allocating Text Object!\n");
        return NULL;
    }

    textobj->text = duplicateString(text);
    textobj->color = color;
    textobj->font = font;
    textobj->texture = createTextTxture(renderer, text, color);

    if (textobj->texture) {
        SDL_QueryTexture(textobj->texture, NULL, NULL, &(textobj->width), &(textobj->height));
    }
    else {
        textobj->width = 0;
        textobj->height = 0;
    }

    if (!addText) {
        destroyText(textobj);
        return NULL;
    }

    return textobj;
}

