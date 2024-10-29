#ifndef CONFIG_H
#define CONFIG_H

#include "../Utils.h"

typedef struct WINDOW_DIMENSION {
	short int width, height;
	short int centerX, centerY;
} windowSize;
extern windowSize winsize;
extern const char* winTitle;

extern unsigned short int boardSquareSize;

extern const char* anonymouseProfileName;
extern char* anonymouseProfileImgPath;


extern char* selfProfileImgPath;
extern char* enemyProfileImgPath;

extern char* selfProfileName;
extern char* enemyProfileName;

extern char* captureSoundPath;
extern char* castleSoundPath;
extern char* moveCheckSoundPath;
extern char* moveSelfSoundPath;
extern char* notifySoundPath;
extern char* promoteSoundPath;

extern char* frozenStarMusicPath;
extern char* gymnopediaNo1MusicPath;


extern char* montserratBoldFontPath;
extern char* montserratMediumFontPath;
extern char* openSansRegularFontPath;


extern char* pieceImagePath;
extern char* knightBgPath1;
extern char* knightBgPath2;
extern char* knightBgPath3;
extern char* kingBgPath1;
extern char* queenBgPath1;
extern char* winIconPath;

typedef struct Config{
	char** filePaths;
	size_t numFilePath;
	size_t capacity;
} Config;

void CreateConfig(Config* config);
void DestroyConfig(Config* config);


#endif // CONFIG_H
