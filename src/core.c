#include "../include/core.h"

#include "../include/Values/Config.h"

Piece kw = { WHITE, KING, {0, 0, 333, 334} };
Piece kb = { BLACK, KING, {0, 334, 333, 334} };
Piece qw = { WHITE, QUEEN, {333, 0, 333, 334} };
Piece qb = { BLACK, QUEEN, {333, 334, 333, 334} };
Piece pw = { WHITE, PAWN, {1665, 0, 333, 334} };
Piece pb = { BLACK, PAWN, {1665, 334, 333, 334} };
Piece bw = { WHITE, BISHOP, {666, 0, 333, 334} };
Piece bb = { BLACK, BISHOP, {666, 334, 333, 334} };
Piece nw = { WHITE, KNIGHT, {999, 0, 333, 334} };
Piece nb = { BLACK, KNIGHT, {999, 334, 333, 334} };
Piece rw = { WHITE, ROOK, {1332, 0, 333, 334} };
Piece rb = { BLACK, ROOK, {1332, 334, 333, 334} };
Piece none = { NOCOLOR, NONE_TYPE, {0, 0, 0, 0} };

Mixer* mixer = NULL;
Mix_Chunk* moveSelfSound = NULL;
Mix_Chunk* captureSound = NULL;
Mix_Chunk* promoteSound = NULL;


void DestroyTimer(Timer_t* timer) {
    if (!timer) return;
    if (timer->activeText) UI_DestroyLabel(timer->activeText);
    if (timer->inactiveText) UI_DestroyLabel(timer->inactiveText);
    free(timer);
    timer = NULL;
}

Timer_t* CreateTimer(RenderContext* rc, int min, int sec, int posX, int posY, SDL_Color* activeTextColor, SDL_Color* inactiveTextColor, TTF_Font* font) {
    Timer_t* timer = (Timer_t*)malloc(sizeof(Timer_t));
    if (!timer) {
        fprintf(stderr, "Malloc Failed While Creating Timer_t structure!\n");
        return NULL;
    }

    timer->min = min;
    timer->sec = sec;
    timer->isActive = true;
    char str[6];
    sprintf_s(str, 6, "%02d:%02d", min, sec);
    timer->activeText = UI_CreateLabel(rc, NULL, str, posX, posY, activeTextColor, font);
    if (!timer->activeText) {
        DestroyTimer(timer);
        return NULL;
    }    
    
    timer->inactiveText = UI_CreateLabel(rc, NULL, str, posX, posY, inactiveTextColor, font);
    if (!timer->inactiveText) {
        DestroyTimer(timer);
        return NULL;
    }
    return timer;
}

void TimerDecreement(RenderContext* rc, Timer_t* timer) {
    if (timer->sec > 0) {
        timer->sec--;
    }
    else {
        timer->sec = 59;
        if (timer->min > 0) {
            timer->min--;
        }
    }
    char str[6];
    sprintf_s(str, 6, "%02d:%02d", timer->min, timer->sec);
    UI_UpdateLabel(rc, timer->activeText, str, NONE, NONE);
    UI_UpdateLabel(rc, timer->inactiveText, str, NONE, NONE);
}

Mixer* CoreInit() {
    mixer = createMixer();
    moveSelfSound = loadSound(mixer, moveSelfSoundPath);
    captureSound = loadSound(mixer, captureSoundPath);
    promoteSound = loadSound(mixer, promoteSoundPath);


    return mixer;
}

void destroyCore() {
    destroyMixer(mixer);
}


void loadPositionFromFen(const char* fen, CellState *board) {
    int file = 0, rank = 7;

    for (int i = 0; i < strlen(fen); ++i) {
        char symbol = fen[i];
        if (symbol == ' ') {
            break;
        }
        else if ( symbol == '/') {
            file = 0;
            rank--;
        }
        else {
            if (isdigit(symbol)) {
                file += (int)(symbol - '0'); // converting to int if it is a number
            }
            else {
                int index = rank * 8 + file;
                switch (symbol) {
                    case 'r':
                        board[index].piece = rb;
                        break;
                    case 'n':
                        board[index].piece = nb;
                        break;
                    case 'b':
                        board[index].piece = bb;
                        break;
                    case 'q':
                        board[index].piece = qb;
                        break;
                    case 'k':
                        board[index].piece = kb;
                        break;
                    case 'p':
                        board[index].piece = pb;
                        break;
                    case 'R':
                        board[index].piece = rw;
                        break;
                    case 'N':
                        board[index].piece = nw;
                        break;
                    case 'B':
                        board[index].piece = bw;
                        break;
                    case 'Q':
                        board[index].piece = qw;
                        break;
                    case 'K':
                        board[index].piece = kw;
                        break;
                    case 'P':
                        board[index].piece = pw;
                        break;
                    default:
                        board[index].piece = none;
                        break;
                }

                file++;

            }
        }
    }
}

//bool getCellPressed(int* row, int* col) {
//    int mouseX, mouseY;
//    SDL_GetMouseState(&mouseX, &mouseY);
//    (*row) = (int)(mouseY / squareSize);
//    (*col) = (int)(mouseX / squareSize);
//    if (0 <= (*row) && (*row) < 8 && 0 <= (*col) && (*col) < 8) {
//        return true;
//    }
//    else {
//        return false;
//    }
//}

bool getCellPressed(int* row, int* col) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    int boardStartX = (winsize.width - (8 * squareSize)) / 2;
    int boardStartY = (winsize.height - (8 * squareSize)) / 2;

    int relX = mouseX - boardStartX;
    int relY = mouseY - boardStartY;
    if (relX >= 0 && relX < 8 * squareSize && relY >= 0 && relY < 8 * squareSize) {
        *col = relX / squareSize;
        *row = relY / squareSize;
        return true;
    }
    return false;
}


bool markSelected(CellState* cell, int row, int col) {
    int index = 8 * row + col;
    if (cell[index].piece.type != NONE_TYPE) {
        cell[index].isSelected = true;
        return true;
    }
    else {
        return false;
    }
}


static void promotePawn(CellState* cell, int targetRow, int tragetCol, PieceColor currentPieceColor) {

    playSound(promoteSound);
}

void movePiece(CellState* cell, int fromRow, int fromCol, int toRow, int toCol) {
    int fromIndex = fromRow * 8 + fromCol;
    int toIndex = toRow * 8 + toCol;


    if (fromIndex == toIndex || cell[fromIndex].piece.color == cell[toIndex].piece.color) {
        return;
    }
    else if (cell[toIndex].piece.type == NONE_TYPE) {
        playSound(moveSelfSound);
    }
    else if (fromIndex == toIndex || cell[fromIndex].piece.color != cell[toIndex].piece.color) {
        playSound(captureSound);
    }

    if (cell[fromIndex].piece.type == PAWN && cell[fromIndex].piece.color == WHITE && toRow == 7) {
        cell[toIndex].piece = qw; // Promote to white queen
        playSound(promoteSound);
    }
    else if (cell[fromIndex].piece.type == PAWN && cell[fromIndex].piece.color == BLACK && toRow == 0) {
        cell[toIndex].piece = qb; // Promote to black queen
        playSound(promoteSound);
    }
    else {
        cell[toIndex].piece = cell[fromIndex].piece;
    }
    cell[fromIndex].piece.type = NONE_TYPE;
    cell[fromIndex].piece.color = NOCOLOR;
    cell[fromIndex].piece.srcRect = (SDL_Rect){ 0, 0, 0, 0 };
    //printf("Moving piece from [%d, %d] (index %d) to [%d, %d] (index %d)\n", fromRow, fromCol, fromIndex, toRow, toCol, toIndex);
}

static Move* createMove(int startingIndex, int targetIndex) {
    Move* newMove = (Move*)malloc(sizeof(Move));
    if (newMove == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    newMove->startingCell = startingIndex;
    newMove->targetCell = targetIndex;
    return newMove;
}

void destroyMoves(CellState* cell, Move** moves, int moveCount) {
    if (moves != NULL) {
        for (int i = 0; i < moveCount; ++i) {
            if (moves[i] != NULL) {
                cell[moves[i]->targetCell].istarget = false;
                free(moves[i]);
                moves[i] = NULL;
            }
        }

        free(moves);
        moves = NULL;
    }
}

static bool isHover(Button* button)
{
    SDL_Point mousePos = {0, 0};
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    return SDL_PointInRect(&mousePos, &button->rect);
}

static bool isPressed(Button* button, SDL_Event* event) {
    return event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT && isHover(button);
}

static bool isReleased(Button* button, SDL_Event* event) {
    return event->type == SDL_MOUSEBUTTONUP && button->state == BUTTON_PRESSED;
}

Scene updateButtons(WidgetManager* wm) {
    Scene nextScene = MAIN_MENU;
    for (int i = 0; i < wm->numButtons; ++i) {
        Button* button = wm->buttons[i];
        switch (button->state) {
        case BUTTON_RELEASED: {
            switch (button->id) {
            case 'N': {
                nextScene =  MAIN_GAME;
                break;
            }
            case 'O': {
                nextScene =  OPTIONS;
                break;
            }
            case 'Q': {
                nextScene =  QUIT;
                break;
            }
            default: {
                nextScene = MAIN_MENU;
                break;
            }
            }
            break;
        }
        case BUTTON_HOVER: {
            button->currentLabel = button->hoverLabel;
            break;
        }
        case BUTTON_PRESSED: {
            button->currentLabel = button->pressLabel;
            break;
        }
        case BUTTON_NORMAL: {
            button->currentLabel = button->normalLabel;
        }
        default: {
            button->currentLabel = button->normalLabel;
            break;
        }
        }
    }

    return nextScene;
}


void handleButtonEvent(WidgetManager* wm, SDL_Event* event) {
    if (wm->buttons != NULL) {
        for (int i = 0; i < wm->numButtons; ++i) {
            Button* button = wm->buttons[i];

            if (button->state != BUTTON_PRESSED) {
                if (isHover(button)) {
                    button->state = BUTTON_HOVER;
                }
                else {
                    button->state = BUTTON_NORMAL;
                }
            }

            if (isPressed(button, event)) {
                button->state = BUTTON_PRESSED;
            }
            else if (isReleased(button, event)) {
                button->state = BUTTON_RELEASED;
            }
        }
    }
}


static bool addMove(CellState* cell, Move** moves, int* moveCount, int startingIndex, int targetIndex) {
    if (targetIndex < 0 || targetIndex >= 64) return false;
    Move* newMove = createMove(startingIndex, targetIndex);

    if (newMove == NULL) {
        destroyMoves(cell, moves, *moveCount);
        return false;
    }

    cell[targetIndex].istarget = true;
    moves[(*moveCount)++] = newMove;
    return true;
}

static MovesArray* generatePawnMoves(CellState* cell, int row, int col) {
    int startingIndex = row * 8 + col;
    int forwardDir = 0;
    int topLeftDir = 0;
    int topRightDir = 0;
    int maxMoves = 0;
    int leftDir = 0;
    int rightDir = 0;

    if (cell[startingIndex].piece.color == WHITE) {
        forwardDir = 8;
        topLeftDir = 7;
        topRightDir = 9;
        leftDir = 1;
        rightDir = -1;
        if (row == 1) {
            maxMoves = 4;
        }
        else {
            maxMoves = 3;
        }
    }
    else {
        forwardDir = -8;
        topLeftDir = -9;
        topRightDir = -7;
        leftDir = -1;
        rightDir = 1;
        if (row == 6) {
            maxMoves = 4;
        }
        else {
            maxMoves = 3;
        }
    }

    Move** moves = (Move**)malloc(maxMoves * sizeof(Move*));
    if (moves == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int moveCount = 0;

    int targetIndex = startingIndex + forwardDir;
    if (cell[targetIndex].piece.type == NONE_TYPE) {

        addMove(cell, moves, &moveCount, startingIndex, targetIndex);

        targetIndex += forwardDir;
        if (maxMoves == 4) {
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
        }
    }    
    if (col > 0) {
        targetIndex = startingIndex + topLeftDir;
        if (targetIndex >= 0 && targetIndex < 64 && cell[targetIndex].piece.type != NONE_TYPE) {
            if (cell[targetIndex].piece.color != cell[startingIndex].piece.color) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
        }

        // el passant (ill work on it later)
        targetIndex = startingIndex + leftDir;
        if (targetIndex >= 0 && targetIndex < 64 && cell[targetIndex].piece.type != NONE_TYPE) {
            if (cell[targetIndex].piece.color != cell[startingIndex].piece.color) {

            }
        }
    }

    if (col < 7) {
        targetIndex = startingIndex + topRightDir;
        if (cell[targetIndex].piece.color != cell[startingIndex].piece.color && cell[targetIndex].piece.type != NONE_TYPE) {
            addMove(cell, moves, &moveCount, startingIndex, targetIndex);
        }
    }

    MovesArray* moveobj = (MovesArray*)malloc(sizeof(MovesArray));
    if (moveobj == NULL) {
        printf("Memory allocation failed\n");
        destroyMoves(cell, moves, moveCount);
        return NULL;
    }

    moveobj->moves = moves;
    moveobj->moveCount = moveCount;

    return moveobj;
}

static MovesArray* generateKingMoves(CellState* cell, int row, int col) {
    int startingIndex = row * 8 + col;
    PieceColor color = cell[startingIndex].piece.color;
    int maxMoves = 8;

    static const int rowOffsets[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    static const int colOffsets[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    
    Move** moves = (Move**)malloc(maxMoves * sizeof(Move*));
    if (moves == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int moveCount = 0;

    for (int i = 0; i < 8; ++i) {
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            int targetIndex = newRow * 8 + newCol;

            if (cell[targetIndex].piece.type == NONE_TYPE || cell[targetIndex].piece.color != color) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
        }
    }
    
    MovesArray* moveobj = (MovesArray*)malloc(sizeof(MovesArray));
    if (moveobj == NULL) {
        printf("Memory allocation failed\n");
        free(moves);
        moves = NULL;
        return NULL;
    }

    moveobj->moves = moves;
    moveobj->moveCount = moveCount;
    return moveobj;
}

static MovesArray* generateQueenMoves(CellState* cell, int row, int col) {
    int startingIndex = row * 8 + col;
    PieceColor currentPieceColor = cell[startingIndex].piece.color;
    int maxMoves = 28;

    Move** moves = (Move**)malloc(maxMoves * sizeof(Move*));
    if (moves == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int moveCount = 0;

    bool hLeft = true;
    bool hRight = true;
    bool vUp = true;
    bool vDown = true;

    for (int i = 1; i < 8; i++) {
        // Horizontal Right
        int targetIndex = row * 8 + (col + i);
        if (hRight && col + i < 8) {
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                hRight = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                hRight = false;
            }
        }

        // Horizontal Left
        targetIndex = row * 8 + (col - i);
        if (hLeft && col - i >= 0) {
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                hLeft = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                hLeft = false;
            }
        }

        // Vertical Up
        targetIndex = (row - i) * 8 + col;
        if (vUp && row - i >= 0) {
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                vUp = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                vUp = false;
            }
        }

        // Vertical Down
        targetIndex = (row + i) * 8 + col;
        if (vDown && row + i < 8) {
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                vDown = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                vDown = false;
            }
        }
    }

    bool topLeft = true;
    bool topRight = true;
    bool bottomLeft = true;
    bool bottomRight = true;

    for (int i = 1; i < 8; i++) {
        // Top-left diagonal
        if (topLeft && (row - i >= 0 && col - i >= 0)) {
            int targetIndex = (row - i) * 8 + (col - i);
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                topLeft = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                topLeft = false;
            }

        }
        // Top-right diagonal
        if (topRight && (row - i >= 0 && col + i < 8)) {
            int targetIndex = (row - i) * 8 + (col + i);

            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                topRight = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
                topRight = false;
            }

        }
        // Bottom-left diagonal
        if (bottomLeft && (row + i < 8 && col - i >= 0)) {
            int targetIndex = (row + i) * 8 + (col - i);

            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                bottomLeft = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                bottomLeft = false;
            }
        }
        // Bottom-right diagonal
        if (bottomRight && (row + i < 8 && col + i < 8)) {
            int targetIndex = (row + i) * 8 + (col + i);

            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                bottomRight = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                bottomRight = false;
            }
        }
    }



    MovesArray* moveobj = (MovesArray*)malloc(sizeof(MovesArray));
    if (moveobj == NULL) {
        printf("Memory allocation failed\n");
        free(moves);
        moves = NULL;
        return NULL;
    }

    moveobj->moves = moves;
    moveobj->moveCount = moveCount;
    return moveobj;
}

static MovesArray* generateRookMoves(CellState* cell,int row, int col) {
    int startingIndex = row * 8 + col;
    PieceColor currentPieceColor = cell[startingIndex].piece.color;
    int maxMoves = 14;

    Move** moves = (Move**)malloc(maxMoves * sizeof(Move*));
    if (moves == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int moveCount = 0;

    bool hLeft = true;
    bool hRight = true;
    bool vUp = true;
    bool vDown = true;

    for (int i = 1; i < 8; i++) {
        // Horizontal Right
        int targetIndex = row * 8 + (col + i);
        if (hRight && col + i < 8) {
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                hRight = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                hRight = false;
            }
        }

        // Horizontal Left
        targetIndex = row * 8 + (col - i);
        if (hLeft && col - i >= 0) {
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                hLeft = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                hLeft = false;
            }
        }

        // Vertical Up
        targetIndex = (row - i) * 8 + col;
        if (vUp && row - i >= 0) {
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                vUp = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                vUp = false;
            }
        }

        // Vertical Down
        targetIndex = (row + i) * 8 + col;
        if (vDown && row + i < 8) {
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                vDown = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                vDown = false;
            }
        }
    }


    MovesArray* moveobj = (MovesArray*)malloc(sizeof(MovesArray));
    if (moveobj == NULL) {
        printf("Memory allocation failed\n");
        free(moves);
        moves = NULL;
        return NULL;
    }

    moveobj->moves = moves;
    moveobj->moveCount = moveCount;
    return moveobj;
}

static MovesArray* generateBishopMoves(CellState* cell, int row, int col) {
    int startingIndex = row * 8 + col;
    PieceColor currentPieceColor = cell[startingIndex].piece.color;
    int maxMoves = 14;

    Move** moves = (Move**)malloc(maxMoves * sizeof(Move*));
    if (moves == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int moveCount = 0;

    bool topLeft = true;
    bool topRight = true;
    bool bottomLeft = true;
    bool bottomRight = true;

    for (int i = 1; i < 8; i++) {
        // Top-left diagonal
        if (topLeft && (row - i >= 0 && col - i >= 0)) {
            int targetIndex = (row - i) * 8 + (col - i);
            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                topLeft = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                topLeft = false;
            }

        }
        // Top-right diagonal
        if (topRight && (row - i >= 0 && col + i < 8)) {
            int targetIndex = (row - i) * 8 + (col + i);

            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                topRight = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
                topRight = false;
            }

        }
        // Bottom-left diagonal
        if (bottomLeft && (row + i < 8 && col - i >= 0)) {
            int targetIndex = (row + i) * 8 + (col - i);

            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                bottomLeft = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                bottomLeft = false;
            }
        }
        // Bottom-right diagonal
        if (bottomRight && (row + i < 8 && col + i < 8)) {
            int targetIndex = (row + i) * 8 + (col + i);

            if (cell[targetIndex].piece.type == NONE_TYPE) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
            else if (cell[targetIndex].piece.color == currentPieceColor) {
                bottomRight = false;
            }
            else if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);

                bottomRight = false;
            }
        }
    }

    MovesArray* moveobj = (MovesArray*)malloc(sizeof(MovesArray));
    if (moveobj == NULL) {
        printf("Memory allocation failed\n");
        free(moves);
        moves = NULL;
        return NULL;
    }

    moveobj->moves = moves;
    moveobj->moveCount = moveCount;
    return moveobj;
}

static MovesArray* generateKnightMoves(CellState* cell, int row, int col) {
    int startingIndex = row * 8 + col;
    PieceColor currentPieceColor = cell[startingIndex].piece.color;
    int maxMoves = 8;
    static const int rowOffsets[] = { 2, 2, -2, -2, 1, 1, -1, -1 };
    static const int colOffsets[] = { 1, -1, 1, -1, 2, -2, 2, -2 };

    Move** moves = (Move**)malloc(maxMoves * sizeof(Move*));
    if (moves == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int moveCount = 0;

    for (int i = 0; i < 8; ++i) {
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];

        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            int targetIndex = newRow * 8 + newCol;

            if (cell[targetIndex].piece.color != currentPieceColor) {
                addMove(cell, moves, &moveCount, startingIndex, targetIndex);
            }
        }
    }

    MovesArray* moveobj = (MovesArray*)malloc(sizeof(MovesArray));
    if (moveobj == NULL) {
        printf("Memory allocation failed\n");
        free(moves);
        moves = NULL;
        return NULL;
    }

    moveobj->moves = moves;
    moveobj->moveCount = moveCount;
    return moveobj;
}

MovesArray* generateMoves(CellState* cell, int row, int col) {
    int startingIndex = row * 8 + col;
    switch (cell[startingIndex].piece.type) {
    case PAWN:
        return generatePawnMoves(cell, row, col);
    case KING:
        return generateKingMoves(cell,  row, col);
    case QUEEN:
        return generateQueenMoves(cell, row, col);
    case ROOK:
        return generateRookMoves(cell, row, col);
    case BISHOP:
        return generateBishopMoves(cell, row, col);
    case KNIGHT:
        return generateKnightMoves(cell, row, col);
    default:
        return NULL;
    }
    
}
