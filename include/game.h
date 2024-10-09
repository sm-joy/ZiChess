#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdbool.h>

#include "Graphics.h"
#include "core.h"
#include "Audio.h"
#include "Ui.h"


bool initializeGame();
Scene run_game();
void clean();
void initMenu();
Scene runMainMenu();
Scene runStartUp();
void renderFrame(RenderContext* rc, CellState* cell);
void initOptionsMenu();
Scene runOptionsmenu();


#endif
