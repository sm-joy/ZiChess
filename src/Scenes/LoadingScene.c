#include <SDL.h>
#include <stdbool.h>

#include "../../include/Scenes/LoadingScene.h"
#include "../../include/Values/Colors.h"
#include "../../include/Utils.h"
#include "../../include/Settings.h"
#include "../../include/Values/Config.h"




Scene RunLoadingScene(RenderContext* rc, WidgetManager* wm, AssetManager* am) {
    SDL_Texture* bg = GetTexture(am, "loading_scene_bg");
    TTF_Font* pbarFont = UI_LoadFont(NULL, montserratMediumFontPath, 12);
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

        GFX_SetBg(rc, bg, winsize.width, winsize.height, &ColorBlack);

        UI_RenderProgressBar(rc, pbar);

        GFX_Display(rc);
        //Clock_Update(&clock, 60);
    }

    TTF_CloseFont(pbarFont);
    return MAIN_MENU;
}