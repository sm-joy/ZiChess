#include "../../include/Engine/Board.h"
#include "../../include/Values/Config.h"
#include "../../include/Values/Colors.h"

static int boardStartX, boardStartY;
static const char* startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void CreateBoard(Board* board, SDL_Texture* pieceTexture) {
    if (!board || !pieceTexture) return;

    board->pieceTex = pieceTexture;
    board->currentPlayerTurn = BLACK;
    board->selectedCell = -1;
    board->lastSelected = -1;

    boardStartX = (winsize.centerX - (4 * boardSquareSize));
    boardStartY = (winsize.centerY - (4 * boardSquareSize));

    Cell* currentCell = &board->cell;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            currentCell->type = NONE_TYPE;
            currentCell->color = NONE_COLOR;

            currentCell->isSelected = false;
            currentCell->isTarget = false;

            currentCell->dstRect.x = boardStartX + j * boardSquareSize;
            currentCell->dstRect.y = boardStartY + i * boardSquareSize;
            currentCell->dstRect.w = boardSquareSize;
            currentCell->dstRect.h = boardSquareSize;

            currentCell++;
        }
    }
}

void RenderBoard(Board* board, RenderContext* rc) {
    SDL_Rect* srcRect = NULL;
    SDL_Color* tileColor = NULL;
    Cell* currentCell = board->cell;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            bool isEven = (i + j) % 2 == 0;
            tileColor = NULL;

            if (currentCell->isSelected) tileColor = &ColorSelectedSquare;
            else if (currentCell->isTarget) tileColor = isEven ? &ColorTargetSquare1 : &ColorTargetSquare2;
            else tileColor = isEven ? &ColorSquare1 : &ColorSquare2;

            srcRect = Piece_GetSrcRect(currentCell->type, currentCell->color);

            GFX_RenderRect(rc, &(currentCell->dstRect), tileColor, true);
            if (srcRect) renderTextureEa(rc, board->pieceTex, srcRect, &currentCell->dstRect);

            currentCell++;
        }
    }
}

void LoadFenBoard(Board* board, const char* fen) {
    int file = 0;
    int rank = 7; 
    int index;
    char symbol;
    char upperSymbol;
    PieceType type;
    PieceColor color;

    for (int i = 0; fen[i] != '\0'; ++i) {
        symbol = fen[i];

        if (symbol == ' ') break; 
        else if (symbol == '/') {
            file = 0;
            rank--;
        }
        else if (symbol >= '1' && symbol <= '8') {
            file += (int)(symbol - '0'); 
        }
        else {
            index = rank * 8 + file;
            type = NONE_TYPE;
            color = NONE_COLOR;
            color = (symbol >= 'A' && symbol <= 'Z') ? WHITE : BLACK;
            upperSymbol = (symbol >= 'a' && symbol <= 'z') ? (symbol - 'a' + 'A') : symbol;

            switch (upperSymbol) {
            case 'R': type = ROOK;   break;
            case 'N': type = KNIGHT; break;
            case 'B': type = BISHOP; break;
            case 'Q': type = QUEEN;  break;
            case 'K': type = KING;   break;
            case 'P': type = PAWN;   break;
            default:
                type = NONE_TYPE;
                color = NONE_COLOR;
                break;
            }

            board->cell[index].type = type;
            board->cell[index].color = color;
            file++;
        }
    }
}


bool CheckCellPress(int* index) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    int relMouseX = mouseX - boardStartX;
    int relMouseY = mouseY - boardStartY;

    if (relMouseX >= 0 && relMouseX < 8 * boardSquareSize && relMouseY >= 0 && relMouseY < 8 * boardSquareSize) {
        *index = (relMouseY / boardSquareSize) * 8 + (relMouseX / boardSquareSize);
        return true;
    }
    return false;
}


bool MarkCellSelected(Board* board, int index) {
    if (index < 0 || index >= 64) return false;

    Cell* selectedCell = &board->cell[index];
    if (selectedCell->type == NONE_TYPE) return false;

    if (board->currentPlayerTurn == selectedCell->color) selectedCell->isSelected = true;
    else return false;
    
    board->selectedCell = index;
    return true;
}
