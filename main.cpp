#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"
#include "globals.h"

int init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    TTF_Init();

    fIntro = TTF_OpenFont("Media/Fonts/PressStart2P.ttf", 15);

    screen = SDL_SetVideoMode(SCR_W, SCR_H, SCR_BPP, SDL_SWSURFACE);

    SDL_WM_SetCaption("Scapegoat | Knights of the Compiler", NULL);

    gm = new cGameMan(STATE_INTRO);

    return 0;
}

int game() {
    while (gm->getState() != STATE_EXIT) {
        gm->events();

        gm->logic();

        gm->changeState();

        gm->render();

        SDL_Delay(1);
    }

    return 0;
}

int quit() {
    TTF_CloseFont(fIntro);

    TTF_Quit();

    SDL_Quit();

    return 0;
}

int main(int argc, char *argv[]) {
    init();

    game();

    quit();

    return 0;
}
