#include <SDL/SDL.h>

#include "globals.h"
#include "gsGame.h"

gsGame::gsGame(int flvl) {
    player = new cPlayer();

    player->viewViewport(true);
}

gsGame::~gsGame() {
    delete player;
}

int gsGame::events() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                gm->setNextState(STATE_EXIT);
            }
        }

        player->events(&event);
    }
    return 0;
}

int gsGame::logic() {
    player->logic();

    return 0;
}

int gsGame::render(SDL_Surface* dst) {
    SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, 0, 150, 0));

    player->render(dst);

    SDL_Flip(dst);

    return 0;
}

