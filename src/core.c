#include "../include/core.h"

Piece kw = { WHITE | KING, {0, 0, 333, 334} };
Piece kb = { BLACK | KING, {0, 334, 333, 334} };
Piece qw = { WHITE | QUEEN, {333, 0, 333, 334} };
Piece qb = { BLACK | QUEEN, {333, 334, 333, 334} };
Piece pw = { WHITE | PAWN, {1665, 0, 333, 334} };
Piece pb = { BLACK | PAWN, {1665, 334, 333, 334} };
Piece bw = { WHITE | BISHOP, {666, 0, 333, 334} };
Piece bb = { BLACK | BISHOP, {666, 334, 333, 334} };
Piece nw = { WHITE | KNIGHT, {999, 0, 333, 334} };
Piece nb = { BLACK | KNIGHT, {999, 334, 333, 334} };
Piece rw = { WHITE | ROOK, {1332, 0, 333, 334} };
Piece rb = { BLACK | ROOK, {1332, 334, 333, 334} };
Piece none = { NONE, {0, 0, 0, 0} };

static int getPieceColor(CellState* cell) {
    return cell->piece.state & (WHITE | BLACK);
}

void LoadPositionFromFen(const char* fen, CellState *board) {
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

bool getCellPressed(int* row, int* col) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    (*row) = (int)(mouseY / squareSize);
    (*col) = (int)(mouseX / squareSize);
    if (0 <= (*row) && (*row) < 8 && 0 <= (*col) && (*col) < 8) {
        return true;
    }
    else {
        return false;
    }
}

bool markSelected(CellState* cell, int row, int col) {
    int index = 8 * row + col;
    if (cell[index].piece.state != NONE) {
        cell[index].selected = true;
        cell[index].tileColor = selectedColor;
        return true;
    }
    else {
        return false;
    }
}


void movePiece(CellState* cell, int fromRow, int fromCol, int toRow, int toCol, Mix_Chunk* moveSound, Mix_Chunk* captureSound, Mix_Chunk* promoteSound) {
    int fromIndex = fromRow * 8 + fromCol;
    int toIndex = toRow * 8 + toCol;

    cell[fromIndex].selected = false;
    cell[fromIndex].tileColor = (fromRow + fromCol) % 2 == 0 ? tileColor1 : tileColor2;

    if (fromIndex == toIndex || getPieceColor(&cell[fromIndex]) == getPieceColor(&cell[toIndex])) {
        return;
    }
    else if (cell[toIndex].piece.state == NONE) {
        playSound(moveSound);
    }
    else if (fromIndex == toIndex || getPieceColor(&cell[fromIndex]) != getPieceColor(&cell[toIndex])) {
        playSound(captureSound);
    }

    if (cell[fromIndex].piece.state == (WHITE | PAWN) && toRow == 7) {
        cell[toIndex].piece = qw; // Promote to white queen
        playSound(promoteSound);
    }
    else if (cell[fromIndex].piece.state == (BLACK | PAWN) && toRow == 0) {
        cell[toIndex].piece = qb; // Promote to black queen
        playSound(promoteSound);
    }
    else {
        cell[toIndex].piece = cell[fromIndex].piece;
    }
    cell[fromIndex].piece.state = NONE;
    cell[fromIndex].piece.srcRect = (SDL_Rect){ 0, 0, 0, 0 };
    printf("Moving piece from [%d, %d] (index %d) to [%d, %d] (index %d)\n", fromRow, fromCol, fromIndex, toRow, toCol, toIndex);
}


static bool isValidMove(int targetIndex, CellState* cell, int pieceColor) {
    return (targetIndex >= 0 && targetIndex < 64) && (cell[targetIndex].piece.state == NONE || getPieceColor(&cell[targetIndex]) != pieceColor);
}

Move** generateMoves() {
    int maxMoves = 64;
    Move** moves = (Move**)malloc(maxMoves * sizeof(Move*));
    return moves;
}
