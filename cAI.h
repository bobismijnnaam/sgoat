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
    int addPatrolGuard(int movSpd, float rotSpd);

    int addPointGuard(int x, int y, float rotSpd);

    int addSleeper(int x, int y);

    bool isSpotted();

private:
    std::vector<cGuard*> guards;

    std::vector<coord> waypoints;

    bool spotted;

    SDL_Surface* sGuardSleeper;
    SDL_Surface* sGuardPoint;
    SDL_Surface* sGuardPatrol;

} ;

#endif
