#ifndef UI_H 
#define UI_H

#include <stdbool.h>
#include <SDL.h>
#include "Graphics.h"

#define NONE -1

typedef struct UI_LABEL {
    TTF_Font* font;
    SDL_Color color;
    SDL_Texture* texture;
    SDL_Rect rect;
} Label;

typedef enum {
    BUTTON_NORMAL,
    BUTTON_HOVER,
    BUTTON_PRESSED,
    BUTTON_RELEASED
} ButtonState;

typedef struct UI_BUTTON {
    char id;
    SDL_Rect rect;
    Label* currentLabel;
    Label* normalLabel;
    Label* hoverLabel;
    Label* pressLabel;
    SDL_Color color;
    ButtonState state;
} Button;

typedef struct UI_PROGRESSBAR {
    int maxProgress, currentProgress;
    SDL_Color bgColor, fgColor;
    SDL_Rect fgRect, bgRect;
    Label* progressLabel;
} ProgressBar;


typedef struct UI_WIDGET_MANAGER {
    TTF_Font** fonts;
    size_t numFonts;
    Label** labels;
    size_t numLabels;
    Button** buttons;
    size_t numButtons;    
    ProgressBar** progressBars;
    size_t numProgressBar;
} WidgetManager;

WidgetManager* UI_CreateWidgetManager();
void UI_Clean(WidgetManager* wm);

TTF_Font* UI_LoadFont(WidgetManager* wm, const char* fontPath, int fontSize);
void UI_DestroyFonts(WidgetManager* wm);

Label* UI_CreateLabel(RenderContext* rc, WidgetManager* wm, const char* text, int posX, int posY, SDL_Color* color, TTF_Font* font);
void UI_UpdateLabel(RenderContext* rc, Label* label, const char* text, int posX, int posY);
void UI_RenderLabel(RenderContext* rc, Label* label);
void UI_DestroyLabels(WidgetManager* wm);
void UI_DestroyLabel(Label* label);

Button* UI_CreateButtonEx(RenderContext* rc, WidgetManager* wm, char id, const char* text, int posX, int posY, int width, int height, SDL_Color* normalLabelColor, SDL_Color* hoverLabelColor, SDL_Color* pressLabelColor, SDL_Color* buttonColor, TTF_Font* font);
void UI_DestroyButtons(WidgetManager* wm);
void UI_RenderButton(RenderContext* rc, Button* button);

ProgressBar* UI_CreateProgressBar(RenderContext* rc, WidgetManager* wm, int maxProgress, int posX, int posY, int width, int height, SDL_Color* bgColor, SDL_Color* fgColor, SDL_Color* labelColor, TTF_Font* font);
void UI_UpdateProgressBar(RenderContext* rc, ProgressBar* pb, int increment);
void UI_DestroyProgressBars(WidgetManager* wm);
void UI_RenderProgressBar(RenderContext* rc, ProgressBar* pb);

#endif // UI_H
