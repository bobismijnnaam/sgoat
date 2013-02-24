#include <cmath>
#include <SDL/SDL.h>
#include "SDL_gfxPrimitives.h"

#include "constants.h"
#include "cSleeperGuard.h"
#include "functions.h"

cSleeperGuard::cSleeperGuard(int X, int Y, int Range, SDL_Surface* guardImg) {
    range = Range;
    x = X;
    y = Y;

    sGuard = guardImg;
    state = SleeperGuardStates_SLEEP;
}

cSleeperGuard::~cSleeperGuard() {
    // No memory management
}

int cSleeperGuard::logic(cPlayer* player, cLevel* level) {
    //  Check if in range, then if clear LOS, then if speed is over threshhold. If so, SPOTTED!

    int dx = player->gX() - x;
    int dy = player->gY() - y;
    int dist = dx * dx + dy * dy;

    if (paused) return 0;

    switch (state) {
        case SleeperGuardStates_SLEEP:
            look(player, level, false);

            if (dist < range * range) {
                if (!level->raycast(player->gX(), player->gY(), x, y, false)) {
                    if (player->gSpd() > 100) {
                        spotted = true;
                    }
                }
            }

            if (spotted) {
                state = SleeperGuardStates_ALARM;
                angle = atan2(dy, dx);
            }

            break;
        case SleeperGuardStates_ALARM:
            angle = atan2(dy, dx);
            break;
    }

    return 0;
}

int cSleeperGuard::render(SDL_Surface* dst, cLevel* level, bool los) {
    coord tc = {x, y};
    tc = level->toScreen(tc);

    switch (state) {
        case SleeperGuardStates_ALARM:
            drawLOS(dst, level, x, y, angle, GUARD_FOV, 100);

            applySurface(sGuard, dst, tc.x - sGuard->w/2, tc.y - sGuard->h/2);

            break;
        case SleeperGuardStates_SLEEP:
            circleRGBA(dst, tc.x, tc.y, range, 255, 0, 0, 255);
            filledCircleRGBA(dst, tc.x, tc.y, range, 255, 0, 0, 80);

            applySurface(sGuard, dst, tc.x - sGuard->w/2, tc.y - sGuard->h/2);

            break;
    }
    return 0;
}

int cSleeperGuard::pause() {
    paused = true;
    pauseTime = SDL_GetTicks();

    return 0;
}

int cSleeperGuard::resume() {
    paused = false;

    return 0;
}
