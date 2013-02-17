#include <SDL/SDL.h>
#include "SDL_gfxPrimitives.h"

#include "functions.h"
#include "globals.h"
#include "gsGame.h"

gsGame::gsGame(int flvl) {
    player = new cPlayer();
    level = new cLevel(screen, -500, 500, 500, -500);

    player->viewViewport(true);
    level->addWall(-300, 50, -200, -50); // Bovenste
    level->addWall(-50, 300, 50, 200); // Rechter
    level->addWall(200, 50, 300, -50);
    level->addWall(-50, -200, 50, -300);

    mx = 0;
    my = 0;

    rect = new SDL_Rect;
    rect->x = 100;
    rect->y = 100;
    rect->w = 50;
    rect->h = 100;
}

gsGame::~gsGame() {
    delete player;
    delete level;
}

int gsGame::events() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                gm->setNextState(STATE_EXIT);
            } else if (event.key.keysym.sym == SDLK_1) {
                // flat = !flat;
            } else if (event.key.keysym.sym == SDLK_2) {
                // displaced = !displaced;
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            mx = event.motion.x;
            my = event.motion.y;
        }

        player->events(&event);

        level->events(&event);
    }
    return 0;
}

int gsGame::logic() {
    player->logic(level);

    level->logic(player);

    intersector.setLine(350, 350, mx, my);
    intersector.setRect(rect);

    if (intersector.calc()) {
        clr = 255; // 5-HT
    } else {
        clr = 0;
    }

    N = intersector.get(&x1, &y1, &x2, &y2);

    if (x1 == x2 && y1 == y2) gm->setNextState(STATE_EXIT);

    return 0;
}

int gsGame::render(SDL_Surface* dst) {
    SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, 128, 128, 128));

    level->render(dst);

    player->render(dst);

    lineRGBA(dst, 350, 350, mx, my, 255-clr, clr, 0, 255);

    rectangleRGBA(dst, rect->x, rect->y, rect->x + rect->w, rect->y + rect->h, 255-clr, clr, 0, 255);

    if (N > 0) {
        // cross(dst, x1, y1, 10, 0, 0, 255, 255);
        cross(dst, x1, y1, 10, 0, 255, 255, 255);

        if (N == 2) {
            cross(dst, x2, y2, 10, 0, 0, 255, 255);
        }
    }

    SDL_Flip(dst);

    return 0;
}

