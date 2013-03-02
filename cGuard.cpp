#include <SDL/SDL.h>
#include <cmath>
#include <iostream>
#include "SDL_gfxPrimitives.h"

#include "cLineRectISCT.h"
#include "cGuard.h"
#include "cLevel.h"
#include "constants.h"
#include "functions.h"

int drawLOS(SDL_Surface* dst, cLevel* level, int x, int y, float Ang, float FOV, float Len) {
    // coord l[2];
    coord tc = {x, y};
    // tc = level->toScreen(tc);

    // Draw Line of Sight
    /*
    l.x = x + cos(Ang) * Len;
    l.y = y + sin(Ang) * Len;
    l = level->toScreen(l);
    lineRGBA(dst, tc.x, tc.y, l.x, l.y, 255, 0, 0, 255);
    */

    /*
    // Edge line
    l[0].x = x + cos(Ang + FOV * .5) * Len;
    l[0].y = y + sin(Ang + FOV * .5) * Len;
    l[0] = level->toScreen(l[0]);

    // Other edge
    l[1].x = x + cos(Ang - FOV * .5) * Len;
    l[1].y = y + sin(Ang - FOV * .5) * Len;
    l[1] = level->toScreen(l[1]);

    filledTrigonRGBA(dst, tc.x, tc.y, l[1].x, l[1].y, l[0].x, l[0].y, 255, 0, 0, 80);
    lineRGBA(dst, l[1].x, l[1].y, l[0].x, l[0].y, 255, 0, 0, 255);
    lineRGBA(dst, tc.x, tc.y, l[0].x, l[0].y, 255, 0, 0, 255);
    lineRGBA(dst, tc.x, tc.y, l[1].x, l[1].y, 255, 0, 0, 255);
    */

    int q = 30;

    Sint16 vx[q+1];
    Sint16 vy[q+1];
    coord hits[q];

    int tx, ty;
    float dAng = FOV / (q - 1);
    float sAng = Ang - FOV / 2;

    // Check if line of sight hits viewport
    SDL_Rect vp;
    vp = level->getVP();
    int px, py;
    px = vp.x + 350;
    py = vp.y + 350;

    // If not in LOS, don't draw fov
    if (!pointInRect(x, y, vp) && level->raycast(x, y, px, py, false)) return 0;

    for (int i = 0; i < q; ++i) {
        tx = cos(sAng + dAng * (float)i) * 3000.0 + x;
        ty = sin(sAng + dAng * (float)i) * 3000.0 + y;

        /*
        tc.x = tx;
        tc.y = ty;
        tc = level->toScreen(tc); */
        // circleRGBA(dst, tc.x, tc.y, 10, 255, 0, 0, 255);

        if (!level->raycast(x, y, tx, ty, true)) { // <- RAYCAST WERKT NIET GOED
            vx[i + 1] = tx;
            vy[i + 1] = ty;
        } else {
            hits[i] = level->getRayHit();
            vx[i + 1] = hits[i].x;
            vy[i + 1] = hits[i].y;
        }

        tc.x = vx[i + 1];
        tc.y = vy[i + 1];
        tc = level->toScreen(tc);
        vx[i + 1] = tc.x;
        vy[i + 1] = tc.y;

        // circleRGBA(dst, tc.x, tc.y, 10, 0, 0, 255, 255);
    }

    tc.x = x;
    tc.y = y;
    tc = level->toScreen(tc);
    vx[0] = tc.x;
    vy[0] = tc.y;

    filledPolygonRGBA(dst, vx, vy, q + 1, 255, 0, 0, 80);
    // polygonRGBA(dst, vx, vy, q + 1, 255, 0, 0, 255);

    return 0;
}

    /*
    for (int i = q / 2 * -1, vi = 0; i < q / 2 + 1; ++i, ++vi) {
        tx = cos(Ang + FOV / 2 / (q / 2) * i) * 300 + vx[0];
        ty = sin(Ang + FOV / 2 / (q / 2) * i) * 300 + vy[0];
        tc.x = tx;
        tc.y = ty;
        tc = level->toScreen(tc);
        pixelRGBA(dst, tc.x, tc.y, 0, 0, 255, 255);

        if (!level->raycast(vx[0], vy[0], tx, ty, true)) {
            vx[vi + 1] = tx;
            vy[vi + 1] = ty;
        } else {
            hits[vi] = level->getRayHit();
            vx[vi + 1] = hits[vi].x;
            vy[vi + 1] = hits[vi].y;
        }
    }
    */

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

int cGuard::look(cPlayer* player, cLevel* level, bool watch) {
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

    // Only if guard is in the screen of player
    if (watch && rectCol(t, player->getViewport())) {
        // Check if player is in FOV. If so, check if line of sight is without obstacles
        int dx = player->gX() - x;
        int dy = player->gY() - y;
        float playerAngle = atan2((float)dy, (float)dx);

        float dAng = diffAng(playerAngle, angle);

        if (dAng < GUARD_FOV / 2) {
            spotted = !(level->raycast(x, y, player->gX(), player->gY(), false)); // Als er GEEN objecten tussenstaan geeft raycast FALSE, dus is de speler zichtbaar dus ziet de guard hem
        } else {
            spotted = false;
        }
    }

    return 0;
}

bool cGuard::isSpotted() {
    return spotted;
}

/*
// To keep the angle in domain [0, 2pi] because of atan2
if (playerAngle < 0) {
    playerAngle += 2.0 * pi;
}

float diffCClock = fabs(playerAngle - angle); // Verschil tegen de klok in.
float diffClock = Bmin(playerAngle, angle) + (2.0 * pi - Bmax(playerAngle, angle)); // Verschil met de klok mee

// Degrees away from looking angle
float dAng = Bmin(diffCClock, diffClock);
*/

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
