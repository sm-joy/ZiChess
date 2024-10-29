#include "../../include/Engine/Piece.h"


static const SDL_Rect WKingSrcRect   = { 0, 0, 333, 334 };
static const SDL_Rect BKingSrcRect   = {0, 334, 333, 334};
static const SDL_Rect WQueenSrcRect  = {333, 0, 333, 334};
static const SDL_Rect BQueenSrcRect  = {333, 334, 333, 334};
static const SDL_Rect WPawnSrcRect   = {1665, 0, 333, 334};
static const SDL_Rect BPawnSrcRect   = {1665, 334, 333, 334};
static const SDL_Rect WBishopSrcRect = {666, 0, 333, 334};
static const SDL_Rect BBishopSrcRect = {666, 334, 333, 334};
static const SDL_Rect WKnightSrcRect = {999, 0, 333, 334};
static const SDL_Rect BKnightSrcRect = {999, 334, 333, 334};
static const SDL_Rect WRookSrcRect   = {1332, 0, 333, 334};
static const SDL_Rect BRookSrcRect   = {1332, 334, 333, 334};



SDL_Rect* Piece_GetSrcRect(PieceType type, PieceColor color) {
    static const SDL_Rect* pieceRects[] = {
        // KING        QUEEN       ROOK        BISHOP      KNIGHT      PAWN
        &WKingSrcRect, &WQueenSrcRect, &WRookSrcRect, &WBishopSrcRect, &WKnightSrcRect, &WPawnSrcRect,  // WHITE
        &BKingSrcRect, &BQueenSrcRect, &BRookSrcRect, &BBishopSrcRect, &BKnightSrcRect, &BPawnSrcRect   // BLACK
    };

    if (type < KING || type > PAWN || color < WHITE || color > BLACK) return NULL;

    return pieceRects[((color - 1) * 6) + (type - 1)];
}