#ifndef GSGAME_H
#define GSGAME_H

#include "gamestate.h"
#include "cLevel.h"
#include "cLineRectISCT.h"
#include "cPlayer.h"
#include "cAI.h"

enum InGameStates {
    InGameStates_GAME = 0,
    InGameStates_PAUSED,
    InGameStates_WON,
    InGameStates_LOSS
} ;

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

    InGameStates state;

    cPlayer* player;
    cLevel* level;
    cAI* ai;
    SDL_Surface* sBG;

    cButtonSet* pauseButtons;
    cButtonSet* endButtons;

    SDL_Surface* sPaused;
    SDL_Surface* sLost;
    SDL_Surface* sWon;

    int lossWinTime;

} ;

#endif
