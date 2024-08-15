#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>

#include "utils.h"

#define MUSIC_WAIT_TIME 3000 //3 seconds
#define MUSIC_FADE_TIME 3000 //3 seconds

typedef struct {
    Mix_Chunk** sounds;
    Mix_Music** musics;
    int soundCount;    
    int soundCapacity;
    int musicCount;
    int musicCapacity;
    bool musicPlaying;
} Mixer;


Mixer* createMixer();
Mix_Chunk* loadSound(Mixer* audioSystem, const char* file);
Mix_Music* loadMusic(Mixer* audioSystem, const char* file);
void playSound(Mix_Chunk* sound);
void playMusic(Mixer* audioSystem, Mix_Music* music, bool fade);
void destroyMixer(Mixer* audioSystem);
void setVolume(int percent);
void playRandomMusic(Mixer* audioSystem);
void updateMusic(Mixer* audioSystem);

#endif // AUDIO_H
