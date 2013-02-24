

#include "cAI.h"
#include "cPatrolGuard.h"
#include "cPointGuard.h"
#include "cSleeperGuard.h"
#include "functions.h"

cAI::cAI() {
    sGuardSleeper = loadImage("Media/Graphics/guardSleeper.png");
    sGuardPoint = loadImage("Media/Graphics/guardPoint.png");
    sGuardPatrol = loadImage("Media/Graphics/guardPatrol.png");
}

cAI::~cAI() {
    SDL_FreeSurface(sGuardSleeper);
    SDL_FreeSurface(sGuardPoint);
    SDL_FreeSurface(sGuardPatrol);
}

int cAI::events(SDL_Event* event) {
    // Nothing to see here, move on

    return 0;
}

int cAI::logic(cPlayer* player, cLevel* level) {
    for (int i = 0; i < guards.size(); ++i) {
        guards[i]->logic(player, level);
    }

    return 0;
}

int cAI::render(SDL_Surface* dst, cLevel* level) {
    for (int i = 0; i < guards.size(); ++i) {
        guards[i]->render(dst, level, true);
    }

    return 0;
}

int cAI::addWaypoint(int x, int y) {
    coord tc;
    tc.x = x;
    tc.y = y;

    waypoints.push_back(tc);

    return 0;
}
// int X, int Y, int RotTime, int Speed, std::vector<coord>& Waypoints, SDL_Surface* guardImg
int cAI::addPatrolGuard(int x, int y, int movSpd, float rotTime, bool loop) {
    guards.push_back(new cPatrolGuard(x, y, rotTime, movSpd, waypoints, sGuardPatrol, loop));
    return 0;
}

int cAI::addPointGuard(int x, int y, float StartAng, float EndAng, int rotTime, bool clockwise) {
    guards.push_back(new cPointGuard(x, y, StartAng, EndAng, rotTime, clockwise, sGuardPoint));

    return 0;
}

int cAI::addSleeper(int x, int y, int range) {
    guards.push_back(new cSleeperGuard(x, y, range, sGuardSleeper));

    return 0;
}

bool cAI::isSpotted() {
    for (int i = 0; i < guards.size(); ++i) {
        if (guards[i]->isSpotted()) {
            return true;
        }
    }

    return false;
}

int cAI::pause() {
    for (int i = 0; i < guards.size(); ++i) {
        guards[i]->pause();
    }

    return 0;
}

int cAI::resume() {
    for (int i = 0; i < guards.size(); ++i) {
        guards[i]->resume();
    }

    return 0;
}
