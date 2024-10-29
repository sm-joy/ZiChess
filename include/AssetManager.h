#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SDL.h>
#include <stdint.h>
#include <SDL_mixer.h>
#include "Graphics.h"
#include <stdbool.h>

#define INITIAL_CAPACITY 4

typedef struct Asset_SDLTexture{
	SDL_Texture* tex;
	uint32_t hash;
} Texture;

typedef struct {
	Texture* textures;
	size_t numTexture;
	size_t capTexture;
} TextureList;


typedef struct Asset_SDLMixMusic {
	Mix_Music* music;
	uint32_t hash;
} Music;

typedef struct {
	Music* musics;
	size_t numMusic;
	size_t capMusic;
} MusicList;

typedef struct Asset_SDLMixChunk {
	Mix_Chunk* sound;
	uint32_t hash;
} Sound;

typedef struct {
	Sound* sounds;
	size_t numSound;
	size_t capSound;
} SoundList;

typedef struct {
	TextureList texList;
	MusicList musicList;
	SoundList soundList;
} AssetManager;

void CreateAssetManager(AssetManager* am);
void DestroyAssetManager(AssetManager* am);

SDL_Texture* LoadTexture(AssetManager* am, RenderContext* rc, const char* filePath, const char* key);
Mix_Music* LoadMusic(AssetManager* am, const char* filePath, const char* key);
Mix_Chunk* LoadSound(AssetManager* am, const char* filePath, const char* key);

SDL_Texture* GetTexture(AssetManager* am, const char* key);
Mix_Music* GetMusic(AssetManager* am, const char* key);
Mix_Chunk* GetSound(AssetManager* am, const char* key);

void DestroyTexture(AssetManager* am, const char* key);
void DestroyMusic(AssetManager* am, const char* key);
void DestroySound(AssetManager* am, const char* key);

#endif // ASSET_MANAGER_H
