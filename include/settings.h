#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL.h>
#include "utils.h"

typedef struct WINDOW_DIMENSION {
	short int width, height;
} windowSize;

extern windowSize winsize;
extern int squareSize;
extern char* pieceImagePath;
extern char* knightBgPath1;
extern char* knightBgPath2;
extern char* knightBgPath3;
extern char* kingBgPath1;
extern char* queenBgPath1;
extern char* winIconPath;
extern char* montserratFontPath;
extern char* opensansFontPath;

extern SDL_Color tileColor1;
extern SDL_Color tileColor2;
extern SDL_Color selectedColor;
extern SDL_Color targetColor1;
extern SDL_Color targetColor2;

extern char* captureSoundPath;
extern char* castleSoundPath;
extern char* moveCheckSoundPath;
extern char* moveSelfSoundPath;
extern char* notifySoundPath;
extern char* promoteSoundPath;

extern char* frozenStarMusicPath;
extern char* gymnopediNo1MusicPath;

void initSettings();
void cleanSettings();

#endif // SETTINGS_H
