#include <cmath>
#include "SDL_gfxPrimitives.h"

#include "constants.h"
#include "cPointGuard.h"
#include "functions.h"

cPointGuard::cPointGuard(int X, int Y, float RotSpd, SDL_Surface* Img) {
    x = X;
    y = Y;
    rotSpd = RotSpd;

    sGuard = Img;

    lastUpdate = SDL_GetTicks();
}

cPointGuard::~cPointGuard() {

}

int cPointGuard::logic(cPlayer* player, cLevel* level) {
    int thisUpdate = SDL_GetTicks();
    float dt = (thisUpdate - lastUpdate) / 1000.0;

    angle += rotSpd * dt;

    if (angle > 2 * pi) {
        angle -= 2 * pi;
    }

    if (angle < 0) {
        angle += 2 * pi;
    }

    look(player, level);

    lastUpdate = thisUpdate;

    return 0;
}

int cPointGuard::render(SDL_Surface* dst, cLevel* level, bool los) {
    coord tc;
    tc.x = x;
    tc.y = y;

    tc = level->toScreen(tc);

    if (!spotted) applySurface(sGuard, dst, tc.x - sGuard->w / 2, tc.y - sGuard->h / 2);

    coord l;
    float length = 200.0;
    if (los) {
        // Draw Line of Sight
        l.x = x + cos(angle) * length;
        l.y = y + sin(angle) * length;
        l = level->toScreen(l);
        lineRGBA(dst, tc.x, tc.y, l.x, l.y, 255, 0, 0, 255);

        // Edge line
        l.x = x + cos(angle + GUARD_FOV / 2.0) * length;
        l.y = y + sin(angle + GUARD_FOV / 2.0) * length;
        l = level->toScreen(l);
        lineRGBA(dst, tc.x, tc.y, l.x, l.y, 255, 0, 0, 255);

        // Other edge
        l.x = x + cos(angle - GUARD_FOV / 2.0) * length;
        l.y = y + sin(angle - GUARD_FOV / 2.0) * length;
        l = level->toScreen(l);
        lineRGBA(dst, tc.x, tc.y, l.x, l.y, 255, 0, 0, 255);
    }

    return 0;
}
