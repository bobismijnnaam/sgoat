#ifndef GSMENU_H
#define GSMENU_H

#include <SDL/SDL.h>

#include "gamestate.h"
#include "buttons.h"

class gsMenu : public cGameState {
public:
    gsMenu();
    ~gsMenu();

    int events();
    int logic();
    int render(SDL_Surface* dst);

private:
    SDL_Surface* bg;
    SDL_Surface* sT;
    SDL_Surface* sHeader;
    SDL_Surface* sAbout;

    int mx, my;
    int ox, oy;
    bool disp;

    cButtonSet* buttons;

    GameStates targetState;

} ;

#endif
