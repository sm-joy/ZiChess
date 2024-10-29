#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "../Ui.h"
#include "../Graphics.h"
#include "../Audio.h"
#include "SceneManager.h"
#include "../AssetManager.h"

void MenuInit(RenderContext* rc, WidgetManager* wm, AssetManager* am);
Scene RunMenuScene(RenderContext* rc, WidgetManager* wm, AssetManager* am);


#endif // MENU_SCENE_H

