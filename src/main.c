#include "../include/Core.h"
#include <crtdbg.h>



int main(int argc, char* argv[]) {
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Core core;
    if (!CreateCore(&core)) return -1;
    CoreLoop(&core);
    CoreDestroy(&core);
    return 0;
}