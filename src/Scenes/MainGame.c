#include "../../include/Scenes/MainGame.h"
#include "../../include/Values/Colors.h"
#include "../../include/Values/Config.h"
#include "../../include/Audio.h"

static Timer* timerOwn = NULL;
static Timer* timerEnemy = NULL;
static SDL_Rect lineRect;

static Mix_Chunk* moveSound = NULL;
static Mix_Chunk* captureSound = NULL;
static Mix_Chunk* promoteSound = NULL;
static Mix_Chunk* checkSound = NULL;
static Mix_Chunk* castleSound = NULL;
static Mix_Chunk* notifySound = NULL;


void MainGameInit(RenderContext* rc, WidgetManager* wm, Engine* engine, AssetManager* am) {
    LoadFenBoard(&engine->board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");


    TTF_Font* timerFont = UI_LoadFont(NULL, montserratMediumFontPath, 50);
    timerOwn = UI_CreateTimer(rc, wm, 3, 0, 0, 0, &ColorWhite, &ColorGray, timerFont);
    UI_UpdateLabel(NULL, timerOwn->activeLabel, NULL, (1108 - timerOwn->activeLabel->rect.w / 2), 380);
    UI_UpdateLabel(NULL, timerOwn->inactiveLabel, NULL, (1108 - timerOwn->activeLabel->rect.w / 2), 380);
    timerEnemy = UI_CreateTimer(rc, wm, 3, 0, 0, 0, &ColorWhite, &ColorGray, timerFont);
    UI_UpdateLabel(NULL, timerEnemy->activeLabel, NULL, (1108 - timerEnemy->activeLabel->rect.w / 2), 280);
    UI_UpdateLabel(NULL, timerEnemy->inactiveLabel, NULL, (1108 - timerEnemy->activeLabel->rect.w / 2), 280);
    TTF_CloseFont(timerFont);

    moveSound = GetSound(am, "move_self");
    captureSound = GetSound(am, "capture");
    promoteSound = GetSound(am, "promote");
    checkSound = GetSound(am, "move_check");
    castleSound = GetSound(am, "castle");
    notifySound = GetSound(am, "notify");


    lineRect = (SDL_Rect){ .x = 1008, .y = winsize.centerY, .w = 200, .h = 5 };
}


static void PlayMoveSound(MoveSignal sig) {
    switch (sig) {
        case MOVE_NORMAL:  Audio_PlaySound(moveSound);    break;
        case MOVE_CAPTURE: Audio_PlaySound(captureSound); break;
        case MOVE_CASTLE:  Audio_PlaySound(castleSound);  break;
        case MOVE_CHECK:   Audio_PlaySound(checkSound);   break;
        case MOVE_PROMOTE: Audio_PlaySound(promoteSound); break;
        case MOVE_NONE: default: break;
    }
}


static void RenderFrame(RenderContext* rc, Engine* engine) {
    GFX_SetBg(rc, NULL, UNDEFINED, UNDEFINED, &ColorMainGameBG);

    RenderBoard(&engine->board, rc);

    GFX_RenderRect(rc, &lineRect, &ColorWhite, true);
    if (timerOwn->isActive) UI_RenderLabel(rc, timerOwn->activeLabel);
    else UI_RenderLabel(rc, timerOwn->inactiveLabel);

    if (timerEnemy->isActive) UI_RenderLabel(rc, timerEnemy->activeLabel);
    else UI_RenderLabel(rc, timerEnemy->inactiveLabel);

    GFX_Display(rc);
}


Scene MainGameRun(RenderContext* rc, WidgetManager* wm, Engine* engine) {
	engine->board.selectedCell = -1;
    int currentCell = -1;
    SDL_Event event;
    MoveSignal sig = MOVE_NONE;

	while (true) {
        sig = MOVE_NONE;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: return QUIT;
                case SDL_MOUSEBUTTONDOWN: {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        currentCell = -1;
                        if (CheckCellPress(&currentCell)) {
                            if (engine->board.selectedCell != -1) {
                                for (int i = 0; i < engine->moveList.moveCount; ++i) {
                                    if (engine->moveList.moves[i].targetCell == currentCell) {
                                        sig = MovePiece(&engine->board, engine->board.selectedCell, currentCell);
                                    }
                                }
                                ClearMoveHighlights(&engine->board, &engine->moveList);
                            }
                            else if (MarkCellSelected(&engine->board, currentCell)) {
                                GeneratePieceMoves(&engine->board, &engine->moveList, currentCell);
                            }
                        }
                    }
                    break;
                }
                default: break;
            };
        }

        PlayMoveSound(sig);
        RenderFrame(rc, engine);
	}
    
}







