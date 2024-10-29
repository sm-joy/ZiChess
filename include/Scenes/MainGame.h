#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "SceneManager.h"
#include "../Engine/Engine.h"
#include "../../include/Graphics.h"
#include "../../include/Ui.h"

void MainGameInit(RenderContext* rc, WidgetManager* wm, Engine* engine, AssetManager* am);
Scene MainGameRun(RenderContext* rc, WidgetManager* wm, Engine* engine);


#endif // MAIN_GAME_H

