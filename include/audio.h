#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>

typedef struct {
    Mix_Chunk** sounds;
    int soundCount;    
    int soundCapacity;
} Mixer;


Mixer* createMixer();
Mix_Chunk* loadSound(Mixer* audioSystem, const char* file);
void playSound(Mix_Chunk* sound);
void destroyMixer(Mixer* audioSystem);


#endif // AUDIO_H
