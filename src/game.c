#include "../include/game.h"
#include "../include/Scenes/LoadingScene.h"
#include "../include/Values/Config.h"

RenderContext* rc = NULL;
WidgetManager* wm = NULL;
SDL_Texture* piecetex = NULL;
SDL_Event event; 
SDL_Color gameBgColor = { 25, 30, 40, 220 };
SDL_Color optionsMenuBgColor = { 25, 30, 40, 220 };

SDL_Color white = { 255, 255, 255, 255 };
SDL_Color black = { 0, 0, 0, 255 };
SDL_Color lightGray = { 224, 224, 224, 255 };
SDL_Color gray = { 176, 176, 176, 255 };
SDL_Color whiteMatt = { 255, 255, 255, 200 };




//for game
int boardLabeFontSize= 14;
int menuTitleFontSize = 80;
int buttonFontSize = 60;
int timerTextFontSize = 50;

TTF_Font* menuTitleBoldFontMont = NULL;
TTF_Font* menuTitleFontSans = NULL;
TTF_Font* fontSans = NULL;
TTF_Font* buttonMediumFontMont = NULL;
TTF_Font* timerMediumFontMont = NULL;

Label* rankLabelTexture[8] = { NULL };
Label* fileLabelTexture[8] = { NULL };
static const char* rankLabelText[8] = { "8", "7", "6", "5", "4", "3", "2", "1" };
static const char* fileLabelText[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
CellState cell[64] = { {NOCOLOR, NONE_TYPE, {0,0,0,0}}, {0,0,0,0}, false, false, 0};

Timer_t* timerOwn = NULL;
Timer_t* timerEnemy = NULL;

SDL_Rect lineRect;

//for menu

SDL_Texture* bgs[5] = {NULL};
SDL_Texture* mainMenuBgs[4] = {NULL};
SDL_Texture* optionMenuBgs[3] = {NULL};

static Mixer* mixer = NULL;
Mix_Music* frozenStar = NULL;
Mix_Music* gymnopedieNo1 = NULL;

Button* newGameButton = NULL;
Button* optionsButton = NULL;
Button* quitButton = NULL;

Label* menuTitle = NULL;



// for options
Label* optionsMenuTitle = NULL;


void renderFrame(RenderContext* rc, CellState* cell) {
    setBackgroundColor(rc, &gameBgColor);
    SDL_Color tileColor = { 255, 255, 255, 255 };
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;

            // render board
            if (cell[index].isSelected) {
                tileColor = selectedColor;
            }
            else if (cell[index].istarget) {
                if ((row + col) % 2 == 0) {
                    tileColor = targetColor1;
                }
                else {
                    tileColor = targetColor2;
                }
            }
            else if ((row + col) % 2 == 0) {
                tileColor = tileColor1;
            }
            else {
                tileColor = tileColor2;
            }
            renderRect(rc, &(cell[index].dstRect), &tileColor, 1);
            
            //render pieces
            if (cell[index].piece.type != NONE_TYPE) {
                renderTextureEa(rc, piecetex, &(cell[index].piece.srcRect), &(cell[index].dstRect));
            }

            //render board labels
            if (col == 0) {
                UI_RenderLabel(rc, rankLabelTexture[row]);
            }

            if (row == 7) {
                UI_RenderLabel(rc, fileLabelTexture[col]);
            }
        }
    }
    

    renderRect(rc, &lineRect, &whiteMatt, true);
    if (timerOwn->isActive) UI_RenderLabel(rc, timerOwn->activeText);
    else UI_RenderLabel(rc, timerOwn->inactiveText);
    
    if (timerEnemy->isActive) UI_RenderLabel(rc, timerEnemy->activeText);
    else UI_RenderLabel(rc, timerEnemy->inactiveText);

    displayWindow(rc);
}



bool initializeGame() {
    ConfigInit();
    rc = createRenderer("ZiChess", winsize.width, winsize.height, winIconPath);
    wm = UI_CreateWidgetManager();
    if (!rc && !wm) return false;
    mixer = CoreInit();
    frozenStar = loadMusic(mixer, frozenStarMusicPath);
    gymnopedieNo1 = loadMusic(mixer, gymnopediNo1MusicPath);
  
    menuTitleBoldFontMont = UI_LoadFont(wm, montserratBoldFontPath, menuTitleFontSize);
    buttonMediumFontMont = UI_LoadFont(wm, montserratMediumFontPath, buttonFontSize);
    timerMediumFontMont = UI_LoadFont(wm, montserratMediumFontPath, timerTextFontSize);
    menuTitleFontSans = UI_LoadFont(wm, opensansFontPath, menuTitleFontSize);
    fontSans = UI_LoadFont(wm, opensansFontPath, boardLabeFontSize);
    

    SDL_Color* textColor = NULL;
    for (int i = 0; i < 8; ++i) {
        textColor = (i % 2) == 0 ? &tileColor2 : &tileColor1;
        rankLabelTexture[i] = UI_CreateLabel(rc, wm, rankLabelText[i], 0, 0, textColor, fontSans);
        textColor = (i % 2) == 0 ? &tileColor1 : &tileColor2;
        fileLabelTexture[i] = UI_CreateLabel(rc, wm, fileLabelText[i], 0, 0, textColor, fontSans);
    }

    int startX = (winsize.width - 8 * squareSize) / 2;
    int startY = (winsize.height - 8 * squareSize) / 2;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int index = i * 8 + j;
            int posX = startX + j * squareSize;
            int posY = startY + i * squareSize;
            cell[index].dstRect.x = posX;
            cell[index].dstRect.y = posY;
            cell[index].dstRect.w = squareSize;
            cell[index].dstRect.h = squareSize;
            cell[index].index = index;

            if (j == 0) {
                UI_UpdateLabel(NULL, rankLabelTexture[i], NULL, posX, posY);
            }

            if (i == 7) {              
                int fileLabelPosX = posX + squareSize - fileLabelTexture[j]->rect.w;
                int fileLabelPosY = posY + squareSize - fileLabelTexture[j]->rect.h;
                UI_UpdateLabel(NULL, fileLabelTexture[j], NULL, fileLabelPosX, fileLabelPosY);
            }
        }


    }


    lineRect = (SDL_Rect){ .x = 1008, .y = winsize.height/2, .w = 200, .h = 5}; //line center should be 1108 if width of the window is 1280 so line.x1 = 1108 - line.width/2

    const char* startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    loadPositionFromFen(startingFen, cell);

    //boardtex = createboardTexture(rc);
    piecetex = loadTexture(rc, pieceImagePath);
    timerOwn = CreateTimer(rc, 3, 0, 0, 0, &white, &gray, timerMediumFontMont);
    UI_UpdateLabel(NULL, timerOwn->activeText, NULL, (1108 - timerOwn->activeText->rect.w / 2), 380);
    UI_UpdateLabel(NULL, timerOwn->inactiveText, NULL, (1108 - timerOwn->activeText->rect.w / 2), 380);
    timerEnemy = CreateTimer(rc, 3, 0, 0, 0, &white, &gray, timerMediumFontMont);
    UI_UpdateLabel(NULL, timerEnemy->activeText, NULL, (1108 - timerEnemy->activeText->rect.w / 2), 280);
    UI_UpdateLabel(NULL, timerEnemy->inactiveText, NULL, (1108 - timerEnemy->activeText->rect.w / 2), 280);

    timerEnemy->isActive = false;


    return true;
}

Scene run_game() {

    bool run = true;
    bool isPieceSelected = false;
    int selectedPieceRow = 0;
    int selectedPieceCol = 0;

    MovesArray* moves = NULL;

    Clock clock;
    Clock_Init(&clock);
    while (run) {
        Clock_Update(&clock, 60);
        if (Clock_TimePassed(&clock, 1000)) {
            if (timerOwn->isActive) TimerDecreement(rc, timerOwn);
            if (timerEnemy->isActive) TimerDecreement(rc, timerEnemy);
        }

        // event handling
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                run = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int row, col;
                    if (getCellPressed(&row, &col)) {
                        if (isPieceSelected) {
                            int targetIndex = row * 8 + col;
                            int staringIndex = selectedPieceRow * 8 + selectedPieceCol;
                            if (moves != NULL && moves->moves != NULL) {
                                for (int i = 0; i < moves->moveCount; ++i) {
                                    if (moves->moves[i]->targetCell == targetIndex) {
                                        movePiece(cell, selectedPieceRow, selectedPieceCol, row, col);
                                    }
                                }
                                destroyMoves(cell, moves->moves, moves->moveCount);
                                free(moves);
                            }
                            else {
                                movePiece(cell, selectedPieceRow, selectedPieceCol, row, col);
                            }

                            cell[staringIndex].isSelected = false;
                            isPieceSelected = false;
                        }
                        else if (markSelected(cell, row, col)) {
                            isPieceSelected = true;
                            selectedPieceCol = col;
                            selectedPieceRow = row;

                            moves = generateMoves(cell, row, col);
                        }
                    }
                }
            default:
                break;
            };
        }

        //update


        //rendering
        renderFrame(rc, cell);

        printf("\rFPS - %0.2f", clock.fps);
        
    }

    return QUIT;
}

void initMenu() {
    bgs[0] = loadTexture(rc, knightBgPath1);
    bgs[1] = loadTexture(rc, knightBgPath2);
    bgs[2] = loadTexture(rc, knightBgPath3);
    bgs[3] = loadTexture(rc, queenBgPath1);
    bgs[4] = loadTexture(rc, kingBgPath1);

    mainMenuBgs[0] = bgs[0];
    optionMenuBgs[0] = bgs[0];     
    mainMenuBgs[1] = bgs[4];
    optionMenuBgs[1] = bgs[4];    
    mainMenuBgs[2] = bgs[1];
    optionMenuBgs[2] = bgs[2];
    mainMenuBgs[3] = bgs[3];

    int button_padY = 10;
    int button_padX = 100;

    newGameButton = UI_CreateButtonEx(rc, wm, 'N', "New Game", button_padX, 300, NONE, NONE, &lightGray, &gray, &white, NULL, buttonMediumFontMont);
    optionsButton = UI_CreateButtonEx(rc, wm, 'O', "Options", button_padX, (newGameButton->rect.h + newGameButton->rect.y + button_padY), NONE, NONE, &lightGray, &gray, &white, NULL, buttonMediumFontMont);
    quitButton = UI_CreateButtonEx(rc, wm, 'Q', "Quit", button_padX, (optionsButton->rect.h + optionsButton->rect.y + button_padY), NONE, NONE, &lightGray, &gray, &white, NULL, buttonMediumFontMont);
    menuTitle = UI_CreateLabel(rc, wm, "ZiChess", 0, 0, &white, menuTitleBoldFontMont);
    UI_UpdateLabel(NULL, menuTitle, NULL, (winsize.width / 2) - (menuTitle->rect.w / 2), 100);

}

Scene runMainMenu() {
    int currentMenuBgIndex = randint(0, 3);
    Scene nextScene = MAIN_MENU;
    bool buttonHandled = false;

    while (true) {

        buttonHandled = false;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return QUIT;
            handleButtonEvent(wm, &event);
            buttonHandled = true;
        }

        if (!buttonHandled) {
            handleButtonEvent(wm, &event);
        }

        //update
        nextScene = updateButtons(wm);

        if (nextScene != MAIN_MENU) {
            stopMusic(mixer);
            break;
        }



        updateMusic(mixer);

        setBgTexture(rc, mainMenuBgs[currentMenuBgIndex], 1280, 720);
        UI_RenderLabel(rc, menuTitle);
        UI_RenderButton(rc, newGameButton);
        UI_RenderButton(rc, optionsButton);
        UI_RenderButton(rc, quitButton);

       
        displayWindow(rc);
    }

    return nextScene;
}

void initOptionsMenu() {
    optionsMenuTitle = UI_CreateLabel(rc, wm, "Options", 0, 0, &white, menuTitleBoldFontMont);
    UI_UpdateLabel(NULL, optionsMenuTitle, NULL, (winsize.width / 2) - (optionsMenuTitle->rect.w / 2), 50);
}

Scene runOptionsmenu() {
    Scene nextScene = OPTIONS;
    //bool buttonHandled = false;
    while (true) {
        //buttonHandled = false;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                nextScene = QUIT;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    nextScene = MAIN_MENU;
                }
            }
            //handleButtonEvent(wm, &event);
            //buttonHandled = true;
        }

        //if (!buttonHandled) {
        //    handleButtonEvent(wm, &event);
        //}

        //update
        //nextScene = updateButtons(wm);

        if (nextScene != OPTIONS) {
            break;
        }

        setBackgroundColor(rc, &optionsMenuBgColor);
        UI_RenderLabel(rc, optionsMenuTitle);


        displayWindow(rc);
    }
    return nextScene;
}

Scene runStartUp() {
    return RunLoadingScene(rc, wm);
}


void clean()
{
    DestroyTimer(timerOwn);
    DestroyTimer(timerEnemy);
    destroyRenderer(rc);
    destroyCore();
    cleanSettings();
    UI_Clean(wm);
    ConfigClean();
}



