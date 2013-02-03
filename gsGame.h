#ifndef GSGAME_H
#define GSGAME_H

#include "gamestate.h"

class gsGame : public cGameState {
public:
    gsGame();
    ~gsGame();

    int events();
    int logic();
    int render(SDL_Surface* dst);

private:

} ;

#endif
