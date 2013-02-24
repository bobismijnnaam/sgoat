#ifndef CAI_H
#define CAI_H

#include <vector>

#include "cGuard.h"
#include "cLevel.h"
#include "cPlayer.h"

class cAI {
public:
    cAI();
    ~cAI();

    int events(SDL_Event* event);
    int logic(cPlayer* player, cLevel* level);
    int render(SDL_Surface* dst, cLevel* level);

    int addWaypoint(int x, int y);
    int addPatrolGuard(int x, int y, int movSpd, float rotTime, bool loop);

    int addPointGuard(int x, int y, float StartAng, float EndAng, int rotTime, bool clockwise);

    int addSleeper(int x, int y, int range);

    bool isSpotted();

    int pause();
    int resume();

private:
    std::vector<cGuard*> guards;

    std::vector<coord> waypoints;

    bool spotted;

    SDL_Surface* sGuardSleeper;
    SDL_Surface* sGuardPoint;
    SDL_Surface* sGuardPatrol;

} ;

#endif
