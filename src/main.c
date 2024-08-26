#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../include/Renderer.h"
#include "../include/game.h"

#include <crtdbg.h>



int main(int argc, char* argv[]) {    
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Scene currentScene = MAIN_MENU;
    initSettings();
    initializeGame();
    initMenu();
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