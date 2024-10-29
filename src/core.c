#include "../include/Core.h"
#include "../include/Settings.h"

static void LoadAssets(AssetManager* am, RenderContext* rc) {
	LoadTexture(am, rc, knightBgPath1, "loading_scene_bg");
	LoadTexture(am, rc, kingBgPath1,   "menu_scene_bg");

	LoadSound(am, moveSelfSoundPath,  "move_self");
	LoadSound(am, captureSoundPath,   "capture");
	LoadSound(am, promoteSoundPath,   "promote");
	LoadSound(am, moveCheckSoundPath, "move_check");
	LoadSound(am, castleSoundPath,    "castle");
	LoadSound(am, notifySoundPath,    "notify");

	LoadMusic(am, gymnopediaNo1MusicPath, "gymnopedia_no1");
	LoadMusic(am, frozenStarMusicPath,    "frozen_star");
}

bool CreateCore(Core* core) {
	GFX_CreateRenderContext(&core->rc, winTitle, winsize.width, winsize.height);
	CreateAssetManager(&core->am);
	CreateConfig(&core->config);
	setWindowIcon(&core->rc, winIconPath);
	CreateEngine(&core->rc, &core->engine, &core->am);
	UI_CreateWidgetManager(&core->wm);

	LoadAssets(&core->am, &core->rc);

	core->currentScene = MAIN_MENU;
	MenuInit(&core->rc, &core->wm, &core->am);
	MainGameInit(&core->rc, &core->wm, &core->engine, &core->am);
	OptionsMenuInit(&core->rc, &core->wm);
	return true;
}

void CoreLoop(Core* core) {
	while (core->currentScene != QUIT) {
		switch (core->currentScene) {
			case STARTUP:   core->currentScene = RunLoadingScene(&core->rc, &core->wm, &core->am); break;
			case MAIN_MENU: core->currentScene = RunMenuScene(&core->rc, &core->wm, &core->am); break;
			case MAIN_GAME: core->currentScene = MainGameRun(&core->rc, &core->wm, &core->engine); break;
			case OPTIONS:   core->currentScene = RunOptionsMenu(&core->rc, &core->wm); break;
			default:		core->currentScene = QUIT; break;
		}
	}
}

void CoreDestroy(Core* core) {
	UI_DestroyWidgetManager(&core->wm);
	DestroyAssetManager(&core->am);
	GFX_DestroyRenderContext(&core->rc);
	DestroyConfig(&core->config);
}
