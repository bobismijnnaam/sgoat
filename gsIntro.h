#ifndef GSINTRO_H
#define GSINTRO_H

#include <SDL/SDL.h>

#include "gamestate.h"
#include "knightslogo.h"

class gsIntro : public cGameState {
public:
    gsIntro();
    ~gsIntro();

    int events();
    int logic();
    int render(SDL_Surface* dst);

private:
    SDL_Surface* sdl;
    SDL_Surface* ogam;

    cKnightsLogo* logo;

    int introTime;

} ;

#endif
