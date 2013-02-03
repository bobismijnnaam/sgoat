#ifndef GSLVLSELECT_H
#define GSLVLSELECT_H

#include "gamestate.h"

class gsLvlSelect : public cGameState {
public:
    gsLvlSelect();
    ~gsLvlSelect();

    int events();
    int logic();
    int render(SDL_Surface* dst);

private:

} ;

#endif
