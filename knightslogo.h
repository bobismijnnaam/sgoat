#ifndef KNIGHTSLOGO_H
#define KNIGHTSLOGO_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include <string>

class cKnightsLetter {
public:
    cKnightsLetter(cKnightsLetter* prnt, TTF_Font* font, int fX, int fY, bool bgBlack);
    ~cKnightsLetter();

    int logic(bool step);
    int render(SDL_Surface* dst, int cX, int cY);

    int gBit();
    bool gActive();

private:
    TTF_Font* f;
    SDL_Surface* sBit;
    cKnightsLetter* parent;
    int bit;
    int x, y;
    SDL_Color clr;
    bool active;
} ;

class cKnightsLogo {
public:
    cKnightsLogo(bool bgBlack, bool fTitle, TTF_Font* fF);
    ~cKnightsLogo();

    int logic();
    int render(SDL_Surface* dst, int fX, int fY);

private:
    TTF_Font* f;
    int lastStep;
    std::vector< std::vector<cKnightsLetter*> > bits;
    SDL_Surface* sTitle;
    bool title;
} ;

const int KL_DELAY = 90;

int applySurfaceKL(SDL_Surface* src, SDL_Surface* dst, int x, int y);

#endif
