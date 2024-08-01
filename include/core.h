#ifndef CORE_H
#define CORE_H

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "settings.h"
#include "renderer.h"
#include "utils.h"
#include "audio.h"

#define index(row, col) row * 8 + col

typedef enum CHESS_PIECE_STATE {
	NONE = 0,
	KING = 1,
	QUEEN = 2,
	PAWN = 3,
	KNIGHT = 4,
	BISHOP = 5,
	ROOK = 6,

	WHITE = 8,
	BLACK = 16
} PieceState;

typedef struct CHESS_PIECE {
	PieceState state;// or can be called id = COLOR | PIECE or NONE (0)
	SDL_Rect srcRect; // from the texture
} Piece;

typedef struct {
	Piece piece;
	SDL_Rect dstRect;
	bool selected;
	SDL_Color tileColor;
} CellState;

typedef enum {
	UP = -8,
	DOWN = 8,
	RIGHT = 1,
	LEFT = -1,
	TOP_LEFT = -9,
	TOP_RIGHT = -7,
	BOTTOM_LEFT = 9,
	BOTTOM_RIGHT = 7
} Directions;

typedef struct {
	int startingCell, targetCell;
} Move;


typedef enum SCENE_STATE {
	MAIN_MENU,
	MAIN_GAME,
	OPTIONS,
	QUIT
} Scene;

void LoadPositionFromFen(const char* fen, CellState* board);
bool getCellPressed(int* row, int* col);
bool markSelected(CellState* cell, int row, int col);
void movePiece(CellState* cell, int fromRow, int fromCol, int toRow, int toCol, Mix_Chunk* moveSound, Mix_Chunk* captureSound, Mix_Chunk* promoteSound);
Move** generateMoves();
#endif // CORE_H
