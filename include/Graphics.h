#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <stdbool.h>

typedef struct  SDL_GRAPHICS_WRAPPER {
    SDL_Window* window;
    SDL_Renderer* renderer;
} RenderContext;

#define GFX_Display(rc) SDL_RenderPresent((rc)->renderer)

void GFX_CreateRenderContext(RenderContext*  rc, const char* winTitle, int winWidth, int winHeight);
void GFX_DestroyRenderContext(RenderContext* rc);
void GFX_SetBg(RenderContext* rc, SDL_Texture* tex, int winWidth, int winHeight, SDL_Color* color);
void setWindowIcon(RenderContext* rc, const char* pWinIconPath);
void GFX_RenderRect(RenderContext* rc, SDL_Rect* rect, SDL_Color* color, bool isFill);
void GFX_RenderTextureEx(RenderContext* rc, SDL_Texture* pTexture, int pPosX, int pPosY, int pTextureWidth, int pTextureHeight);
void renderTextureEa(RenderContext* rc, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect);
void renderTexture(RenderContext* rc, SDL_Texture* texture, int posX, int poxY);


#endif //RENDERER_H