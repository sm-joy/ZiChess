#ifndef PIECE_H
#define PIECE_H

#include <SDL.h>


typedef enum {
    NONE_TYPE = 0,
    KING = 1,
    QUEEN = 2,
    ROOK = 3,
    BISHOP = 4,
    KNIGHT = 5,
    PAWN = 6,
} PieceType;

typedef enum {
    NONE_COLOR = 0,
    WHITE = 1,
    BLACK = 2
} PieceColor;


SDL_Rect* Piece_GetSrcRect(PieceType type, PieceColor color);



#endif // PIECE_H