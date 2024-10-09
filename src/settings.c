#include "../include/Settings.h"
#include "../include/Utils.h"

int squareSize = 70;
windowSize winsize = { 1280 , 720 };

Settings settings = {true,100};

char* knightBgPath1 = NULL;
char* knightBgPath2 = NULL;
char* knightBgPath3 = NULL;
char* kingBgPath1 = NULL;
char* queenBgPath1 = NULL;

char* montserratBoldFontPath = NULL;
char* montserratMediumFontPath = NULL;
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
char* gymnopediNo1MusicPath = NULL;



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

	montserratBoldFontPath = getAbsolutePath("..\\..\\asset\\fonts\\Montserrat-Bold.ttf");
	montserratMediumFontPath = getAbsolutePath("..\\..\\asset\\fonts\\Montserrat-Medium.ttf");
	opensansFontPath = getAbsolutePath("..\\..\\asset\\fonts\\OpenSans-Regular.ttf");

	captureSoundPath = getAbsolutePath("..\\..\\asset\\audio\\sfx\\capture.wav");
	castleSoundPath = getAbsolutePath("..\\..\\asset\\audio\\\sfx\\castle.wav");
	moveCheckSoundPath = getAbsolutePath("..\\..\\asset\\audio\\\sfx\\move-check.wav");
	moveSelfSoundPath = getAbsolutePath("..\\..\\asset\\audio\\\sfx\\move-self.wav");
	notifySoundPath = getAbsolutePath("..\\..\\asset\\audio\\\sfx\\notify.wav");
	promoteSoundPath = getAbsolutePath("..\\..\\asset\\audio\\\sfx\\promote.wav");

	frozenStarMusicPath = getAbsolutePath("..\\..\\asset\\audio\\music\\FrozenStar.mp3");
	gymnopediNo1MusicPath = getAbsolutePath("..\\..\\asset\\audio\\music\\GymnopedieNo1.mp3");

	

}

void cleanSettings() {
	if (montserratBoldFontPath != NULL) free(montserratBoldFontPath);
	if (montserratMediumFontPath != NULL) free(montserratMediumFontPath);
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
	if (knightBgPath2 != NULL) free(knightBgPath2);
	if (knightBgPath3 != NULL) free(knightBgPath3);
	if (kingBgPath1 != NULL) free(kingBgPath1);
	if (queenBgPath1 != NULL) free(queenBgPath1);

	
}



