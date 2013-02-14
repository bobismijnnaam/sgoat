#include <SDL/SDL.h>

#include "functions.h"
#include "globals.h"
#include "gsMenu.h"

gsMenu::gsMenu() {
    Uint32 nclr = SDL_MapRGB(screen->format, 255, 255, 255);
    Uint32 hclr = SDL_MapRGB(screen->format, 0, 150, 0);
    buttons = new cButtonSet(fButton, true, B_NULL, 100, 100, 10, 50, 500, 100, DIR_LEFT, nclr, hclr, screen->format);
    buttons->addB("Play");
    buttons->addB("Help");
    buttons->addB("About");
    buttons->addB("Exit");

    targetState = STATE_NULL;
}

gsMenu::~gsMenu() {
    SDL_FreeSurface(bg);

    delete buttons;
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

        buttons->handleEvents(&event);
    }

    return 0;
}

int gsMenu::logic() {
    switch (buttons->gReleased()) {
    case 1:
        buttons->moveOut();
        targetState = STATE_LVLSELECT;
        break;
    case 2:
        buttons->moveOut();
        targetState = STATE_HELP;
        break;
    case 3:
        // Iets met about;
        break;
    case 4:
        buttons->moveOut();
        targetState = STATE_EXIT;
        break;
    }

    buttons->logic();

    if (buttons->gState() == B_AWAY && targetState != STATE_NULL) {
        gm->setNextState(targetState);
    }

    return 0;
}

int gsMenu::render(SDL_Surface* dst) {
    fresh(dst, true);

    buttons->render(dst);

    return 0;
}
