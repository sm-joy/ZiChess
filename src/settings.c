#include "../include/settings.h"

int squareSize = 74;
//windowSize winsize = { 74*8+300, 74*8 };
windowSize winsize = { 1280 , 720 };

char* montserratFontPath = NULL;
char* knightBgPath1 = NULL;
char* knightBgPath2 = NULL;
char* knightBgPath3 = NULL;
char* kingBgPath1 = NULL;
char* queenBgPath1 = NULL;
char* opensansFontPath = NULL;

char* pieceImagePath = NULL;
char* winIconPath = NULL;

char* captureSoundPath = NULL;
char* castleSoundPath = NULL;
char* moveCheckSoundPath = NULL;
char* moveSelfSoundPath = NULL;
char* notifySoundPath = NULL;
char* promoteSoundPath = NULL;

char* frozenStarMusicPath = NULL;
char* gymnopediNo1MusicPath = NULL;;



SDL_Color tileColor1 = { 180, 136, 97, 255 };
SDL_Color tileColor2 = { 239, 217, 180, 255 };
SDL_Color selectedColor = { 222, 201, 110, 220 };

//targetColor = { 199, 91, 84, 255 };
SDL_Color targetColor1 = { 189, 113, 90, 255 }; //blended 50%
SDL_Color targetColor2 = { 219, 154, 132, 255 }; //blended 50%

void initSettings() {
	pieceImagePath = getAbsolutePath("..\\..\\asset\\images\\pieces\\Pieces.png");
	knightBgPath1 = getAbsolutePath("..\\..\\asset\\images\\knight_bg1.png");
	knightBgPath2 = getAbsolutePath("..\\..\\asset\\images\\knight_bg2.png");
	knightBgPath3 = getAbsolutePath("..\\..\\asset\\images\\knight_bg3.png");
	kingBgPath1 = getAbsolutePath("..\\..\\asset\\images\\king_bg1.png");
	queenBgPath1 = getAbsolutePath("..\\..\\asset\\images\\queen_bg1.png");
	winIconPath = getAbsolutePath("..\\..\\asset\\images\\icon.png");

	montserratFontPath = getAbsolutePath("..\\..\\asset\\fonts\\Montserrat-Bold.ttf");
	opensansFontPath = getAbsolutePath("..\\..\\asset\\fonts\\OpenSans-Regular.ttf");

	captureSoundPath = getAbsolutePath("..\\..\\asset\\audio\\capture.wav");
	castleSoundPath = getAbsolutePath("..\\..\\asset\\audio\\castle.wav");
	moveCheckSoundPath = getAbsolutePath("..\\..\\asset\\audio\\move-check.wav");
	moveSelfSoundPath = getAbsolutePath("..\\..\\asset\\audio\\move-self.wav");
	notifySoundPath = getAbsolutePath("..\\..\\asset\\audio\\notify.wav");
	promoteSoundPath = getAbsolutePath("..\\..\\asset\\audio\\promote.wav");

	frozenStarMusicPath = getAbsolutePath("..\\..\\asset\\audio\\FrozenStar.mp3");
	gymnopediNo1MusicPath = getAbsolutePath("..\\..\\asset\\audio\\GymnopedieNo1.mp3");
}

void cleanSettings() {
	if (montserratFontPath != NULL) free(montserratFontPath);
	if (opensansFontPath != NULL) free(opensansFontPath);
	if (pieceImagePath != NULL) free(pieceImagePath);
	if (winIconPath != NULL) free(winIconPath);
	if (captureSoundPath != NULL) free(captureSoundPath);
	if (castleSoundPath != NULL) free(castleSoundPath);
	if (moveCheckSoundPath != NULL) free(moveCheckSoundPath);
	if (moveSelfSoundPath != NULL) free(moveSelfSoundPath);
	if (notifySoundPath != NULL) free(notifySoundPath);
	if (promoteSoundPath != NULL) free(promoteSoundPath);
	if (knightBgPath1 != NULL) free(knightBgPath1);
}
