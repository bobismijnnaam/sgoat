

#include "functions.h"
#include "gsIntro.h"
#include "globals.h"

gsIntro::gsIntro() {
    sdl = loadImage("Media/Graphics/sdl.gif");
    ogam = loadImage("Media/Graphics/1gam.png");

    logo = new cKnightsLogo(false, true, fIntro);

    introTime = SDL_GetTicks();
}

gsIntro::~gsIntro() {
    SDL_FreeSurface(sdl);

    delete logo;
}

int gsIntro::events() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
            gm->setNextState(STATE_MENU);
        }
    }

    return 0;
}

int gsIntro::logic() {
    logo->logic();

    if (SDL_GetTicks() - introTime >= 5000) {
        gm->setNextState(STATE_MENU);
    }

    return 0;
}

int gsIntro::render(SDL_Surface* dst) {
    fresh(dst, true);

    applySurface(commonBG, dst, 0, 0);

    logo->render(dst, 230, 70);

    applySurface(sdl, dst, 130, 450);
    applySurface(ogam, dst, 430, 420);

    return 0;
}
