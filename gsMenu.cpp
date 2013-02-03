#include <SDL/SDL.h>

#include "functions.h"
#include "globals.h"
#include "gsMenu.h"

gsMenu::gsMenu() {
    bg = loadImage("Media/Graphics/bg2.png");
}

gsMenu::~gsMenu() {
    SDL_FreeSurface(bg);
}

int gsMenu::events() {
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

int gsMenu::logic() {

    return 0;
}

int gsMenu::render(SDL_Surface* dst) {
    applySurface(bg, dst, 0, 0);

    return 0;
}
