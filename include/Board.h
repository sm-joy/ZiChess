#ifndef BOARD_H
#define BOARD_H

#include <SDL.h>
#include <stdbool.h>

typedef enum CHESS_PIECE_TYPE {
	NONE_TYPE = 0,
	KING,
	QUEEN,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK
} PieceType;

typedef enum CHESS_PIECE_COLOR {
	NONE_COLOR = 0,
	WHITE,
	BLACK
} PieceColor;

typedef struct CHESS_PIECE {
	PieceColor color;
	PieceType type;
	SDL_Rect srcRect;
} Piece;

typedef struct CHESS_BOARD_CELL {
	Piece piece;
	SDL_Rect dstRect;
	bool isSelected;
	bool istarget;
	int index;
} Cell;


//typedef struct CHESS_BOARD {
//	
//} Board;


//Board* CreateBoard();
//void RenderBoard(Board* board);
//void UpdateBoard(Board* board);


#endif //BOARD_H
