#include <SDL/SDL.h>
#include "SDL_gfxPrimitives.h"

#include "constants.h"
#include "functions.h"
#include "globals.h"
#include "gsMenu.h"

gsMenu::gsMenu() {
    Uint32 nclr = SDL_MapRGB(screen->format, 0, 0, 0);
    Uint32 hclr = SDL_MapRGB(screen->format, 0, 150, 0);
    buttons = new cButtonSet(fButton, true, B_NULL, 120, 410, 10, 65, 500, 100, DIR_LEFT, nclr, hclr, screen->format);
    buttons->addB("Play");
    buttons->addB("Help");
    buttons->addB("About");
    buttons->addB("Exit");

    targetState = STATE_NULL;

    sT = loadImage("Media/Graphics/t.png");
    sHeader = loadImage("Media/Graphics/mainheader.png");
    sAbout = loadImage("Media/Graphics/about.png");

    mx = 0;
    my = 0;
    ox = 0;
    oy = 0;

    disp = false;
}

gsMenu::~gsMenu() {
    SDL_FreeSurface(bg);
    SDL_FreeSurface(sT);
    SDL_FreeSurface(sHeader);
    SDL_FreeSurface(sAbout);

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
            } else if (event.key.keysym.sym == SDLK_1) { // Leftovers
            } else if (event.key.keysym.sym == SDLK_2) {
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            mx = event.motion.x;
            my = event.motion.y;
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
        gm->setNextState(targetState);
        break;
    case 2:
        buttons->moveOut();
        targetState = STATE_GAME;
        gm->lvl = LEVEL_TUTORIAL;
        gm->setNextState(targetState);
        break;
    case 3:
        disp = !disp;
        break;
    case 4:
        buttons->moveOut();
        targetState = STATE_EXIT;
        gm->setNextState(targetState);
        break;
    }

    buttons->logic();

    /*
    if (buttons->gState() == B_AWAY && targetState != STATE_NULL) {
        gm->setNextState(targetState);
    }
    */

    return 0;
}

int gsMenu::render(SDL_Surface* dst) {
    fresh(dst, true);

    applySurface(commonBG, dst, 0, 0);

    applySurface(sHeader, dst, 113, 10);

    buttons->render(dst);

    if (disp) applySurface(sAbout, dst, 263, 372);

    /*
    int x, y;
    bool b = lineline(400, 0, 400, 20000000, 0, 1700, 20000000, 1700, &x, &y);
    // bool b = lineline(400, 0, 400, 2000, 0, 400, 2000, 400, &x, &y);

    if (b) {
        SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, 255, 0, 0));
    }

    lineRGBA(dst, 412, 0, 412, 2411, 255, 255, 0, 255);
    lineRGBA(dst, 0, 400, 2000, 400, 255, 255, 0, 255);
    */

    // applyTile(sT, dst, 100, 100, mx-100, my-100, 10, 10); // Tile test

    return 0;
}
