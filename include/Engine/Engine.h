#ifndef ENGINE_H
#define ENGINE_H

#include "Board.h"
#include "Move.h"
#include "../AssetManager.h"

typedef struct {
	Board board;
	MoveList moveList;
} Engine;

void CreateEngine(RenderContext* rc, Engine* engine, AssetManager* am);


#endif // ENGINE_H
