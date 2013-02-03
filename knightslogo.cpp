#include "mtwist.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <sstream>

#include "knightslogo.h"

cKnightsLetter::cKnightsLetter(cKnightsLetter* prnt, TTF_Font* font, int fX, int fY, bool bgBlack) {
    // Gather parameters
    parent = prnt;
    x = fX;
    y = fY;
    f = font;

    // Check if it should be a row starter or not
    if (fX == 0) {
        active = true;
    } else {
        active = false;
    }

    // Decide color of letter
    int clrInd = 1; // Color indicator. Red = 0; grey = 1; black/white = 2

    if (y == 1 && x > 5 && x < 10) { // Red
        clrInd = 0;
    } else if (y == 2 && x > 4 && x < 11) {
        clrInd = 0;
    } else if (y == 3 && x > 3 && x < 10) {
        clrInd = 0;
    } else if (y == 4 && x > 3 && x < 7) {
        clrInd = 0;
    } else if (y == 5 && x > 2 && x < 6) {
        clrInd = 0;
    } else if (y == 6 && x > 3 && x < 6) {
        clrInd = 0;
    } else if (y == 7 && x > 3 && x < 6) {
        clrInd = 0;
    } else if (y == 8 && x > 2 && x < 5) {
        clrInd = 0;
    } else if (y == 9 && x == 4) {
        clrInd = 0;
    }

    if (y == 4 && x > 7 && x < 11) {
        clrInd = 2;
    } else if (y == 5 && x > 6 && x < 13) {
        clrInd = 2;
    } else if (y == 6 && x > 5 && x < 13) {
        clrInd = 2;
    } else if (y == 7 && x > 5 && x < 14) {
        clrInd = 2;
    } else if (y == 8 && x > 6 && x < 14) {
        clrInd = 2;
    } else if (y == 9 && x > 6 && x < 13) {
        clrInd = 2;
    } else if (y == 10 && x > 5 && x < 12) {
        clrInd = 2;
    } else if (y == 11 && x > 5 && x < 11) {
        clrInd = 2;
    } else if (y == 12 && ((x > 4 && x < 7) || (x > 8 && x < 12))) {
        clrInd = 2;
    } else if (y == 13 && x > 10 && x < 13) {
        clrInd = 2;
    } else if (y == 14 && x == 12) {
        clrInd = 2;
    }

    if (clrInd == 0) {
        clr = {255, 0, 0};
    } else if (clrInd == 1) {
        clr = {128, 128, 128};
    } else if (clrInd == 2) {
        if (bgBlack) {clr = {255, 255, 255};} else {clr = {0, 0, 0};}
    }

    // Generate letter & surface
    bit = mt_lrand() % 2;
    std::stringstream ss;
    ss << bit;
    sBit = TTF_RenderText_Blended(f, ss.str().c_str(), clr);
}

cKnightsLetter::~cKnightsLetter() {
    SDL_FreeSurface(sBit);
}

int cKnightsLetter::logic(bool step) {
    if (step) {
        SDL_FreeSurface(sBit);

        if (parent != NULL) {
            bit = parent->gBit();
            active = parent->gActive();
        } else {
            bit = mt_lrand() % 2;
        }

        std::stringstream ss;
        ss << bit;
        sBit = TTF_RenderText_Blended(f, ss.str().c_str(), clr);
    }

    return 0;
}

int cKnightsLetter::render(SDL_Surface* dst, int cX, int cY) {
    if (active) {applySurfaceKL(sBit, dst, cX + sBit->h * x, cY + sBit->h * y);}
    return 0;
}

int cKnightsLetter::gBit() {
    return bit;
}

bool cKnightsLetter::gActive() {
    return active;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

cKnightsLogo::cKnightsLogo(bool bgBlack, bool fTitle, TTF_Font* fF) {
    f = fF;

    title = fTitle;

    bits.resize(17);

    for (int y = 0; y < 16; y++) {
        bits[0].push_back(new cKnightsLetter(NULL, f, 0, y, bgBlack));
    }

    for (int x = 1; x < 17; x++) {
        for (int y = 0; y < 16; y++) {
            bits[x].push_back(new cKnightsLetter(bits[x-1][y], f, x, y, bgBlack));
        }
    }

    SDL_Color c = {128, 128, 128};
    sTitle = TTF_RenderText_Blended(f, "A Knights of the Compiler game", c);

    lastStep = SDL_GetTicks();
}

cKnightsLogo::~cKnightsLogo() {
    for (int x = 0; x < 17; x++) {
        for (int y = 0; y < 16; y++) {
            delete bits[x][y];
        }
    }

    TTF_CloseFont(f);
    SDL_FreeSurface(sTitle);
}

int cKnightsLogo::logic() {
    bool trigger = false;

    if (SDL_GetTicks() - lastStep > KL_DELAY) {
        trigger = true;
        lastStep = SDL_GetTicks();
    }

    for (int y = 0; y < 16; y++) {
        for (int x = 16; x > -1; x--) {
            bits[x][y]->logic(trigger);
        }
    }

    return 0;
}

int cKnightsLogo::render(SDL_Surface* dst, int fX, int fY) {
    for (int x = 0; x < 17; x++) {
        for (int y = 0; y < 16; y++) {
            bits[x][y]->render(dst, fX, fY);
        }
    }

    int x = (fX + 17/2 * sTitle->h) - sTitle->w / 2;
    int y = fY + 17 * sTitle->h;
    if (title) {
        applySurfaceKL(sTitle, dst, x, y);
    }

    return 0;
}

int applySurfaceKL(SDL_Surface* src, SDL_Surface* dst, int x, int y) {
    SDL_Rect tRect;
    int t = 0;

    tRect.x = x;
    tRect.y = y;

    t = SDL_BlitSurface(src, NULL, dst, &tRect);

    return t;
}

