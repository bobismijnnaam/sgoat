#ifndef CSLEEPERGUARD_H
#define CSLEEPERGUARD_H

#include "cGuard.h"

enum SleeperGuardStates {
    SleeperGuardStates_SLEEP = 0,
    SleeperGuardStates_ALARM
} ;

class cSleeperGuard : public cGuard {
public:
    cSleeperGuard(int X, int Y, int Range, SDL_Surface* guardImg);
    ~cSleeperGuard();

    int logic(cPlayer* player, cLevel* level);
    int render(SDL_Surface* dst, cLevel* level, bool los);
    int pause();
    int resume();

private:
    SleeperGuardStates state;
    int range;
} ;

#endif
