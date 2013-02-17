#include <SDL/SDL.h>
#include "SDL_gfxPrimitives.h"

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

    return 0;
}

int gsGame::render(SDL_Surface* dst) {
    SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, 0, 150, 0));

    level->render(dst);

    player->render(dst);

    SDL_Flip(dst);

    return 0;
}

