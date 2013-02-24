#include <SDL/SDL.h>

#include "constants.h"
#include "functions.h"
#include "globals.h"
#include "gsLvlSelect.h"

gsLvlSelect::gsLvlSelect() {
    Uint32 nclr = SDL_MapRGB(screen->format, 255, 255, 255);
    Uint32 hclr = SDL_MapRGB(screen->format, 0, 150, 0);
    buttons = new cButtonSet(fButton, true, B_NULL, 100, 100, 0, 50, 500, 100, DIR_LEFT, nclr, hclr, screen->format);
    bBack = new cButtonSet(fButton, true, B_NULL, 100, 500, 0, 50, 500, 100, DIR_LEFT, nclr, hclr, screen->format);

    buttons->addB("Ouverture");
    buttons->addB("Anacrusis");
    buttons->addB("Crescendo");
    buttons->addB("Ledger Lines");

    bBack->addB("Back");

    targetState = STATE_NULL;
}

gsLvlSelect::~gsLvlSelect() {
    delete buttons;
}

int gsLvlSelect::events() {
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

        buttons->handleEvents(&event);
        bBack->handleEvents(&event);
    }

    return 0;
}

int gsLvlSelect::logic() {
    if (buttons->gReleased() == 1) {
        gm->setNextState(STATE_GAME);
        gm->lvl = LEVEL_OUVERTURE;
    }

    int chosenLvl = buttons->gReleased();
    if (chosenLvl > 0) {
        gm->setNextState(STATE_GAME);
        gm->lvl = chosenLvl;
    }

    if (bBack->gReleased() == 1) {
        gm->setNextState(STATE_MENU);
    }

    buttons->logic();

    bBack->logic();

    return 0;
}

int gsLvlSelect::render(SDL_Surface* dst) {
    fresh(dst, true);

    buttons->render(dst);

    bBack->render(dst);

    return 0;
}

