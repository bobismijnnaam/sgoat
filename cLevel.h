#ifndef CLEVEL_H
#define CLEVEL_H

#include <SDL/SDL.h>
#include <vector>

#include "cPlayer.h"

struct coord {
    int x;
    int y;
    bool hit;
} ;

class cLevel {
public:
    cLevel(SDL_Surface* dst);
    ~cLevel();

    coord raycast(int x1, int y1, int x2, int y2);
    int addWall(int t, int r, int b, int l);
    coord slideCol(SDL_Rect* pos, coord* prevPos);

    int events(SDL_Event* event);
    int logic(cPlayer* player);
    int render(SDL_Surface* dst);

    int dispWall(SDL_Surface* dst, SDL_Rect* w, SDL_Rect* vp); // DIsplay a rectangle based on a viewport rectangle

private:
    std::vector<SDL_Rect> walls;
    SDL_Rect vp;
    bool col;

    Uint32 wallclr;

} ;

#endif
