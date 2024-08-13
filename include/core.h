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

typedef struct POSITION{
	unsigned int x, y;
} Pos;


typedef enum CHESS_PIECE_Type {
	NONE,
	KING,
	QUEEN,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK
} PieceType;

typedef enum CHESS_PIECE_COLOR {
	NOCOLOR,
	WHITE,
	BLACK
} PieceColor;

typedef struct CHESS_PIECE {
	PieceColor color;
	PieceType type;
	SDL_Rect srcRect;
} Piece;

typedef struct {
	Piece piece;
	SDL_Rect dstRect;
	bool isSelected;
	bool istarget;
	int index;
} CellState;



typedef struct {
	int startingCell, targetCell;
} Move;

typedef struct {
	Move** moves;
	int moveCount;
} MovesArray;


typedef enum SCENE_STATE {
	MAIN_MENU,
	MAIN_GAME,
	OPTIONS,
	QUIT
} Scene;

void CoreInit();
void destroyCore();

void loadPositionFromFen(const char* fen, CellState* board);
bool getCellPressed(int* row, int* col);
bool markSelected(CellState* cell, int row, int col);
void movePiece(CellState* cell, int fromRow, int fromCol, int toRow, int toCol);
MovesArray* generateMoves(CellState* cell, int row, int col);
void destroyMoves(CellState* cell, Move** moves, int moveCount);
#endif // CORE_H
