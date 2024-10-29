#include "../include/Graphics.h"
#include "../include/Utils.h"
#include <SDL_image.h>
#include <stdio.h>

void GFX_CreateRenderContext(RenderContext* rc, const char* winTitle, int winWidth, int winHieght) {
    if (!rc) {
        LogNullError("Render Context", NULL);
        return;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    rc->window = SDL_CreateWindow(winTitle,
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        winWidth,
        winHieght,
        SDL_WINDOW_SHOWN);

    if (!rc->window) {
        fprintf(stderr, "Error initializing SDL_Window: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }
    SDL_SetWindowResizable(rc->window, SDL_FALSE);

    rc->renderer = SDL_CreateRenderer(rc->window, -1, SDL_RENDERER_ACCELERATED);
    if (!rc->renderer) {
        fprintf(stderr, "Error initializing SDL_Renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(rc->window);
        SDL_Quit();
        return;
    }
    if (SDL_SetRenderDrawBlendMode(rc->renderer, SDL_BLENDMODE_BLEND)) {
        fprintf(stderr, "Error enabling alpha belnding: %s\n", SDL_GetError());
    }
}

void GFX_DestroyRenderContext(RenderContext* rc) {
    if (rc) {
        if (rc->window)   SDL_DestroyWindow(rc->window);
        if (rc->renderer) SDL_DestroyRenderer(rc->renderer);
    }
    SDL_Quit();
}


void GFX_SetBg(RenderContext* rc, SDL_Texture* tex, int winWidth, int winHeight, SDL_Color* color) {
    if (!rc) {
        LogNullError("Render Context", NULL);
        return;
    }
    if (!color) {
        LogNullError("Color", NULL);
        return;
    }

    SDL_SetRenderDrawColor(rc->renderer, color->r, color->g, color->b, color->a);
    SDL_RenderClear(rc->renderer);
    if (tex && winWidth != UNDEFINED && winHeight != UNDEFINED) GFX_RenderTextureEx(rc, tex, 0, 0, winWidth, winHeight);
}

void setWindowIcon(RenderContext* rc, const char* winIconPath) {
    if (!rc || !winIconPath) return;

    SDL_Surface* icon = IMG_Load(winIconPath);
    if (icon) {
        SDL_SetWindowIcon(rc->window, icon);
        SDL_FreeSurface(icon);
    }
    else fprintf(stderr, "Error Loading '%s' Icon! Error: %s\n", winIconPath, SDL_GetError());
}

void GFX_RenderRect(RenderContext* rc, SDL_Rect* rect, SDL_Color* color, bool isFill) {
    SDL_SetRenderDrawColor(rc->renderer, color->r, color->g, color->b, color->a);
    if (isFill) SDL_RenderFillRect(rc->renderer, rect);
    else SDL_RenderDrawRect(rc->renderer, rect);
}

void GFX_RenderTextureEx(RenderContext* rc, SDL_Texture* texture, int posX, int posY, int width, int height) {
    SDL_Rect destRect = { posX, posY, width, height };
    SDL_RenderCopy(rc->renderer, texture, NULL, &destRect);
}

void renderTexture(RenderContext* rc, SDL_Texture* texture, int posX, int posY) {
    int height = 0, width = 0;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    GFX_RenderTextureEx(rc, texture, posX, posY, width, height);
}

void renderTextureEa(RenderContext* rc, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect) {
    SDL_RenderCopy(rc->renderer, texture, srcRect, dstRect);
}


