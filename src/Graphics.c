#include "../include/Graphics.h"

RenderContext* createRenderer(const char* pWinTitle, int pWinWidth, int pWinHeight, const char* pWinIconPath) {
    RenderContext* rc = (RenderContext*)malloc(sizeof(RenderContext));
    if (!rc) {
        fprintf(stderr, "Error allocating memory for RenderContext\n");
        return NULL;
    }

    rc->winIcon = NULL;
    rc->loadedTextures = NULL;
    rc->numTextures = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        free(rc);
        rc = NULL;
        return NULL;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    int imageFormats = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
    if (!(IMG_Init(imageFormats) & imageFormats)) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        free(rc);
        return NULL;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf initialization failed: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        free(rc);
        rc = NULL;
        return NULL;
    }

    rc->window = SDL_CreateWindow(pWinTitle, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        pWinWidth, 
        pWinHeight, 
        SDL_WINDOW_SHOWN);
    if (!rc->window) {
        fprintf(stderr, "Error initializing SDL_Window: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        free(rc);
        rc = NULL;
        return NULL;
    }

    SDL_SetWindowResizable(rc->window, SDL_FALSE);

    rc->renderer = SDL_CreateRenderer(rc->window, -1, SDL_RENDERER_ACCELERATED);
    if (!rc->renderer) {
        fprintf(stderr, "Error initializing SDL_Renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(rc->window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        free(rc);
        rc = NULL;
        return NULL;
    }
    if (SDL_SetRenderDrawBlendMode(rc->renderer, SDL_BLENDMODE_BLEND)) {
        fprintf(stderr, "Error enabling alpha belnding: %s\n", SDL_GetError());
    }


    if (pWinIconPath != NULL) setWindowIcon(rc, pWinIconPath);

    return rc;
}

void destroyRenderer(RenderContext* rc) {
    if (rc) {
        destroyTextures(rc);
        if (rc->winIcon) SDL_FreeSurface(rc->winIcon);
        if (rc->window) SDL_DestroyWindow(rc->window);
        if (rc->renderer) SDL_DestroyRenderer(rc->renderer);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        free(rc);
        rc = NULL;
    }
}

void destroyTextures(RenderContext* rc) {
    if (rc->loadedTextures) {
        for (int i = 0; i < rc->numTextures; ++i) {
            SDL_DestroyTexture(rc->loadedTextures[i]);
        }
        free(rc->loadedTextures);
        rc->loadedTextures = NULL;
        rc->numTextures = 0;
    }
}



void clearWindow(RenderContext* rc) {
    SDL_RenderClear(rc->renderer);
}

void setRenderDrawColor(RenderContext* rc, SDL_Color* pColor) {
    SDL_SetRenderDrawColor(rc->renderer, pColor->r, pColor->g, pColor->b, pColor->a);
}

void setBackgroundColor(RenderContext* rc, SDL_Color* pColor) {
    SDL_SetRenderDrawColor(rc->renderer, pColor->r, pColor->g, pColor->b, pColor->a);
    SDL_RenderClear(rc->renderer);
}

void displayWindow(RenderContext* rc) {
    SDL_RenderPresent(rc->renderer);
}

void setWindowIcon(RenderContext* rc, const char* winIconPath) {
    if (!rc || !winIconPath) return;
    if (rc->winIcon) SDL_FreeSurface(rc->winIcon);

    rc->winIcon = IMG_Load(winIconPath);
    if (rc->winIcon) {
        SDL_SetWindowIcon(rc->window, rc->winIcon);
    }
    else {
        fprintf(stderr, "Error Loading '%s' Icon! Error: %s\n", winIconPath, SDL_GetError());
    }
}

void renderRect(RenderContext* rc, SDL_Rect* rect, SDL_Color* color, bool isFill) {
    SDL_SetRenderDrawColor(rc->renderer, color->r, color->g, color->b, color->a);
    if (isFill) SDL_RenderFillRect(rc->renderer, rect);
    else SDL_RenderDrawRect(rc->renderer, rect);
}

void renderTextureEx(RenderContext* rc, SDL_Texture* texture, int posX, int posY, int width, int height) {
    SDL_Rect destRect = { posX, posY, width, height };
    if (SDL_RenderCopy(rc->renderer, texture, NULL, &destRect) != 0) {
        fprintf(stderr, "Error rendering texture! Error: %s", SDL_GetError());
    }
}

void renderTexture(RenderContext* rc, SDL_Texture* texture, int posX, int posY) {
    int height = 0, width = 0;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    renderTextureEx(rc, texture, posX, posY, width, height);
}

void renderTextureEa(RenderContext* rc, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect) {
    SDL_RenderCopy(rc->renderer, texture, srcRect, dstRect);
}


void setBgTexture(RenderContext* rc, SDL_Texture* texture, int width, int height) {
    SDL_RenderClear(rc->renderer);
    renderTextureEx(rc, texture, 0, 0, width, height);
}


