#include "../../include/Values/Config.h"
#include "../../include/Utils.h"
#include "../../include/zini.h"
#include <stdio.h> //currently for NULL


windowSize winsize = { 1280 , 720, 640, 360};

const char* anonymouseProfileName;
char* anonymouseProfileImgPath = NULL;

char* selfProfileImgPath = NULL;
char* enemyProfileImgPath = NULL;

char* selfProfileName = NULL;
char* enemyProfileName = NULL;

void ConfigInit() {
	INIFILE configFile;
	char* configFilePath = getAbsolutePath("..\\..\\data\\config.ini");
	ZINI_Open(&configFile, configFilePath);
	if (!ZINI_SectionExists(&configFile, "UserProfile")) ZINI_AddSection(&configFile, "UserProfile");
	Section* userProfile = ZINI_FindSection(&configFile, "UserProfile");


	anonymouseProfileName = ZINI_GetValue(userProfile, "user_name");
	anonymouseProfileImgPath = getAbsolutePath(ZINI_GetValue(userProfile, "profile_picture_path"));

	ZINI_Save(&configFile, configFilePath);
	ZINI_Clean(&configFile);
	free(configFilePath);
}

void ConfigClean() {
	if (anonymouseProfileImgPath) free(anonymouseProfileImgPath);
}
