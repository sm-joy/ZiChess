#ifndef UI_H 
#define UI_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Graphics.h"

#define UNDEFINED -1

typedef struct UI_Label {
    TTF_Font* font;
    SDL_Color color;
    SDL_Texture* texture;
    SDL_Rect rect;
} Label;

typedef enum {
    BUTTON_NORMAL,
    BUTTON_HOVER,
    BUTTON_PRESSED,
} ButtonState;

typedef struct UI_Button {
    int id;
    SDL_Rect rect;
    Label* normalLabel;
    Label* hoverLabel;
    Label* pressLabel;
    SDL_Color color;
    ButtonState state;
} Button;

typedef struct UI_Progressbar {
    float maxProgress, currentProgress;
    SDL_Color bgColor, fgColor;
    SDL_Rect fgRect, bgRect;
    Label* progressLabel;
} ProgressBar;

typedef struct UI_Timer {
    unsigned short int min, sec;
    Label* activeLabel;
    Label* inactiveLabel;
    bool isActive;
} Timer;

typedef struct UI_WidgetManager {
    TTF_Font** fonts;
    size_t numFonts;

    Label** labels;
    size_t numLabels;

    Button** buttons;
    size_t numButtons;  

    ProgressBar** progressBars;
    size_t numProgressBar;

    Timer** timers;
    size_t numTimer;
} WidgetManager;


bool UI_CreateWidgetManager(WidgetManager* wm);
void UI_DestroyWidgetManager(WidgetManager* wm);

TTF_Font* UI_LoadFont(WidgetManager* wm, const char* fontPath, int fontSize);
void UI_DestroyFonts(WidgetManager* wm);

Label* UI_CreateLabel(RenderContext* rc, WidgetManager* wm, const char* text, int posX, int posY, SDL_Color* color, TTF_Font* font);
void UI_UpdateLabel(RenderContext* rc, Label* label, const char* text, int posX, int posY);
void UI_RenderLabel(RenderContext* rc, Label* label);
void UI_DestroyLabels(WidgetManager* wm);
void UI_DestroyLabel(Label* label);
//void UI_CreateTextureFromLabel();

Button* UI_CreateButtonEx(RenderContext* rc, WidgetManager* wm, int id, const char* text, int posX, int posY, int width, int height, SDL_Color* normalLabelColor, SDL_Color* hoverLabelColor, SDL_Color* pressLabelColor, SDL_Color* buttonColor, TTF_Font* font);
void UI_DestroyButtons(WidgetManager* wm);
void UI_RenderButton(RenderContext* rc, Button* button);
int UI_HandleButtonEvent(WidgetManager* wm, SDL_Event* event);

ProgressBar* UI_CreateProgressBar(RenderContext* rc, WidgetManager* wm, float maxProgress, int posX, int posY, int width, int height, SDL_Color* bgColor, SDL_Color* fgColor, SDL_Color* labelColor, TTF_Font* font);
void UI_UpdateProgressBar(RenderContext* rc, ProgressBar* pb, float increment);
void UI_DestroyProgressBars(WidgetManager* wm);
void UI_RenderProgressBar(RenderContext* rc, ProgressBar* pb);

Timer* UI_CreateTimer(RenderContext* rc, WidgetManager* wm, int min, int sec, int posX, int posY, SDL_Color* activeColor, SDL_Color* inactiveColor, TTF_Font* font);
void UI_DestroyTimer(Timer* timer);
void UI_DestroyTimers(WidgetManager* wm);
void UI_TimerDecreement(RenderContext* rc, Timer* timer);

#endif // UI_H
