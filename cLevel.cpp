#include <iostream>
#include <SDL/SDL.h>
#include "SDL_gfxPrimitives.h"

#include "cLevel.h"
#include "functions.h"

cLevel::cLevel(SDL_Surface* dst, int t, int r, int b, int l, cPlayer* player) {
    wallclr = SDL_MapRGB(dst->format, 0, 0, 0);
    rayHit.hit = false;
    dir = enumDirDOWN;
    dirOn = false;

    borders[0].x = l; // Top
    borders[0].y = t - 400;
    borders[0].w = r - l;
    borders[0].h = 400;

    borders[1].x = l - 400; // Left? xD
    borders[1].y = t - 400;
    borders[1].w = 400;
    borders[1].h = 400 + (b - t) + 400;

    borders[2].x = r; // Right
    borders[2].y = t - 400;
    borders[2].w = 400;
    borders[2].h = 400 + (b - t) + 400;

    borders[3].x = l;
    borders[3].y = b;
    borders[3].w = r - l;
    borders[3].h = 400;

    playerPOI[0] = player->gPlayerStart();
    playerPOI[1] = player->gPlayerEnd();
    sEntrance = loadImage("Media/Graphics/entrance.png");
    sExit = loadImage("Media/Graphics/exit.png");

    edges[0] = t;
    edges[1] = r;
    edges[2] = b;
    edges[3] = l;

    sWall = loadImage("Media/Graphics/paper.jpg");
}

cLevel::~cLevel() {
    SDL_FreeSurface(sEntrance);
    SDL_FreeSurface(sExit);
    SDL_FreeSurface(sWall);
}

bool cLevel::raycast(int x1, int y1, int x2, int y2, bool checkBorders) { // Returns collision closest to (x1, y1)!
    std::vector<coord> points;
    int t1x, t1y, t2x, t2y, N;
    coord tc;
    bool ret = false;

    // Iterate through all the walls. Check for collision. If there is, add the collision points to the list.
    for (int i = 0; i < walls.size(); ++i) {
        lineRect.set(&walls[i], x1, y1, x2, y2);
        if (lineRect.calc()) {
            ret = true;
            N = lineRect.get(&t1x, &t1y, &t2x, &t2y);
            tc.x = t1x;
            tc.y = t1y;
            points.push_back(tc);

            if (N == 2) {
                tc.x = t2x;
                tc.y = t2y;
                points.push_back(tc);
            }
        }
    }

    // std::cout << points.size() << " | "; // Debugging leftovers

    // Iterate through borders if checkBorders = true
    // If again there are collisions, add the collision points to the list.
    if (checkBorders) {
        for (int i = 0; i < 4; ++i) {
            lineRect.set(&borders[i], x1, y1, x2, y2);
            if (lineRect.calc()) {
                ret = true;
                N = lineRect.get(&t1x, &t1y, &t2x, &t2y);
                tc.x = t1x;
                tc.y = t1y;
                points.push_back(tc);

                if (N == 2) {
                    tc.x = t2x;
                    tc.y = t2y;
                    points.push_back(tc);
                }
            }
        }
    }

    // std::cout << points.size() << "\n"; // More leftovers

    // Find the point closest to (x1, y1) and mark it as the first collision point.
    // This point can later be retrieved with getRayHit()
    int closest = -1, cDist = 9999999999999999;
    int dx, dy, dist2;

    if (ret) {
        for (int i = 0; i < points.size(); ++i) {
            dx = points[i].x - x1;
            dy = points[i].y - y1;
            dist2 = dx * dx + dy * dy;

            if (dist2 < cDist) {
                closest = i;
                cDist = dist2;
            }
        }

        rayHit.x = points[closest].x;
        rayHit.y = points[closest].y;

        return true;
    } else {
        return false;
    }
}

coord cLevel::getRayHit() {
    return rayHit;
}

int cLevel::addWall(int t, int r, int b, int l) {
    SDL_Rect dummy; // Rectangle
    dummy.x = l;
    dummy.y = t;
    dummy.w = r - l;
    dummy.h = b - t;
    walls.push_back(dummy);

    return 0;
}

bool cLevel::slideCol(BOB_Rect* pos, coord* centroid) {
    bool col = false;
    int dt, dr, db, dl;


    for (int i = 0; i < walls.size(); ++i) {
        if (BOBrectCol(*pos, walls[i])) {
            // Pushes players outside walls
            displace(pos, walls[i]);

            col = true;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (BOBrectCol(*pos, borders[i])) {
            // Keeps player within borders
            displace(pos, borders[i]);

            col = true;
        }
    }

    if (col) {// Recalculate centroid
        centroid->x = pos->x + pos->w / 2.0;
        centroid->y = pos->y + pos->h / 2.0;
    }

    return col;
}

enumDir cLevel::displace(BOB_Rect* pos, SDL_Rect wall) {
    // Deze functie duwt het vierkant buiten het andere vierkant
    // En geeft de kant dat ie 'm opduwt

    enumDir direction;

    // Ten opzichte van wall. Dus dt = verschil onderrand speler en bovenrand wall
    float dt = pos->y + pos->h - wall.y;
    float dr = wall.x + wall.w - pos->x;
    float db = wall.y + wall.h - pos->y;
    float dl = pos->x + pos->h - wall.x;

    if (dt <= dr && dt <= db && dt <= dl) {
        direction = enumDirUP;
        pos->y = wall.y - pos->h;
    } else if (dr <= dt && dr <= db && dr <= dl) {
        direction = enumDirRIGHT;
        pos->x = wall.x + wall.w;
    } else if (db <= dt && db <= dr && db <= dl) {
        direction = enumDirDOWN;
        pos->y = wall.y + wall.h;
    } else if (dl <= dt && dl <= dr && dl <= db) {
        direction = enumDirLEFT;
        pos->x = wall.x - pos->w;
    }

    return direction;
}

int cLevel::events(SDL_Event* event) {
    // Events zijn niet van belang?

    return 0;
}

int cLevel::logic(cPlayer* player) {
    vp = player->getViewport();

    return 0;
}

int cLevel::render(SDL_Surface* dst) {
    for (int i = 0; i < walls.size(); ++i) {
        dispWall(dst, &walls[i], &vp);
    }
    for (int i = 0; i < 4; ++i) {
        dispWall(dst, &borders[i], &vp);
    }

    if (dirOn) {
        switch (dir) {
            case enumDirLEFT:
                filledTrigonRGBA(dst, 100, 0, 0, 50, 100, 100, 255, 0, 0, 255);
                break;
            case enumDirRIGHT:
                filledTrigonRGBA(dst, 0, 0, 100, 50, 0, 100, 255, 0, 0, 255);
                break;
            case enumDirUP:
                filledTrigonRGBA(dst, 50, 0, 100, 100, 0, 100, 255, 0, 0, 255);
                break;
            case enumDirDOWN:
                filledTrigonRGBA(dst, 0, 0, 100, 0, 50, 100, 255, 0, 0, 255);
                break;
        }
    }

    coord tc;
    tc = toScreen(playerPOI[0]);
    applySurface(sEntrance, dst, tc.x - sEntrance->w/2, tc.y - sEntrance->h/2);
    tc = toScreen(playerPOI[1]);
    applySurface(sExit, dst, tc.x - sEntrance->w/2, tc.y - sEntrance->h/2);

    return 0;
}

int cLevel::dispWall(SDL_Surface* dst, SDL_Rect* w, SDL_Rect* vp) {
    SDL_Rect dstRect;

    dstRect.w = w->w;
    dstRect.h = w->h;
    dstRect.x = w->x - vp->x;
    dstRect.y = w->y - vp->y;

    SDL_FillRect(dst, &dstRect, wallclr);

    coord wc = {w->x, w->y};
    wc = toScreen(wc);

    applyTile(sWall, dst, wc.x, wc.y, w->w, w->h, 0, 0);

    return 0;
}

coord cLevel::toScreen(coord p) {
    coord np;
    np.x = p.x - vp.x;
    np.y = p.y - vp.y;

    return np;
}

coord cLevel::toWorld(coord p) {
    coord np;
    np.x = p.x + vp.x;
    np.y = p.y + vp.y;

    return np;
}

int cLevel::getEdge(int edge) {
    return edges[edge];
}

SDL_Rect cLevel::getVP() {
    return vp;
}

int placeCoolers(int x, int y, int n, cLevel* level) {
    int wx;
    for (int i = 0; i < n; ++i) {
        wx = x + i * 2 * 75;
        level->addWall(y, wx + 75, y + 10 * 75, wx);
    }

    return 0;
}
