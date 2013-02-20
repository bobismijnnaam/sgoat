#ifndef CGUARD_H
#define CGUARD_H

#include <SDL/SDL.h>

#include "cLevel.h"
#include "cPlayer.h"

class cGuard {
public:
    cGuard();
    ~cGuard();

    int events();
    virtual int logic(cPlayer* player, cLevel* level) = 0;
    virtual int render(SDL_Surface* dst, cLevel* level, bool los) = 0;
    int look(cPlayer* player, cLevel* level);

    bool isSpotted();

protected:
    SDL_Surface* sGuard;
    int x, y;
    float angle;
    int lastUpdate;
    bool spotted;
} ;

#endif
