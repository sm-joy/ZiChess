#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdbool.h>

#include "renderer.h"
#include "core.h"
#include "audio.h"

bool initializeGame();
Scene run_game();
void clean();
void initMenu();
Scene runMainMenu();
void renderFrame(Renderer* renderer, CellState* cell);

#endif
