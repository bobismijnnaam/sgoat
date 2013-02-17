#ifndef CLEVEL_H
#define CLEVEL_H

#include <SDL/SDL.h>
#include <vector>

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
    cLevel(SDL_Surface* dst, int t, int r, int b, int l);
    ~cLevel();

    bool raycast(int x1, int y1, int x2, int y2);
    coord getRayHit();

    int addWall(int t, int r, int b, int l);
    bool slideCol(BOB_Rect* pos, coord* centroid);
    enumDir displace(BOB_Rect* pos, SDL_Rect wall);

    int events(SDL_Event* event);
    int logic(cPlayer* player);
    int render(SDL_Surface* dst);

    int dispWall(SDL_Surface* dst, SDL_Rect* w, SDL_Rect* vp); // DIsplay a rectangle based on a viewport rectangle

private:
    std::vector<SDL_Rect> walls;
    SDL_Rect borders[4];

    SDL_Rect vp;
    coord rayHit;

    Uint32 wallclr;

    enumDir dir;

    bool dirOn;
} ;

#endif
