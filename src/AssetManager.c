#include "../include/AssetManager.h"
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../include/Utils.h"
#include <SDL_mixer.h>

static uint32_t Hash(const char* key) {  // FNV1AHASH
    const uint32_t offset_basis = 0x811C9DC5;
    const uint32_t FNV_prime = 0x01000193;
    uint32_t hash = offset_basis;

    while (*key) {
        hash ^= (uint32_t)(*key++);
        hash *= FNV_prime;
    }

    return hash;
}

void CreateAssetManager(AssetManager* am) {
    if (!am) {
        LogNullError("Asset Manager", NULL);
        return;
    }

    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf initialization failed: %s\n", TTF_GetError());
        IMG_Quit();
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Mix_OpenAudio Error: %s\n", Mix_GetError());
        IMG_Quit();
        TTF_Quit();
        return;
    }


    am->texList.capTexture = INITIAL_CAPACITY;
    am->musicList.capMusic = INITIAL_CAPACITY;
    am->soundList.capSound = INITIAL_CAPACITY;

    am->texList.numTexture = 0;
    am->musicList.numMusic = 0;
    am->soundList.numSound = 0;

    am->texList.textures = (Texture*)malloc(INITIAL_CAPACITY * sizeof(Texture));
    am->musicList.musics = (Music*)malloc(INITIAL_CAPACITY * sizeof(Music));
    am->soundList.sounds = (Sound*)malloc(INITIAL_CAPACITY * sizeof(Sound));

    if (!am->texList.textures || !am->musicList.musics || !am->soundList.sounds) {
        fprintf(stderr, "Failed to alloc memory for assets!\n");
        if (am->texList.textures) free(am->texList.textures);
        if (am->musicList.musics) free(am->musicList.musics);
        if (am->soundList.sounds) free(am->soundList.sounds);
        TTF_Quit();
        IMG_Quit();
        return;
    }
}


void DestroyAssetManager(AssetManager* am) {
    if (!am) {
        LogNullError("Asset Manager", NULL);
        return;
    }
    for (size_t i = 0; i < am->texList.numTexture; ++i) {
        if (!am->texList.textures[i].tex) {
            SDL_DestroyTexture(am->texList.textures[i].tex);
            am->texList.textures[i].tex = NULL;
        }
    }
    free(am->texList.textures);
    am->texList.textures = NULL;
    am->texList.numTexture = 0;
    am->texList.capTexture = 0;

    for (size_t i = 0; i < am->musicList.numMusic; ++i) {
        if (!am->musicList.musics[i].music) {
            Mix_FreeMusic(am->musicList.musics[i].music);
            am->musicList.musics[i].music = NULL;
        }
    }
    free(am->musicList.musics);
    am->musicList.musics = NULL;
    am->musicList.numMusic = 0;
    am->musicList.capMusic = 0;

    for (size_t i = 0; i < am->soundList.numSound; ++i) {
        if (!am->soundList.sounds[i].sound) {
            Mix_FreeChunk(am->soundList.sounds[i].sound);
            am->soundList.sounds[i].sound = NULL;
        }
    }
    free(am->soundList.sounds);
    am->soundList.sounds = NULL;
    am->soundList.numSound = 0;
    am->soundList.capSound = 0;

    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
}


void DestroyTexture(AssetManager* am, const char* key) {
    if (!am || !key) return;
    uint32_t hash = Hash(key);
    for (size_t i = 0; i < am->texList.numTexture; ++i) {
        if (am->texList.textures[i].hash == hash) {
            SDL_DestroyTexture(am->texList.textures[i].tex);
            am->texList.textures[i].tex = NULL;    
            return;                         
        }
    }
}


void DestroyMusic(AssetManager* am, const char* key) {
    if (!am || !key) return;
    uint32_t hash = Hash(key);
    for (size_t i = 0; i < am->musicList.numMusic; ++i) {
        if (am->musicList.musics[i].hash == hash) {
            Mix_FreeMusic(am->musicList.musics[i].music);
            am->musicList.musics[i].music = NULL;
            return;
        }
    }
}


void DestroySound(AssetManager* am, const char* key) {
    if (!am || !key) return;
    uint32_t hash = Hash(key);
    for (size_t i = 0; i < am->soundList.numSound; ++i) {
        if (am->soundList.sounds[i].hash == hash) {
            Mix_FreeChunk(am->soundList.sounds[i].sound);
            am->soundList.sounds[i].sound = NULL;
            return;
        }
    }
}


static SDL_Texture* GetTextureH(AssetManager* am, uint32_t hash) {
    for (size_t i = 0; i < am->texList.numTexture; i++) {
        if (am->texList.textures[i].hash == hash) {
            return am->texList.textures[i].tex;
        }
    }
    return NULL; // NOT FOUND
}

SDL_Texture* GetTexture(AssetManager* am, const char* key) {
    uint32_t hash = Hash(key);

    for (size_t i = 0; i < am->texList.numTexture; i++) {
        if (am->texList.textures[i].hash == hash) {
            return am->texList.textures[i].tex;
        }
    }

    fprintf(stderr, "Texture with key '%s' not found!\n", key);
    return NULL;
}


SDL_Texture* LoadTexture(AssetManager* am, RenderContext* rc, const char* filePath, const char* key) {
    if (!am || !filePath || !key) {
        if (!am) LogNullError("Asset Manager", NULL);
        if (!filePath) LogNullError("File Path", NULL);
        if (!key) LogNullError("Key", NULL);
        return NULL;
    }
    uint32_t hash = Hash(key);

    SDL_Texture* tex = NULL;
    tex = GetTextureH(am, hash);
    if (tex) return tex;


    tex = IMG_LoadTexture(rc->renderer, filePath);
    if (!tex) {
        fprintf(stderr, "Error Loading '%s' Texture! Error: %s\n", filePath, SDL_GetError());
        return NULL;
    }

    if (am->texList.numTexture >= am->texList.capTexture) {
        am->texList.capTexture *= 2;
        Texture* newptr = (Texture*)realloc(am->texList.textures, am->texList.capTexture * sizeof(Texture));
        if (!newptr) {
            fprintf(stderr, "Realloc Failed while creating texture list\n");
            SDL_DestroyTexture(tex);
            am->texList.capTexture /= 2;
            return NULL;
        }
        am->texList.textures = newptr;
    }

    am->texList.textures[am->texList.numTexture].tex = tex;
    am->texList.textures[am->texList.numTexture++].hash = hash;
    return tex;
}

static Mix_Music* GetMusicH(AssetManager* am, uint32_t hash) {
    for (size_t i = 0; i < am->musicList.numMusic; i++) {
        if (am->musicList.musics[i].hash == hash) {
            return am->musicList.musics[i].music;
        }
    }
    return NULL; // NOT FOUND
}

Mix_Music* GetMusic(AssetManager* am, const char* key) {
    uint32_t hash = Hash(key);

    for (size_t i = 0; i < am->musicList.numMusic; i++) {
        if (am->musicList.musics[i].hash == hash) {
            return am->musicList.musics[i].music;
        }
    }

    fprintf(stderr, "Music with key '%s' not found!\n", key);
    return NULL;
}


Mix_Music* LoadMusic(AssetManager* am, const char* filePath, const char* key) {
    if (!am || !filePath || !key) {
        if (!am) LogNullError("Asset Manager", NULL);
        if (!filePath) LogNullError("File Path", NULL);
        if (!key) LogNullError("Key", NULL);
        return NULL;
    }
    uint32_t hash = Hash(key);

    Mix_Music* music = NULL;
    music = GetMusicH(am, hash);
    if (music) return music;


    music = Mix_LoadMUS(filePath);
    if (!music) {
        fprintf(stderr, "Error Loading '%s' Music! Error: %s\n", filePath, SDL_GetError());
        return NULL;
    }

    if (am->musicList.numMusic >= am->musicList.capMusic) {
        am->musicList.capMusic *= 2;
        Music* newptr = (Music*)realloc(am->musicList.musics, am->musicList.capMusic * sizeof(Music));
        if (!newptr) {
            fprintf(stderr, "Realloc Failed while creating Music list\n");
            Mix_FreeMusic(music);
            am->musicList.capMusic /= 2;
            return NULL;
        }
        am->musicList.musics = newptr;
    }

    am->musicList.musics[am->musicList.numMusic].music = music;
    am->musicList.musics[am->musicList.numMusic++].hash = hash;
    return music;
}


static Mix_Chunk* GetSoundH(AssetManager* am, uint32_t hash) {
    for (size_t i = 0; i < am->soundList.numSound; i++) {
        if (am->soundList.sounds[i].hash == hash) {
            return am->soundList.sounds[i].sound;
        }
    }
    return NULL; // NOT FOUND
}

Mix_Chunk* GetSound(AssetManager* am, const char* key) {
    uint32_t hash = Hash(key);

    for (size_t i = 0; i < am->soundList.numSound; i++) {
        if (am->soundList.sounds[i].hash == hash) {
            return am->soundList.sounds[i].sound;
        }
    }

    fprintf(stderr, "Music with key '%s' not found!\n", key);
    return NULL;
}

Mix_Chunk* LoadSound(AssetManager* am, const char* filePath, const char* key) {
    if (!am || !filePath || !key) {
        if (!am) LogNullError("Asset Manager", NULL);
        if (!filePath) LogNullError("File Path", NULL);
        if (!key) LogNullError("Key", NULL);
        return NULL;
    }

    uint32_t hash = Hash(key);

    Mix_Chunk* sound = NULL;
    sound = GetSoundH(am, hash);
    if (sound) return sound;


    sound = Mix_LoadWAV(filePath);
    if (!sound) {
        fprintf(stderr, "Error Loading '%s' Sound! Error: %s\n", filePath, SDL_GetError());
        return NULL;
    }

    if (am->soundList.numSound >= am->soundList.capSound) {
        am->soundList.capSound *= 2;
        Sound* newptr = (Sound*)realloc(am->soundList.sounds, am->soundList.capSound * sizeof(Sound));
        if (!newptr) {
            fprintf(stderr, "Realloc Failed while creating Sound list\n");
            Mix_FreeChunk(sound);
            am->soundList.capSound /= 2;
            return NULL;
        }
        am->soundList.sounds = newptr;
    }

    am->soundList.sounds[am->soundList.numSound].sound = sound;
    am->soundList.sounds[am->soundList.numSound++].hash = hash;
    return sound;
}