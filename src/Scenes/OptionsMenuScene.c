#include "../../include/Scenes/OptionsMenuScene.h"
#include "../../include/Values/Colors.h"
#include "../../include/Ui.h"
#include "../../include/Settings.h"
#include "../../include/Values/Config.h"

static Label* optionsMenuTitleLabel = NULL;


void OptionsMenuInit(RenderContext* rc, WidgetManager* wm) {
	TTF_Font* titleFont = UI_LoadFont(NULL, montserratBoldFontPath, 80);
	optionsMenuTitleLabel = UI_CreateLabel(rc, wm, "Options", 0, 0, &ColorWhite, titleFont);
	UI_UpdateLabel(NULL, optionsMenuTitleLabel, NULL, winsize.centerX - (optionsMenuTitleLabel->rect.w / 2), 50);
    TTF_CloseFont(titleFont);
}

Scene RunOptionsMenu(RenderContext* rc, WidgetManager* wm) {
    Scene nextScene = OPTIONS;
    SDL_Event event;
    //bool buttonHandled = false;
    while (true) {
        //buttonHandled = false;
        while (SDL_PollEvent(&event)) { // || !buttonHandled) {
            if (event.type == SDL_QUIT) {
                return QUIT;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return MAIN_MENU;
                }
            }

            //buttonHandled = true;
        }

        if (nextScene != OPTIONS) {
            break;
        }

        GFX_SetBg(rc, NULL, UNDEFINED, UNDEFINED, &ColorDarkGrey);
        UI_RenderLabel(rc, optionsMenuTitleLabel);
        GFX_Display(rc);
    }
    return nextScene;
}


