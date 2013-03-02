#ifndef CLEVEL_H
#define CLEVEL_H

#include <SDL/SDL.h>
#include <vector>

#include "cLineRectISCT.h"
#include "cPlayer.h"

enum enumDir {
    enumDirLEFT = 0,
    enumDirRIGHT,
    enumDirUP,
    enumDirDOWN
} ;

struct coord {
    float x;
    float y;
    bool hit;
} ;

struct BOB_Rect {
    float x;
    float y;
    float w;
    float h;
} ;

class cLevel {
public:
    cLevel(SDL_Surface* dst, int t, int r, int b, int l, cPlayer* player);
    ~cLevel();

    int addWall(int t, int r, int b, int l);
    int dispWall(SDL_Surface* dst, SDL_Rect* w, SDL_Rect* vp); // DIsplay a rectangle based on a viewport rectangle

    bool slideCol(BOB_Rect* pos, coord* centroid);
    enumDir displace(BOB_Rect* pos, SDL_Rect wall);
    bool raycast(int x1, int y1, int x2, int y2, bool checkBorders);
    coord getRayHit();

    coord toScreen(coord p);
    coord toWorld(coord p);

    int events(SDL_Event* event);
    int logic(cPlayer* player);
    int render(SDL_Surface* dst);

    int getEdge(int edge);
    SDL_Rect getVP();

private:
    std::vector<SDL_Rect> walls;
    SDL_Rect borders[4];
    SDL_Surface* sWall;

    coord playerPOI[2];
    SDL_Surface* sEntrance;
    SDL_Surface* sExit;

    SDL_Rect vp;
    coord rayHit;

    Uint32 wallclr;

    enumDir dir;

    cLineRectISCT lineRect;

    int edges[4];

    bool dirOn;
} ;

int placeCoolers (int x, int y, int n, cLevel* level);

#endif
