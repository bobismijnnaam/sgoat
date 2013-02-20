#include <SDL/SDL.h>
#include <cmath>
#include <iostream>

#include "cGuard.h"
#include "cLevel.h"
#include "constants.h"
#include "functions.h"

cGuard::cGuard() {
    // sGuard = loadImage("Media/Graphics/guard.png");
    spotted = false;
}

cGuard::~cGuard() {
    // SDL_FreeSurface(sGuard);
}

int cGuard::events() {
    // Events hebben geen invloed
    return 0;
}

int cGuard::look(cPlayer* player, cLevel* level) {
    // First check if the player touches the guard
    SDL_Rect t;
    t.x = x - sGuard->w / 2;
    t.y = y - sGuard->h / 2;
    t.w = sGuard->w;
    t.h = sGuard->h;

    if (rectCol(t, player->playerRect())) {
        spotted = true;
        return 1;
    }

    // Check if player is in FOV. If so, check if line of sight is without obstacles
    int dx = player->gX() - x;
    int dy = player->gY() - y;
    float playerAngle = atan2((float)dy, (float)dx);

    // To keep the angle in domain [0, 2pi] because of atan2
    if (playerAngle < 0) {
        playerAngle += 2.0 * pi;
    }

    float diffCClock = fabs(playerAngle - angle); // Verschil tegen de klok in.
    float diffClock = Bmin(playerAngle, angle) + (2.0 * pi - Bmax(playerAngle, angle)); // Verschil met de klok mee

    // Degrees away from looking angle
    float dAng = Bmin(diffCClock, diffClock);

    if (dAng < GUARD_FOV / 2) {
        spotted = !(level->raycast(x, y, player->gX(), player->gY(), false)); // Als er GEEN objecten tussenstaan geeft raycast FALSE, dus is de speler zichtbaar dus ziet de guard hem
    } else {
        spotted = false;
    }

    return 0;
}

bool cGuard::isSpotted() {
    return spotted;
}

/*

int cGuard::render(SDL_Surface* dst, cLevel* level, bool los) { // los = Line Of Sight
    coord tc;
    tc.x = x;
    tc.y = y;

    tc = level->toScreen(tc);

    applySurface(sGuard, dst, tc.x - sGuard->w / 2, tc.y - sGuard->h / 2);

    if (los) {
        // Draw Line of Sight
    }

    return 0;
}

*/
