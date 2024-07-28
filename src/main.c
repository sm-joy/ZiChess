#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../include/Renderer.h"
#include "../include/core.h"



int main(int argc, char* argv[]) {    

    Scene currentScene = MAIN_GAME;
    initialize_game();

    while (true) {
        if (currentScene == MAIN_MENU) {
            currentScene = runMainMenu();
        }
        else if (currentScene == MAIN_GAME) {
            currentScene = run_game();
        }
        else if (currentScene = QUIT) {
            break;
        }
    }

    clean();
    return 0;
}