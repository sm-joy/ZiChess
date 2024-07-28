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
} CellState;



typedef enum SCENE_STATE {
	MAIN_MENU,
	MAIN_GAME,
	OPTIONS,
	QUIT
} Scene;


bool initialize_game();

Scene run_game();
void clean();
Scene runMainMenu();
void LoadPositionFromFen(const char* fen, CellState* board);

#endif // CORE_H
