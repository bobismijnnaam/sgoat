#include <SDL/SDL.h>

#include "globals.h"
#include "gsHelp.h"

gsHelp::gsHelp() {

}

gsHelp::~gsHelp() {

}

int gsHelp::events() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                gm->setNextState(STATE_EXIT);
            } else if (event.key.keysym.sym == SDLK_1) {
                gm->setNextState(STATE_INTRO);
            } else if (event.key.keysym.sym == SDLK_2) {
                gm->setNextState(STATE_MENU);
            }
        }
    }

    return 0;
}

int gsHelp::logic() {

    return 0;
}

int gsHelp::render(SDL_Surface* dst) {

    return 0;
}
