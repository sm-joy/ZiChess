#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "SceneManager.h"
#include "../Ui.h"
#include "../Graphics.h"
#include "../Audio.h"


void MenuInit(RenderContext* rc, WidgetManager* wm);
Scene RunMenuScene(RenderContext* rc, WidgetManager* wm, Mixer* mix);


#endif // MENU_SCENE_H

