#ifndef CORE_H
#define CORE_H

#include "Graphics.h"
#include "Ui.h"
#include "Audio.h"
#include "Scenes/SceneManager.h"
#include "Engine/Engine.h"
#include <stdbool.h>
#include "AssetManager.h"
#include "Values/Config.h"

typedef struct {
	RenderContext rc;
	WidgetManager wm;
	Engine engine;
	Scene currentScene;
	AssetManager am;
	Config config;
} Core;


bool CreateCore(Core* core);
void CoreLoop(Core* core);
void CoreDestroy(Core* core);

#endif // CORE_H
