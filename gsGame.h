#ifndef GSGAME_H
#define GSGAME_H

#include "gamestate.h"
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

    cPlayer* player;

} ;

#endif
