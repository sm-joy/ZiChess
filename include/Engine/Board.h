#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "SDL.h"
#include "../../include/Graphics.h"
#include "stdbool.h"

#define BOARD_DIMENSION 8


typedef struct CELL_STATE {
	PieceType type;
	PieceColor color;
	SDL_Rect dstRect;
	bool isSelected, isTarget;
} Cell;

typedef struct {
	Cell cell[64];
	PieceColor currentPlayerTurn;
	SDL_Texture* pieceTex;
	int selectedCell, lastSelected;
} Board;

void CreateBoard(Board* board, SDL_Texture* pieceTexture);
void RenderBoard(Board* board, RenderContext* rc);
void LoadFenBoard(Board* board, const char* fen);
bool MarkCellSelected(Board* board, int index);
bool CheckCellPress(int* index);


#endif //BOARD_H
