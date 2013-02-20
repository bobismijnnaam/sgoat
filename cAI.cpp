

#include "cAI.h"
#include "cPointGuard.h"
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

int cAI::addPatrolGuard(int movSpd, float rotSpd) {

    return 0;
}

int cAI::addPointGuard(int x, int y, float rotSpd) {
    guards.push_back(new cPointGuard(x, y, rotSpd, sGuardPoint));

    return 0;
}

int cAI::addSleeper(int x, int y) {

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
