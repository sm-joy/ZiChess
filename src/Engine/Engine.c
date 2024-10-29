#include "../../include/Engine/Engine.h"
#include "../../include/Utils.h"
#include "../../include/Values/Config.h"


void CreateEngine(RenderContext* rc, Engine* engine, AssetManager* am) {
	if (engine){
		CreateBoard(&engine->board, LoadTexture(am, rc, pieceImagePath, "chess_piece"));
		engine->moveList.moveCount = 0;
	}
}