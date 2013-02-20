#ifndef CPOINTGUARD_H
#define CPOINTGUARD_H

#include "cGuard.h"

class cPointGuard : public cGuard {
public:
    cPointGuard(int X, int Y, float RotSpd, SDL_Surface* Img);
    ~cPointGuard();

    int logic(cPlayer* player, cLevel* level);
    int render(SDL_Surface* dst, cLevel* level, bool los);

private:
    float rotSpd;
} ;

#endif
