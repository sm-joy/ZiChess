#ifndef MOVE_H
#define MOVE_H


#include "Board.h"

#define MAX_MOVES 32
#define BOARD_SIZE 8
#define ROWCOL2INDEX(row, col) ((row) * BOARD_SIZE + (col))
#define INDEX2ROW(index) ((index) / BOARD_SIZE)
#define INDEX2COL(index) ((index) % BOARD_SIZE)

typedef enum {
    MOVE_NONE,
    MOVE_NORMAL,
    MOVE_CHECK,
    MOVE_CHECKMATE,
    MOVE_CASTLE,
    MOVE_PROMOTE,
    MOVE_CAPTURE,
    MOVE_EN_PASSANT,
    MOVE_STALEMATE,
    MOVE_ILLEGAL
} MoveSignal;

typedef struct ChessPieceMove {
	int startingCell, targetCell;
} Move;

typedef struct {
	Move moves[MAX_MOVES];
	int moveCount;
} MoveList;


void GeneratePieceMoves(Board* board, MoveList* moveList, int startingCell);
void ClearMoveHighlights(Board* board, MoveList* moveList);
MoveSignal MovePiece(Board* board, int startingCell, int targetCell);

#endif // MOVE_H
