#include "../include/Config.h"
#include <stdio.h> //currently for NULL
#include "../include/Utils.h"


const char* anonymouseProfileName = "Anonymouse";
char* anonymouseProfileImgPath = NULL;

char* selfProfileImgPath = NULL;
char* enemyProfileImgPath = NULL;

char* selfProfileName = NULL;
char* enemyProfileName = NULL;

void ConfigInit() {
	anonymouseProfileImgPath = getAbsolutePath("..\\..\\asset\\images\\anonymouse_user.png");
}
