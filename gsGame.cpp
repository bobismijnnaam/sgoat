#include <SDL/SDL.h>
#include "SDL_gfxPrimitives.h"

#include "functions.h"
#include "globals.h"
#include "gsGame.h"

gsGame::gsGame(int flvl) {
    // Initialize game objects
    player = new cPlayer();
    level = new cLevel(screen, -500, 500, 500, -500);

    // Initialize level
    player->viewViewport(true);
    level->addWall(-300, 50, -200, -50); // Bovenste
    level->addWall(-50, 300, 50, 200); // Rechter
    level->addWall(200, 50, 300, -50);
    level->addWall(-50, -200, 50, -300);

    // Initialize mouse variables
    mx = 0;
    my = 0;

    // Initialize ray test variables
    hit.hit = false;
    rayTest = true;
}

gsGame::~gsGame() {
    // Clean up
    delete player;
    delete level;
}

int gsGame::events() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT); // Quit game if escape is pressed/red cross is clicked
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                gm->setNextState(STATE_EXIT);
            } else if (event.key.keysym.sym == SDLK_1) { // Niet weggehaald, misschien voor later
            } else if (event.key.keysym.sym == SDLK_2) {
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            mx = event.motion.x;
            my = event.motion.y;
        }

        // Update events for game objects
        player->events(&event);
        level->events(&event);
    }
    return 0;
}

int gsGame::logic() {
    // Update player
    player->logic(level);

    // Update level
    level->logic(player);

    // Raycasting test
    if (rayTest) {
        coord mouse;
        mouse.x = mx;
        mouse.y = my;
        mouse = level->toWorld(mouse); // Save mouse coordinates and convert to world coordinates
        hit.hit = level->raycast(player->gX(), player->gY(), mouse.x, mouse.y, true); // Cast ray
        if (hit.hit) {
            hit = level->toScreen(level->getRayHit()); // Get collision position
            hit.hit = true; // toScreen loses hit variable :/
        }
    }

    return 0;
}

int gsGame::render(SDL_Surface* dst) {
    // Clear screen
    SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, 128, 128, 128));

    // Render game objects
    level->render(dst);
    player->render(dst);

    // Display raycasting test hitpoint
    if (hit.hit && rayTest) {
        // Line from player to mouse
        lineRGBA(dst, 350, 350, mx, my, 255, 0, 0, 255);
        // Draw place of collision
        cross(dst, hit.x, hit.y, 10, 0, 0, 255, 255);
    }

    return 0;
}

