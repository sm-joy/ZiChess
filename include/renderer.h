#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>



typedef struct {
    char* text;
    TTF_Font* font;
    SDL_Color* color;
    SDL_Texture* texture;
    int height, width;
} Text;

typedef struct {
    SDL_Rect rect;        
    Text* text;
    Text* hoverText;
    Text* pressText;
    SDL_Color color;       
    bool isHovered;
    bool isPressed;        
} Button;

typedef struct HORIZONTAL_SCROLLBAR {
    unsigned int currentValue;
    unsigned int maxValue;
    SDL_Rect barRect;
    SDL_Rect handleRect;
    bool isDragging;
    int dragOffsetX;
    SDL_Color barColor;
    SDL_Color handleColor;
} hScrollbar;

typedef struct SDL_RENDERER {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* winIcon;
    SDL_Texture** loadedTextures;
    size_t numTextures;
    Text** createdTexts;
    size_t numTexts;
    Button** createdButtons;
    size_t numButtons;
} Renderer;


Renderer* createRenderer(const char* pWinTitle, int pWinWidth, int pWinHeight, const char* pWinIconPath);
void destroyRenderer(Renderer* renderer);
void clearWindow(Renderer* renderer);
void setRenderDrawColor(Renderer* renderer, SDL_Color* pColor);
void setBackgroundColor(Renderer* renderer, SDL_Color* pColor);
void setBackgroundTexture(Renderer* renderer, SDL_Texture* bgTexture);
void displayWindow(Renderer* renderer);
TTF_Font* loadFont(Renderer* renderer, const char* pFontPath, int pFontSize);
void setWindowIcon(Renderer* renderer, const char* pWinIconPath);
void renderRect(Renderer* renderer, SDL_Rect* pRect, SDL_Color* pColor, bool pFilled);
void renderText(Renderer* renderer, Text* text, int posX, int posY);
void renderTextCenterRect(Renderer* renderer, const char* pText, SDL_Color* pColor, SDL_Rect* pRect, TTF_Font* font);
void renderTextureEx(Renderer* renderer, SDL_Texture* pTexture, int pPosX, int pPosY, int pTextureWidth, int pTextureHeight);
void renderTextureEa(Renderer* renderer, SDL_Texture* pTexture, SDL_Rect* srcRect, SDL_Rect* dstRect);
void destroyTextures(Renderer* renderer);
void renderTexture(Renderer* renderer, SDL_Texture* texture, int posX, int poxY);
void destroyTexts(Renderer* renderer);
void destroyText(Text* textobj);


bool addText(Renderer* renderer, Text* textobj);
SDL_Texture* createTextTxture(Renderer* renderer, const char* text, SDL_Color* color, TTF_Font* font);
Text* createText(Renderer* renderr, const char* text, SDL_Color* color, TTF_Font* font);


Button* createButton(Renderer* renderer, const char* text, SDL_Color* buttonColor, SDL_Color* textColor, TTF_Font* font, int width, int height, int x, int y);
Button* createButtonTx(Renderer* renderer, SDL_Texture* texture);
Button* createButtonEx(Renderer* renderer, int x, int y, int width, int height, Text* normalText, Text* hoverText, Text* pressText, SDL_Color* buttonColor);

void destroyButton(Button* button);
void destroyButtons(Renderer* renderer);

hScrollbar* createVScrollbar(Renderer* renderer, int x, int y, int width, int height, int maxValue, SDL_Color handleColor, SDL_Color barColor);
void drawHScrollbar(Renderer* renderer, hScrollbar* scrollbar);

#endif //RENDERER_H