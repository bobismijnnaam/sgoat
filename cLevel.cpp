#include <SDL/SDL.h>

#include "cLevel.h"
#include "functions.h"

cLevel::cLevel(SDL_Surface* dst) {
    wallclr = SDL_MapRGB(dst->format, 0, 0, 0);
    col = false;
}

cLevel::~cLevel() {
    // Niets te deleten
}

coord cLevel::raycast(int x1, int y1, int x2, int y2) { // Komt nog
    coord a;
    return a;
}

int cLevel::addWall(int t, int r, int b, int l) {
    SDL_Rect dummy; // Rectangle
    dummy.x = l;
    dummy.y = t;
    dummy.w = r - l;
    dummy.h = b - t;
    walls.push_back(dummy);

    // Sides converted to lines

    return 0;
}

coord cLevel::slideCol(SDL_Rect* pos, coord* prevPos) { // Komt nog
    coord newPos;
    newPos.hit = false;

    for (int i = 0; i < walls.size(); ++i) {
        col = false;

        if (rectCol(*pos, walls[i]) == true) {
            col = true;
            break;
        }
    }

    return newPos;
}

int cLevel::events(SDL_Event* event) {
    // Events zijn niet van belang?

    return 0;
}

int cLevel::logic(cPlayer* player) {
    vp = player->getViewport();

    return 0;
}

int cLevel::render(SDL_Surface* dst) {
    for (int i = 0; i < walls.size(); ++i) {
        dispWall(dst, &walls[i], &vp);
    }

    if (col) {
        SDL_Rect t;
        t.x = 0;
        t.y = 0;
        t.w = 50;
        t.h = 50;

        SDL_FillRect(dst, &t, SDL_MapRGB(dst->format, 255, 0, 0));
    }

    return 0;
}

int cLevel::dispWall(SDL_Surface* dst, SDL_Rect* w, SDL_Rect* vp) {
    SDL_Rect dstRect;

    dstRect.w = w->w;
    dstRect.h = w->h;
    dstRect.x = w->x - vp->x;
    dstRect.y = w->y - vp->y;

    SDL_FillRect(dst, &dstRect, wallclr);

    return 0;
}
