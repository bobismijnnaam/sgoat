#include <cmath>
#include <iostream>
#include "SDL_gfxPrimitives.h"

#include "constants.h"
#include "cPointGuard.h"
#include "functions.h"

cPointGuard::cPointGuard(int X, int Y, float StartAng, float EndAng, int RotTime, bool Clockwise, SDL_Surface* Img) {
    x = X;
    y = Y;
    rotTime = RotTime;
    startAng = StartAng;
    endAng = EndAng;
    clockwise = Clockwise;

    sGuard = Img;
    angle = startAng;
    animTime = SDL_GetTicks();
    state = PointGuardStates_FORTH;

    lastUpdate = SDL_GetTicks();
    paused = false;
}

cPointGuard::~cPointGuard() {

}

int cPointGuard::logic(cPlayer* player, cLevel* level) {
    int thisUpdate = SDL_GetTicks();
    float diff;
    float dtAng;

    if (paused) return 0;

    // Change state after rottime has elapsed
    if (thisUpdate - animTime > rotTime) {
        if (state == PointGuardStates_BACK) {
            state = PointGuardStates_FORTH;
        } else if (state == PointGuardStates_FORTH) {
            state = PointGuardStates_BACK;
        } else {
            state = PointGuardStates_ALARM;
        }

        animTime = thisUpdate;
        dtAng = 0;
    } else {
        dtAng = (thisUpdate - animTime) / (float) rotTime;
        dtAng = tweenInOut(dtAng);
    }

    switch (state) {
        case PointGuardStates_ALARM:
            angle = atan2(player->gY() - y, player->gX() - x);
            break;
        case PointGuardStates_BACK:
            angle = lerpAng(startAng, endAng, 1 - dtAng, clockwise);
            break;
        case PointGuardStates_FORTH: // From start to end
            angle = lerpAng(startAng, endAng, dtAng, clockwise);
            break;
    }

    // Convert angle to [0, 2pi] domain
    while (angle > 2 * pi) {
        angle -= 2 * pi;
    }
    while (angle < 0) {
        angle += 2 * pi;
    }

    look(player, level, true);
    if (spotted) state = PointGuardStates_ALARM;

    lastUpdate = thisUpdate;

    return 0;
}

int cPointGuard::render(SDL_Surface* dst, cLevel* level, bool los) {
    coord tc;
    tc.x = x;
    tc.y = y;

    tc = level->toScreen(tc);

    if (los) {
        drawLOS(dst, level, x, y, angle, GUARD_FOV, 200);
    }

    applySurface(sGuard, dst, tc.x - sGuard->w / 2, tc.y - sGuard->h / 2);

    return 0;
}

int cPointGuard::pause() {
    paused = true;
    pauseTime = SDL_GetTicks();

    return 0;
}

int cPointGuard::resume() {
    paused = false;
    animTime = SDL_GetTicks() - (pauseTime - animTime);

    return 0;
}
