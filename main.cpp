// OPERATION GET OUT, OP GO

#include "mtwist.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"
#include "globals.h"

int init() {
    mt_seed();

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    TTF_Init();

    fIntro = TTF_OpenFont("Media/Fonts/PressStart2P.ttf", 15);
    fButton = TTF_OpenFont("Media/Fonts/PressStart2P.ttf", 30);
    fSmall = TTF_OpenFont("Media/Fonts/PressStart2P.ttf", 10);
    fBig = TTF_OpenFont("Media/Fonts/PressStart2P.ttf", 30);

    screen = SDL_SetVideoMode(SCR_W, SCR_H, SCR_BPP, SDL_SWSURFACE);

    SDL_WM_SetCaption("Operation Get Out | Knights of the Compiler", NULL);

    gm = new cGameMan(STATE_INTRO);

    // console hack
    // freopen( "CON", "wt", stdout );
    // freopen( "CON", "wt", stderr );

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
    TTF_CloseFont(fButton);
    TTF_CloseFont(fSmall);
    TTF_CloseFont(fBig);

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
