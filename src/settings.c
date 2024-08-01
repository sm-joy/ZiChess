#include "../include/settings.h"


int squareSize = 74;
windowSize winsize = { 74*8+300, 74*8 };
const char* pieceImagePath = ".\\asset\\images\\pieces\\Pieces.png";

const char* captureSoundPath = ".\\asset\\audio\\capture.wav";
const char* castleSoundPath = ".\\asset\\audio\\castle.wav";
const char* moveCheckSoundPath = ".\\asset\\audio\\move-check.wav";
const char* moveSelfSoundPath = ".\\asset\\audio\\move-self.wav";
const char* notifySoundPath = ".\\asset\\audio\\notify.wav";
const char* promoteSoundPath = ".\\asset\\audio\\promote.wav";

SDL_Color tileColor1 = { 180, 136, 97, 255 };
SDL_Color tileColor2 = { 239, 217, 180, 255 }; 
SDL_Color selectedColor = { 222, 201, 110, 220 };
