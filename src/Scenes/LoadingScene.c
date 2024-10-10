#include <SDL.h>
#include <stdbool.h>

#include "../../include/Scenes/LoadingScene.h"
#include "../../include/Values/Colors.h"
#include "../../include/Utils.h"
#include "../../include/Settings.h"
#include "../../include/Values/Config.h"




Scene RunLoadingScene(RenderContext* rc, WidgetManager* wm) {
    SDL_Texture* bg = loadTexture(rc, knightBgPath1);
    TTF_Font* pbarFont = UI_LoadFont(wm, montserratMediumFontPath, 12);
    ProgressBar* pbar = UI_CreateProgressBar(rc, wm, 100.0f, (winsize.centerX - 384), winsize.centerY + 300, 768, 20, &ColorDarkGrey, &ColorLightGray, &ColorWhite, pbarFont);

    //Clock clock;
    //Clock_Init(&clock);
    SDL_Event event;
    while (true) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return QUIT;
            }
        }

        UI_UpdateProgressBar(rc, pbar, 0.1f);

        if (pbar->currentProgress == pbar->maxProgress) break;

        setBgTexture(rc, bg, winsize.width, winsize.height);

        UI_RenderProgressBar(rc, pbar);

        displayWindow(rc);
        //Clock_Update(&clock, 60);
    }

    return MAIN_MENU;
}