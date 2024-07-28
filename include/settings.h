#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct WINDOW_DIMENSION {
	short int width, height;
} windowSize;

extern windowSize winsize;
extern int squareSize;
extern const char* pieceImagePath;


#endif // SETTINGS_H
