#ifndef CORE_H
#define CORE_H

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Scenes/SceneManager.h"
#include "Settings.h"
#include "Graphics.h"
#include "Utils.h"
#include "Audio.h"
#include "Ui.h"

#define index(row, col) (row * 8 + col)

typedef struct POSITION{
	unsigned int x, y;
} Pos;

typedef struct {
	unsigned  int min, sec;
	Label* activeText;
	Label* inactiveText;
	bool isActive;
} Timer_t;


typedef enum CHESS_PIECE_TYPE {
	NONE_TYPE,
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


Mixer* CoreInit();
void destroyCore();

void DestroyTimer(Timer_t* timer);
Timer_t* CreateTimer(RenderContext* rc, int min, int sec, int posX, int posY, SDL_Color* activeTextColor, SDL_Color* inactiveTextColor, TTF_Font* font);
void TimerDecreement(RenderContext* rc, Timer_t* timer);

void loadPositionFromFen(const char* fen, CellState* board);
bool getCellPressed(int* row, int* col);
bool markSelected(CellState* cell, int row, int col);
void movePiece(CellState* cell, int fromRow, int fromCol, int toRow, int toCol);
MovesArray* generateMoves(CellState* cell, int row, int col);
void destroyMoves(CellState* cell, Move** moves, int moveCount);
void handleButtonEvent(WidgetManager* wm, SDL_Event* event);
Scene updateButtons(WidgetManager* wm);

#endif // CORE_H
