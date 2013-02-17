#ifndef GSGAME_H
#define GSGAME_H

#include "gamestate.h"
#include "cLevel.h"
#include "cLineRectISCT.h"
#include "cPlayer.h"

class gsGame : public cGameState {
public:
    gsGame(int flvl);
    ~gsGame();

    int events();
    int logic();
    int render(SDL_Surface* dst);

private:
    int lvl;

    int mx, my;

    int clr;
    int N;
    int x1, y1, x2, y2;
    SDL_Rect* rect;

    cLineRectISCT intersector;

    cPlayer* player;
    cLevel* level;

} ;

#endif
