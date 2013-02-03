#ifndef GSHELP_H
#define GSHELP_H

#include "gamestate.h"

class gsHelp : public cGameState {
public:
    gsHelp();
    ~gsHelp();

    int events();
    int logic();
    int render(SDL_Surface* dst);

private:

} ;

#endif
