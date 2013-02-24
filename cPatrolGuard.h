#ifndef CPATROLGUARD_H
#define CPATROLGUARD_H

#include <vector>

#include "cGuard.h"
#include "cLevel.h"
#include "cPlayer.h"

enum PatrolGuardStates {
    PatrolGuardStates_WALKING = 0,
    PatrolGuardStates_TURNING,
    PatrolGuardStates_ALARM
} ;

class cPatrolGuard : public cGuard {
public:
    cPatrolGuard(int X, int Y, int RotTime, int Speed, std::vector<coord>& Waypoints, SDL_Surface* guardImg, bool Loop);
    ~cPatrolGuard();

    int logic(cPlayer* player, cLevel* level);
    int render(SDL_Surface* dst, cLevel* level, bool los);
    int pause();
    int resume();

private:
    PatrolGuardStates state;
    int stateTime;

    std::vector<coord> waypoints;
    int targetWaypoint, prevWaypoint;
    int walkDuration;
    int speed;
    int rotTime;
    bool loop;

    float prevAng, targetAng;

} ;

#endif
