#ifndef GSLVLSELECT_H
#define GSLVLSELECT_H

#include "gamestate.h"
#include "buttons.h"

class gsLvlSelect : public cGameState {
public:
    gsLvlSelect();
    ~gsLvlSelect();

    int events();
    int logic();
    int render(SDL_Surface* dst);

private:
    cButtonSet* buttons;
    cButtonSet* bBack;

    SDL_Surface* sHeader;

    GameStates targetState;

} ;

#endif
