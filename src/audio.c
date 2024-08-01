#include "../include/audio.h"

static void addSound(Mixer* audioSystem, Mix_Chunk* sound) {
    if (audioSystem->soundCount >= audioSystem->soundCapacity) {
        audioSystem->soundCapacity *= 2;
        Mix_Chunk** newptr = realloc(audioSystem->sounds, audioSystem->soundCapacity * sizeof(Mix_Chunk*));
        if (newptr == NULL) {
            printf("Realloc Error: Unable to resize sounds array\n");
            return;
        }
        audioSystem->sounds = newptr;
    }
    audioSystem->sounds[audioSystem->soundCount++] = sound;
}

Mixer* createMixer() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
    }

    Mixer* audioSystem = (Mixer*)malloc(sizeof(Mixer));
    if (!audioSystem) {
        printf("Malloc Error: Unable to allocate AudioSystem\n");
        return NULL;
    }
    audioSystem->soundCount = 0;
    audioSystem->soundCapacity = 4;
    audioSystem->sounds = malloc(audioSystem->soundCapacity * sizeof(Mix_Chunk*));
    if (!audioSystem->sounds) {
        printf("Malloc Error: Unable to allocate sounds array\n");
        return NULL;
    }
    return audioSystem;
}

Mix_Chunk* loadSound(Mixer* audioSystem, const char* file) {
    Mix_Chunk* sound = Mix_LoadWAV(file);
    if (!sound) {
        printf("Mix_LoadWAV Error: %s\n", Mix_GetError());
    }
    else {
        addSound(audioSystem, sound);
    }
    return sound;
}

void playSound(Mix_Chunk* sound) {
    if (Mix_PlayChannel(-1, sound, 0) == -1) {
        printf("Mix_PlayChannel Error: %s\n", Mix_GetError());
    }
}

void destroyMixer(Mixer* audioSystem) {
    for (int i = 0; i < audioSystem->soundCount; ++i) {
        Mix_FreeChunk(audioSystem->sounds[i]);
    }
    free(audioSystem->sounds);
    Mix_CloseAudio();
    Mix_Quit();
}