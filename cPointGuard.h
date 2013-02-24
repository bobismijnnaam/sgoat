#ifndef CPOINTGUARD_H
#define CPOINTGUARD_H

#include "cGuard.h"

enum PointGuardStates {
    PointGuardStates_FORTH = 0,
    PointGuardStates_BACK,
    PointGuardStates_ALARM
} ;

class cPointGuard : public cGuard {
public:
    cPointGuard(int X, int Y, float StartAng, float EndAng, int RotTime, bool Clockwise, SDL_Surface* Img);
    ~cPointGuard();

    int logic(cPlayer* player, cLevel* level);
    int render(SDL_Surface* dst, cLevel* level, bool los);
    int pause();
    int resume();

private:
    int rotTime;
    float startAng, endAng;
    bool clockwise;

    PointGuardStates state;
    int animTime;
} ;

#endif
