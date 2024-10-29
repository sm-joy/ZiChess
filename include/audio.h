#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "Utils.h"
#include "AssetManager.h"

#define MUSIC_WAIT_TIME 3000 //3 seconds
#define MUSIC_FADE_TIME 3000 //3 seconds

void Audio_PlaySound(Mix_Chunk* sound);
void Audio_PlayMusic(Mix_Music* music, bool fade);
void Auido_SetVolume(int percent);
void Audio_PlayRandomMusic(AssetManager* am);
void Audio_UpdateMusic(AssetManager* am);
void Audio_StopMusic();

#endif // AUDIO_H
