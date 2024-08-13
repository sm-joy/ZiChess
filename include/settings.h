#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL.h>

typedef struct WINDOW_DIMENSION {
	short int width, height;
} windowSize;

extern windowSize winsize;
extern int squareSize;
extern const char* pieceImagePath;
extern const char* winIconPath;
extern const char* montserratFontPath;
extern const char* opensansFontPath;

extern SDL_Color tileColor1;
extern SDL_Color tileColor2;
extern SDL_Color selectedColor;
extern SDL_Color targetColor1;
extern SDL_Color targetColor2;

extern const char* captureSoundPath;
extern const char* castleSoundPath;
extern const char* moveCheckSoundPath;
extern const char* moveSelfSoundPath;
extern const char* notifySoundPath;
extern const char* promoteSoundPath;

#endif // SETTINGS_H
