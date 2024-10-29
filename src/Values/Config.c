#include "../../include/Values/Config.h"
#include "../../include/Utils.h"
#include "../../include/zini.h"
#include <stdlib.h>
#include <string.h>

windowSize winsize = { 1280 , 720, 640, 360};
unsigned short int boardSquareSize = 70;
const char* winTitle = "ZiChess";

const char* anonymouseProfileName;
char* anonymouseProfileImgPath = NULL;

char* selfProfileImgPath = NULL;
char* enemyProfileImgPath = NULL;

char* selfProfileName = NULL;
char* enemyProfileName = NULL;


char* captureSoundPath = NULL;
char* castleSoundPath = NULL;
char* moveCheckSoundPath = NULL;
char* moveSelfSoundPath = NULL;
char* notifySoundPath = NULL;
char* promoteSoundPath = NULL;

char* frozenStarMusicPath = NULL;
char* gymnopediaNo1MusicPath = NULL;


char* montserratBoldFontPath = NULL;
char* montserratMediumFontPath = NULL;
char* openSansRegularFontPath = NULL;

char* pieceImagePath = NULL;
char* knightBgPath1 = NULL;
char* knightBgPath2 = NULL;
char* knightBgPath3 = NULL;
char* kingBgPath1 = NULL;
char* queenBgPath1 = NULL;
char* winIconPath = NULL;


static char* AddPathToConfig(Config* config, const char* filePath) {
	if (!config || !filePath) {
		if (!config) LogNullError("Config", NULL);
		if (!filePath) LogNullError("FilePath", NULL);
		return NULL;
	}

	if (config->numFilePath >= config->capacity) {
		config->capacity *= 2;
		char** newptr = (char**)realloc(config->filePaths, config->capacity * sizeof(char*));
		if (!newptr) {
			fprintf(stderr, "Realloc failed\n");
			config->capacity /= 2;
			return NULL;
		}
		config->filePaths = newptr;
	}
	config->filePaths[config->numFilePath] = _strdup(filePath);
	return config->filePaths[config->numFilePath++]; 
}

static void ConfigShrink(Config* config) {
	if (config->numFilePath < config->capacity) {
		char** newptr = (char**)realloc(config->filePaths, config->numFilePath * sizeof(char*));
		if (!newptr) {
			fprintf(stderr, "Realloc failed\n");
			return;
		}
		config->filePaths = newptr;
	}
}


void CreateConfig(Config* config) {

	config->numFilePath = 0;
	config->capacity = 4;
	config->filePaths = (char**)malloc(config->capacity * sizeof(char*));


	INIFILE configFile;
	char* configFilePath = getAbsolutePath("..\\..\\data\\config.ini");
	ZINI_Open(&configFile, configFilePath);

	Section* userProfile = ZINI_FindSection(&configFile, "UserProfile");
	anonymouseProfileName = ZINI_GetValue(userProfile, "user_name");
	anonymouseProfileImgPath = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(userProfile, "profile_picture_path")));


	Section* audioPaths = ZINI_FindSection(&configFile, "AudioPaths");
	captureSoundPath       = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(audioPaths,  "capture")));
	castleSoundPath        = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(audioPaths,  "castle")));
	moveCheckSoundPath     = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(audioPaths,  "move_check")));
	moveSelfSoundPath      = AddPathToConfig(config, getAbsolutePath( ZINI_GetValue(audioPaths, "move_self")));
	notifySoundPath        = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(audioPaths,  "notify")));
	promoteSoundPath       = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(audioPaths,  "promote")));
	frozenStarMusicPath    = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(audioPaths,  "frozen_star")));
	gymnopediaNo1MusicPath = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(audioPaths,  "gymnopedia_no1")));


	Section* fontPaths = ZINI_FindSection(&configFile, "FontPaths");
	montserratBoldFontPath   = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(fontPaths, "montserrat_bold")));
	montserratMediumFontPath = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(fontPaths, "montserrat_Medium")));
	openSansRegularFontPath  = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(fontPaths, "open_sans_regular")));

	Section* texPaths = ZINI_FindSection(&configFile, "TexturePaths");
	pieceImagePath = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(texPaths, "piece_atlas")));
	knightBgPath1  = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(texPaths, "knight_bg1")));
	knightBgPath2  = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(texPaths, "knight_bg2")));
	knightBgPath3  = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(texPaths, "knight_bg3")));
	kingBgPath1    = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(texPaths, "king_bg")));
	queenBgPath1   = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(texPaths, "queen_bg")));
	winIconPath    = AddPathToConfig(config, getAbsolutePath(ZINI_GetValue(texPaths, "win_icon")));

	ConfigShrink(config);

	ZINI_Save(&configFile, configFilePath);
	ZINI_Clean(&configFile);
	free(configFilePath);
}

void DestroyConfig(Config* config) {
	if (!config) return;
	if (!config->filePaths) return;

	for (size_t i = 0; i < config->numFilePath; ++i) {
		if (config->filePaths[i]) free(config->filePaths[i]);
		config->filePaths[i] = NULL;
	}

	free(config->filePaths);
	config->filePaths = NULL;
}
