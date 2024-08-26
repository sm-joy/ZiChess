#include "../include/audio.h"

static Uint32 lastMusicEndTime = 0;

static void addSound(Mixer* audioSystem, Mix_Chunk* sound) {
    if (audioSystem->soundCount >= audioSystem->soundCapacity) {
        audioSystem->soundCapacity += 2;
        Mix_Chunk** newptr = realloc(audioSystem->sounds, audioSystem->soundCapacity * sizeof(Mix_Chunk*));
        if (newptr == NULL) {
            printf("Realloc Error: Unable to resize sounds array\n");
            return;
        }
        audioSystem->sounds = newptr;
    }
    audioSystem->sounds[audioSystem->soundCount++] = sound;
}

static void addMusic(Mixer* audioSystem, Mix_Music* music) {
    if (audioSystem->musicCount >= audioSystem->musicCapacity) {
        audioSystem->musicCapacity += 4;
        Mix_Music** newptr = realloc(audioSystem->musics, audioSystem->musicCapacity * sizeof(Mix_Music*));
        if (newptr == NULL) {
            printf("Realloc Error: Unable to resize sounds array\n");
            return;
        }
        audioSystem->musics = newptr;
    }
    audioSystem->musics[audioSystem->musicCount++] = music;
}


Mixer* createMixer() {
    Mixer* audioSystem = NULL;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Mix_OpenAudio Error: %s\n", Mix_GetError());
        return NULL;
    }

    audioSystem = (Mixer*)malloc(sizeof(Mixer));
    if (audioSystem == NULL) {
        fprintf(stderr, "Malloc Error: Unable to allocate AudioSystem! Error: %s\n", Mix_GetError());
        return NULL;
    }
    audioSystem->soundCount = 0;
    audioSystem->soundCapacity = 4;
    audioSystem->sounds = NULL;
    audioSystem->sounds = malloc(audioSystem->soundCapacity * sizeof(Mix_Chunk*));
    if (audioSystem->sounds == NULL) {
        fprintf(stderr, "Malloc Error: Unable to allocate sound array! Error: %s\n", Mix_GetError());
        destroyMixer(audioSystem);
        return NULL;
    }    
    audioSystem->musicCount = 0;
    audioSystem->musicCapacity = 4;
    audioSystem->musics = NULL;
    audioSystem->musics = malloc(audioSystem->musicCapacity * sizeof(Mix_Music*));
    if (audioSystem->musics == NULL) {
        fprintf(stderr, "Malloc Error: Unable to allocate music array! Error: %s\n", Mix_GetError());
        destroyMixer(audioSystem);
        return NULL;
    }

    audioSystem->musicPlaying = false;
    lastMusicEndTime = SDL_GetTicks();
    return audioSystem;
}

Mix_Chunk* loadSound(Mixer* audioSystem, const char* file) {
    Mix_Chunk* sound = Mix_LoadWAV(file);
    if (sound == NULL) {
        fprintf(stderr, "Mix_LoadWAV Error: %s\n", Mix_GetError());
        return NULL;
    }
    addSound(audioSystem, sound);
    return sound;
}

Mix_Music* loadMusic(Mixer* audioSystem, const char* file) {
    Mix_Music* music = Mix_LoadMUS(file);
    if (music == NULL) {

        fprintf(stderr, "Mix_LoadMUS Error: %s\n", Mix_GetError());
        return NULL;
    }
    addMusic(audioSystem, music);
    return music;
}

void playSound(Mix_Chunk* sound) {
    if (Mix_PlayChannel(-1, sound, 0) == -1) {
        fprintf(stderr, "Mix_PlayChannel Error: %s\n", Mix_GetError());
    }
}

void playMusic(Mixer* audioSystem, Mix_Music* music,  bool fade){
    if (!fade) {
        if (Mix_PlayMusic(music, 0) == -1) {
            fprintf(stderr, "Mix_PlayMusic Error: %s\n", Mix_GetError());
        }
    }
    else {
        Mix_FadeInMusic(music, 0, MUSIC_FADE_TIME);
    }
    audioSystem->musicPlaying = true;
}

void destroyMixer(Mixer* audioSystem) {
    if (audioSystem->musicPlaying) Mix_HaltMusic();

    for (int i = 0; i < audioSystem->soundCount; ++i) {
        if(audioSystem->sounds[i] != NULL) Mix_FreeChunk(audioSystem->sounds[i]);
    }
       
    for (int i = 0; i < audioSystem->musicCount; ++i) {
        if (audioSystem->musics[i] != NULL) Mix_FreeMusic(audioSystem->musics[i]);
    }
    if (audioSystem->musics != NULL) free(audioSystem->musics);
    if (audioSystem->sounds != NULL) free(audioSystem->sounds);
    audioSystem->musics = NULL;
    audioSystem->sounds = NULL;
    if (audioSystem != NULL) free(audioSystem);
    audioSystem = NULL;
    Mix_CloseAudio();
    Mix_Quit();
}

void setVolume(int percent) {
    if (percent <= 0) {
        Mix_VolumeMusic(0);
    }
    else if (percent >= 100){
        Mix_VolumeMusic(MIX_MAX_VOLUME);
    }
    else {
        int volume = (percent  * MIX_MAX_VOLUME) / 100;
        Mix_VolumeMusic(volume);
    }
}

void playRandomMusic(Mixer* audioSystem) {
    if (audioSystem->musicCount == 0) {
        printf("No music loaded to play!\n");
        return;
    }
    int index = randint(0, audioSystem->musicCount - 1);
    playMusic(audioSystem, audioSystem->musics[index], true); // with fade in effect
}

void updateMusic(Mixer* audioSystem) {
    Uint32 currentTime = SDL_GetTicks();

    if (audioSystem->musicPlaying && !Mix_PlayingMusic()) {
        audioSystem->musicPlaying = false;
        lastMusicEndTime = currentTime;
    }

    if (!audioSystem->musicPlaying && (currentTime - lastMusicEndTime) >= MUSIC_WAIT_TIME) {
        playRandomMusic(audioSystem);
    }
}

