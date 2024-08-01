#include "../include/game.h"


Renderer* renderer = NULL;
// SDL_Texture* boardtex = NULL;
SDL_Texture* piecetex = NULL;
SDL_Event event;
SDL_Color bgColor = { 55, 61, 99, 255 };
Mixer* mixer = NULL;
Mix_Chunk* moveSelfSound = NULL;
Mix_Chunk* captureSound = NULL;
Mix_Chunk* promoteSound = NULL;

CellState cell[64] = { {NONE, {0,0,0,}}, {0,0,0,0}, false, {0, 0, 0, 0} };

void renderFrame(Renderer* renderer, CellState* cell) {
    // clearWindow(renderer);
    setBackgroundColor(renderer, &bgColor);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int index = i * 8 + j; // dont change
            
            // render board
            renderRect(renderer, &(cell[index].dstRect), &(cell[index]).tileColor, 1);

            //render pieces
            if (cell[index].piece.state != NONE) {
                renderTextureEa(renderer, piecetex, &(cell[index].piece.srcRect), &(cell[index].dstRect));
            }
        }
    }
    displayWindow(renderer);
}



bool initialize_game() {
    renderer = createRenderer("Chess", winsize.width, winsize.height, NULL, NULL, 0);
    if (renderer == NULL) {
        return false;
    }

    mixer = createMixer();
    

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int index = i * 8 + j; // row*cols+col //do not change
            (cell[index]).dstRect.x = j * squareSize; // dont change
            (cell[index]).dstRect.y = i * squareSize; // dont change 
            (cell[index]).dstRect.w = squareSize;
            (cell[index]).dstRect.h = squareSize;
            (cell[index]).tileColor = ((i + j) % 2 == 0) ? tileColor1 : tileColor2;
        }
    }

    const char* startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    LoadPositionFromFen(startingFen, cell);

    //boardtex = createboardTexture(renderer);
    piecetex = loadTexture(renderer, pieceImagePath);
    moveSelfSound = loadSound(mixer, moveSelfSoundPath);
    captureSound = loadSound(mixer, captureSoundPath);
    promoteSound = loadSound(mixer, promoteSoundPath);

    return true;
}

Scene run_game() {

    bool run = true;
    bool isPieceSelected = false;
    int selectedPieceRow, selectedPieceCol;

    while (run) {
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
                            movePiece(cell, selectedPieceRow, selectedPieceCol, row, col, moveSelfSound, captureSound, promoteSound);
                            isPieceSelected = false;
                        }
                        else if (markSelected(cell, row, col)) {
                            isPieceSelected = true;
                            selectedPieceCol = col;
                            selectedPieceRow = row;
                        }
                    }
                    
                }
            default:
                break;
            };
        }

        //update


        //rendering
        renderFrame(renderer, cell);

        //renderTextureEx(renderer, boardtex, 0, 0, (squareSize * 8), (squareSize * 8));



        displayWindow(renderer);
    }

    return QUIT;
}




Scene runMainMenu() {
    bool run = true;
    while (run) {

        // event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) run = false;
        }

        //update


        //rendering

        // renderTextureEx(renderer, boardtex, 0, 0, (squareSize * 8), (squareSize * 8));


    }

    return QUIT;
}


void clean()
{
    destroyRenderer(renderer);
    destroyMixer(mixer);
}

