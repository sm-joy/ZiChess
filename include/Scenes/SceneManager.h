#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

typedef enum SceneState {
	STARTUP,
	MAIN_MENU,
	MAIN_GAME,
	OPTIONS,
	QUIT
} Scene;


#include "LoadingScene.h"
#include "MenuScene.h"
#include "OptionsMenuScene.h"
#include "MainGame.h"


#endif // SCENE_MANAGER_H
