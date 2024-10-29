#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL.h>
#include <stdbool.h>

typedef struct {
	bool soundOn;
	int volume;
} Settings;

extern Settings settings;


#endif // SETTINGS_H
