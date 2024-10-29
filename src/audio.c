#include "../include/Audio.h"

static Uint32 lastMusicEndTime = 0;
static bool musicPlaying = false;

void Audio_PlaySound(Mix_Chunk* sound) {
    if (Mix_PlayChannel(-1, sound, 0) == -1) {
        fprintf(stderr, "Mix_PlayChannel Error: %s\n", Mix_GetError());
    }
}

void Audio_PlayMusic(Mix_Music* music, bool fade) {
    if (!fade) {
        if (Mix_PlayMusic(music, 0) == -1) {
            fprintf(stderr, "Mix_PlayMusic Error: %s\n", Mix_GetError());
        }
    }
    else {
        Mix_FadeInMusic(music, 0, MUSIC_FADE_TIME);
    }
    musicPlaying = true;
}

void Auido_SetVolume(int percent) {
    if (percent <= 0) {
        Mix_VolumeMusic(0);
    }
    else if (percent >= 100){
        Mix_VolumeMusic(MIX_MAX_VOLUME);
    }
    else {
        Mix_VolumeMusic((percent * MIX_MAX_VOLUME) / 100);
    }
}

void Audio_PlayRandomMusic(AssetManager* am) {
    if (am->musicList.numMusic <= 0) {
        printf("No music loaded to play!\n");
        return;
    }
    int index = randint(0, am->musicList.numMusic - 1);
    Audio_PlayMusic(am->musicList.musics[index].music, true);
}

void Audio_UpdateMusic(AssetManager* am) {
    Uint32 currentTime = SDL_GetTicks();

    if (musicPlaying && !Mix_PlayingMusic()) {
        musicPlaying = false;
        lastMusicEndTime = currentTime;
    }

    if (!musicPlaying && (currentTime - lastMusicEndTime) >= MUSIC_WAIT_TIME) {
        Audio_PlayRandomMusic(am);
    }
}

void Audio_StopMusic() {
    if (musicPlaying && Mix_PlayingMusic()) {
        Mix_HaltMusic();
        musicPlaying = false;
    }
}

