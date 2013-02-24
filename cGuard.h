#ifndef CGUARD_H
#define CGUARD_H

#include <SDL/SDL.h>

#include "cLevel.h"
#include "cPlayer.h"

int drawLOS(SDL_Surface* dst, cLevel* level, int x, int y, float Ang, float FOV, float Len);

class cGuard {
public:
    cGuard();
    ~cGuard();

    int events();
    virtual int logic(cPlayer* player, cLevel* level) = 0;
    virtual int render(SDL_Surface* dst, cLevel* level, bool los) = 0;
    virtual int pause() = 0;
    virtual int resume() = 0;

    int look(cPlayer* player, cLevel* level, bool watch);
    bool isSpotted();

protected:
    SDL_Surface* sGuard;
    int x, y;
    float angle;
    int lastUpdate;
    bool spotted;
    bool paused;
    int pauseTime;
} ;

#endif
