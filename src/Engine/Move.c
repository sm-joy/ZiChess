#include "../../include/Engine/Move.h"
#include "../../include/Utils.h"


static void AddMove(Board* board, MoveList* moveList, int startingCell, int targetCell) {
	if (targetCell < 0 || targetCell >= 64) return;
	board->cell[targetCell].isTarget = true;
	moveList->moves[moveList->moveCount].startingCell = startingCell;
	moveList->moves[moveList->moveCount++].targetCell = targetCell;
}

void ClearMoveHighlights(Board* board, MoveList* moveList) {
    if (moveList->moveCount <= 0) return;

    for (int i = 0; i < moveList->moveCount; ++i) {
        board->cell[moveList->moves[i].targetCell].isTarget = false;
    }
    board->cell[moveList->moves[0].startingCell].isSelected = false;
    board->selectedCell = -1;
    moveList->moveCount = 0;
}

static void GeneratePawnMoves(Board* board, MoveList* moveList, int startingCell) {
    int row = INDEX2ROW(startingCell);
    int col = INDEX2COL(startingCell);
    int newRow, newCol, targetCell;
    PieceColor startingPieceColor = board->cell[startingCell].color;

    newRow = startingPieceColor == WHITE ? row + 1 : row - 1;
    newCol = col;
    targetCell = ROWCOL2INDEX(newRow, newCol);
    if (newRow >= 0 && newRow < 8 && board->cell[targetCell].type == NONE_TYPE) {
        AddMove(board, moveList, startingCell, targetCell);

        if ((startingPieceColor == WHITE && row == 1) || (startingPieceColor == BLACK && row == 6)) {
            newRow = startingPieceColor == WHITE ? row + 2 : row - 2;
            targetCell = ROWCOL2INDEX(newRow, newCol);
            if (board->cell[targetCell].type == NONE_TYPE) {
                AddMove(board, moveList, startingCell, targetCell);
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        newCol = col + ((i == 0) ? 1 : -1);
        newRow = startingPieceColor == WHITE ? row + 1 : row - 1;
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            targetCell = ROWCOL2INDEX(newRow, newCol);
            if (board->cell[targetCell].color != startingPieceColor && board->cell[targetCell].type != NONE_TYPE) {
                AddMove(board, moveList, startingCell, targetCell);
            }
        }
    }
}


static void GenerateKnightMoves(Board* board, MoveList* moveList, int startingCell) {
    int row = INDEX2ROW(startingCell);
    int col = INDEX2COL(startingCell);
    int newRow, newCol, targetCell;
    static const int knightRowOffsets[] = { 2, 2, -2, -2, 1, 1, -1, -1 };
    static const int knightColOffsets[] = { 1, -1, 1, -1, 2, -2, 2, -2 };

    for (int i = 0; i < 8; ++i) {
        newRow = row + knightRowOffsets[i];
        newCol = col + knightColOffsets[i];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            targetCell = ROWCOL2INDEX(newRow, newCol);

            if (board->cell[startingCell].type == NONE_TYPE || 
                board->cell[startingCell].color != board->cell[targetCell].color) {
                AddMove(board, moveList, startingCell, targetCell);
            }
        }
    }
}


static void GenerateDiagonalMoves(Board* board, MoveList* moveList, int startingCell) {
    bool active[4] = { true, true, true, true };
    int newRow = -1, newCol = -1, targetCell = -1;
    int row = INDEX2ROW(startingCell);
    int col = INDEX2COL(startingCell);
    PieceColor startingPieceColor = board->cell[startingCell].color;
    PieceColor targetPieceColor = NONE_COLOR;
    PieceType targetPieceType = NONE_TYPE;

    static const int diagonalRowOffsets[4] = { -1, -1, 1, 1 };
    static const int diagonalColOffsets[4] = { 1, -1, 1, -1 };

    for (int i = 1; i < 8; ++i) {
        for (int d = 0; d < 4; ++d) {
            if (!active[d]) continue;
            newRow = row + i * diagonalRowOffsets[d];
            newCol = col + i * diagonalColOffsets[d];
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                targetCell = ROWCOL2INDEX(newRow, newCol);
                targetPieceType = board->cell[targetCell].type;
                targetPieceColor = board->cell[targetCell].color;

                if (targetPieceType == NONE_TYPE) AddMove(board, moveList, startingCell, targetCell);
                else if (targetPieceColor == startingPieceColor) active[d] = false;
                else {
                    AddMove(board, moveList, startingCell, targetCell);
                    active[d] = false;
                }
            }
            else active[d] = false;
        }
    }
}


static void GenerateOrthogonalMoves(Board* board, MoveList* moveList, int startingCell) {
    bool active[4] = { true, true, true, true }; 
    int newRow = -1, newCol = -1, targetCell = -1;
    int row = INDEX2ROW(startingCell);
    int col = INDEX2COL(startingCell);
    PieceColor startingPieceColor = board->cell[startingCell].color;
    PieceColor targetPieceColor = NONE_COLOR;
    PieceType targetPieceType = NONE_TYPE;

    static const int rowOffsets[4] = { -1, 1, 0, 0 }; 
    static const int colOffsets[4] = { 0, 0, -1, 1 };

    for (int i = 1; i < 8; ++i) {
        for (int d = 0; d < 4; ++d) {
            if (!active[d]) continue;
            newRow = row + i * rowOffsets[d];
            newCol = col + i * colOffsets[d];
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                targetCell = ROWCOL2INDEX(newRow, newCol);
                targetPieceType = board->cell[targetCell].type;
                targetPieceColor = board->cell[targetCell].color;

                if (targetPieceType == NONE_TYPE) AddMove(board, moveList, startingCell, targetCell);             
                else if (targetPieceColor == startingPieceColor) active[d] = false;
                else {
                    AddMove(board, moveList, startingCell, targetCell);
                    active[d] = false;
                }
            }
            else active[d] = false; 
        }
    }
}


static void GenerateQueenMoves(Board* board, MoveList* moveList, int startingCell) {

    GenerateDiagonalMoves(board, moveList, startingCell);
    GenerateOrthogonalMoves(board, moveList, startingCell);
}


static void GenerateBishopMoves(Board* board, MoveList* moveList, int startingCell) {
    GenerateDiagonalMoves(board, moveList, startingCell);
}


static void GenerateRookMoves(Board* board, MoveList* moveList, int startingCell) {
    GenerateOrthogonalMoves(board, moveList, startingCell);
}


static void GenerateKingMoves(Board* board, MoveList* moveList, int startingCell) {
    static const int rowOffsets[] = { 1, 1, 1, 0, 0, -1, -1, -1 };
    static const int colOffsets[] = { 1, 0, -1, 1, -1, 1, 0, -1 };
    int row = INDEX2ROW(startingCell);
    int col = INDEX2COL(startingCell);
    int newRow = -1, newCol = -1, targetCell = -1;
    PieceColor startingPieceColor = board->cell[startingCell].color;
    for (int i = 0; i < 8; ++i) {
        newRow = row + rowOffsets[i];
        newCol = col + colOffsets[i];

        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            targetCell = ROWCOL2INDEX(newRow, newCol);
            if (board->cell[targetCell].color != startingPieceColor) {
                AddMove(board, moveList, startingCell, targetCell);
            }
        }
    }
}


void GeneratePieceMoves(Board* board, MoveList* moveList, int startingCell) {
    moveList->moveCount = 0;
    switch (board->cell[startingCell].type) {
        case KING:   GenerateKingMoves(board, moveList, startingCell); break;
        case QUEEN:  GenerateQueenMoves(board, moveList, startingCell); break;
        case ROOK:   GenerateRookMoves(board, moveList, startingCell); break;
        case BISHOP: GenerateBishopMoves(board, moveList, startingCell); break;
        case KNIGHT: GenerateKnightMoves(board, moveList, startingCell); break;
        case PAWN:   GeneratePawnMoves(board, moveList, startingCell); break;
        defualt: return; break;
    }
}


static void PromotePawn(Board* board, int targetCell, PieceType color, PieceType type) {
    board->cell[targetCell].type = type;
    board->cell[targetCell].color = color;
}


MoveSignal MovePiece(Board* board, int startingCell, int targetCell) {
    PieceType startingPieceType = board->cell[startingCell].type;
    PieceColor startingPieceColor = board->cell[startingCell].color;
    PieceType targetPieceType = board->cell[targetCell].type;
    PieceColor targetPieceColor = board->cell[targetCell].color;
    MoveSignal sig = MOVE_NORMAL;

    if (startingCell < 0 ||
        targetCell < 0 ||
        startingCell == targetCell ||
        startingPieceColor == targetPieceColor) return MOVE_ILLEGAL;
    else if (targetPieceType == NONE_TYPE) sig =  MOVE_NORMAL;
    else if (startingPieceColor != targetPieceColor) sig = MOVE_CAPTURE;

    if (startingPieceType == PAWN) {
        if ((startingPieceColor == WHITE && INDEX2ROW(targetCell) == 7) || (startingPieceColor == BLACK && INDEX2ROW(targetCell) == 0)) {
            PromotePawn(board, targetCell, startingPieceColor, QUEEN);
            sig = MOVE_PROMOTE;
        }
        else {
            board->cell[targetCell].type = startingPieceType;
            board->cell[targetCell].color = startingPieceColor;
        }
    }
    else {
        board->cell[targetCell].type = startingPieceType;
        board->cell[targetCell].color = startingPieceColor;
    }

    if (startingPieceColor == board->currentPlayerTurn) {
        board->currentPlayerTurn = (startingPieceColor == WHITE) ? BLACK : WHITE;
    }

    board->cell[startingCell].type = NONE_TYPE;
    board->cell[startingCell].color = NONE_COLOR;

    return sig;
}
