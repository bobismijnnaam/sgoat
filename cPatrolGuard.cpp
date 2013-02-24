#include <cmath>
#include <SDL/SDL.h>

#include "cPatrolGuard.h"
#include "cLevel.h"
#include "constants.h"
#include "cPlayer.h"
#include "functions.h"

cPatrolGuard::cPatrolGuard(int X, int Y, int RotTime, int Speed, std::vector<coord>& Waypoints, SDL_Surface* guardImg, bool Loop) {
    state = PatrolGuardStates_WALKING;

    coord c = {X, Y};
    waypoints.push_back(c);

    for (int i = 0; i < Waypoints.size(); ++i) {
        waypoints.push_back(Waypoints[i]);
    }
    Waypoints.clear();

    sGuard = guardImg;
    x = X;
    y = Y;
    speed = Speed;
    rotTime = RotTime;

    targetWaypoint = 1;
    prevWaypoint = 0;
    int dx = waypoints[1].x - waypoints[0].x;
    int dy = waypoints[1].y - waypoints[0].y;
    walkDuration = sqrt(dx * dx + dy * dy) / (float)speed * 1000;
    loop = Loop;

    lastUpdate = SDL_GetTicks();
    stateTime = lastUpdate;
    paused = false;
}

cPatrolGuard::~cPatrolGuard() {
    // Geen dynamisch toegewezen geheugen
}

int cPatrolGuard::logic(cPlayer* player, cLevel* level) {
    int thisUpdate = SDL_GetTicks();
    float dt;
    int dx, dy;
    int d, nextTarget;
    int previousTarget;

    if (paused) return 0;

    switch (state) {
        case PatrolGuardStates_WALKING:
            if (thisUpdate - stateTime > walkDuration) {
                // Bepaal start/end angles
                prevAng = angle;

                if (targetWaypoint == 0) { // Als de doelwaypoint het beginpunt is
                    nextTarget = 1;
                } else if (targetWaypoint == waypoints.size() - 1) { // Als het doelwaypoint het einde is
                    if (loop) { // ALs de guard een rodnje moet lopen maak dan volgend target het beginpunt
                        nextTarget = 0;
                    } else { // ZO niet, ga dan gewoon weer terug
                        nextTarget = targetWaypoint - 1;
                    }
                } else { // Hier vervolgd de guard gewoon zijn patrouille in de richting die ie liep
                    nextTarget = targetWaypoint + (targetWaypoint - prevWaypoint);
                }

                // Bereken nieuwe hoek
                dx = waypoints[nextTarget].x - waypoints[targetWaypoint].x;
                dy = waypoints[nextTarget].y - waypoints[targetWaypoint].y;
                targetAng = atan2(dy, dx);

                // Zet nieuwe staat en tijd
                stateTime = thisUpdate;
                // Switch naar TURNING
                state = PatrolGuardStates_TURNING;
            } else {
                // Niks aan de hand. Interpoleren tussen twee punten
                dt = (thisUpdate - stateTime) / (float)walkDuration;
                dt = tweenInOut(dt);

                if (waypoints[targetWaypoint].x == waypoints[prevWaypoint].x) {
                    x = waypoints[prevWaypoint].x;
                } else {
                    x = waypoints[targetWaypoint].x * dt + waypoints[prevWaypoint].x * (1 - dt);
                }
                if (waypoints[targetWaypoint].y == waypoints[prevWaypoint].y) {
                    y = waypoints[prevWaypoint].y;
                } else {
                    y = waypoints[targetWaypoint].y * dt + waypoints[prevWaypoint].y * (1 - dt);
                }

                dx = waypoints[targetWaypoint].x - waypoints[prevWaypoint].x;
                dy = waypoints[targetWaypoint].y - waypoints[prevWaypoint].y;
                angle = atan2(dy, dx);
            }
            break;
        case PatrolGuardStates_TURNING:
            if (thisUpdate - stateTime > rotTime) {
                // Head to the next waypoint!
                if (targetWaypoint == 0) { // Als de doelwaypoint het beginpunt is
                    prevWaypoint = 0;
                    targetWaypoint = 1;
                } else if (targetWaypoint == waypoints.size() - 1) {  // Als het doelwaypoint het einde is
                    if (loop) { // ALs de guard een rodnje moet lopen maak dan volgend target het beginpunt
                        prevWaypoint = targetWaypoint;
                        targetWaypoint = 0;
                    } else { // Anders gewoon de weg weer teruglopen
                        targetWaypoint = waypoints.size() - 2;
                        prevWaypoint = targetWaypoint + 1;
                    }
                } else { // Gewoon verder gaan in de richting die ie al liep
                    d = targetWaypoint - prevWaypoint;
                    prevWaypoint = targetWaypoint;
                    targetWaypoint += d;
                }

                // Nieuwe punten vastleggen en berekenen hoelang de guard erover zal doen om de snelheid te kunnen bepalen
                x = waypoints[prevWaypoint].x;
                y = waypoints[prevWaypoint].y;

                dx = waypoints[targetWaypoint].x - waypoints[prevWaypoint].x;
                dy = waypoints[targetWaypoint].y - waypoints[prevWaypoint].y;
                walkDuration = sqrt(dx * dx + dy * dy) / (float)speed * 1000;

                // Nieuwe staat & tijd
                state = PatrolGuardStates_WALKING;
                stateTime = SDL_GetTicks();
            } else {
                // Interpolate between angles
                dt = (thisUpdate - stateTime) / (float)rotTime;
                dt = tweenInOut(dt);
                angle = toAng(prevAng, targetAng, dt);
            }
            break;
        case PatrolGuardStates_ALARM:
            dx = player->gX() - x;
            dy = player->gY() - y;
            angle = atan2(dy, dx);
            break;
    }

    look(player, level, true);
    if (spotted) {
        state = PatrolGuardStates_ALARM;
    }

    lastUpdate = thisUpdate;

    return 0;
}

int cPatrolGuard::render(SDL_Surface* dst, cLevel* level, bool los) {
    coord tc = {x, y};
    tc = level->toScreen(tc);

    applySurface(sGuard, dst, tc.x - sGuard->w/2, tc.y -  sGuard->h/2);

    if (los) drawLOS(dst, level, x, y, angle, GUARD_FOV, 100);

    return 0;
}

int cPatrolGuard::pause() {
    paused = true;
    pauseTime = SDL_GetTicks();

    return 0;
}

int cPatrolGuard::resume() {
    paused = false;
    stateTime = SDL_GetTicks() - (pauseTime - stateTime);
    return 0;
}
