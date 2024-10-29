#include <SDL.h>
#include <stdbool.h>

#include "../../include/Scenes/MenuScene.h"
#include "../../include/Values/Colors.h"
#include "../../include/Utils.h"
#include "../../include/Settings.h"
#include "../../include/Values/Config.h"
#include "../../include/Ui.h"

#define SCENE_BUTTON_OFFSET 200

static Label* menuTitleLabel = NULL;
static Button* newGameButton = NULL;
static Button* optionsMenuButton = NULL;
static Button* quitButton = NULL;
static Scene nextScene = MAIN_MENU;


void MenuInit(RenderContext* rc, WidgetManager* wm, AssetManager* am) {
    int buttonPadY = 10;
    int buttonPadX = 100;
    TTF_Font* titleFont = UI_LoadFont(NULL, montserratBoldFontPath, 80);
    TTF_Font* buttonFont = UI_LoadFont(NULL, montserratMediumFontPath, 60);

    menuTitleLabel = UI_CreateLabel(rc, wm, "ZiChess", 0, 0, &ColorWhite, titleFont);
    UI_UpdateLabel(NULL, menuTitleLabel, NULL, winsize.centerX - (menuTitleLabel->rect.w / 2), 100);

    newGameButton = UI_CreateButtonEx(rc, wm, SCENE_BUTTON_OFFSET + 1, "New Game", buttonPadX, 300, UNDEFINED, UNDEFINED, &ColorLightGray, &ColorGray, &ColorWhite, NULL, buttonFont);
    optionsMenuButton = UI_CreateButtonEx(rc, wm, SCENE_BUTTON_OFFSET + 2, "Options", buttonPadX, (newGameButton->rect.h + newGameButton->rect.y + buttonPadY), UNDEFINED, UNDEFINED, &ColorLightGray, &ColorGray, &ColorWhite, NULL, buttonFont);
    quitButton = UI_CreateButtonEx(rc, wm, SCENE_BUTTON_OFFSET + 3, "Quit", buttonPadX, (optionsMenuButton->rect.h + optionsMenuButton->rect.y + buttonPadY), UNDEFINED, UNDEFINED, &ColorLightGray, &ColorGray, &ColorWhite, NULL, buttonFont);

    TTF_CloseFont(titleFont);
    TTF_CloseFont(buttonFont);
}

static void HandleSceneEvents(int pressedButtonId) {
    switch (pressedButtonId) {
        case 201: nextScene = MAIN_GAME; break;
        case 202: nextScene = OPTIONS; break;
        case 203: nextScene = QUIT; break;
        case UNDEFINED: default: break;
    }
}


Scene RunMenuScene(RenderContext* rc, WidgetManager* wm, AssetManager* am) {
    SDL_Texture* mainMenuBg = GetTexture(am, "menu_scene_bg");
    bool buttonHandled = false;
    SDL_Event event;
    int pressedButtonId = -1;



    while (true) {

        buttonHandled = false;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return QUIT;
            pressedButtonId = UI_HandleButtonEvent(wm, &event);
            buttonHandled = true;
        }

        if (!buttonHandled) {
            pressedButtonId = UI_HandleButtonEvent(wm, &event);
            buttonHandled = true;
        }

        HandleSceneEvents(pressedButtonId);

        if (nextScene != MAIN_MENU) {
            Audio_StopMusic();
            break;
        }



        Audio_UpdateMusic(am);

        GFX_SetBg(rc, mainMenuBg, winsize.width, winsize.height, &ColorBlack);

        UI_RenderLabel(rc, menuTitleLabel);
        UI_RenderButton(rc, newGameButton);
        UI_RenderButton(rc, optionsMenuButton);
        UI_RenderButton(rc, quitButton);


        GFX_Display(rc);
    }

    return nextScene;
}

