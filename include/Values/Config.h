#ifndef CONFIG_H
#define CONFIG_H

typedef struct WINDOW_DIMENSION {
	short int width, height;
	short int centerX, centerY;
} windowSize;
extern windowSize winsize;

extern const char* anonymouseProfileName;
extern char* anonymouseProfileImgPath;


extern char* selfProfileImgPath;
extern char* enemyProfileImgPath;

extern char* selfProfileName;
extern char* enemyProfileName;


void ConfigInit();
void ConfigClean();


#endif // CONFIG_H
