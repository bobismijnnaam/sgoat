// Includes I didn't make myself
#include <SDL/SDL.h>

// Includes I did make myself
#include "constants.h"
#include "functions.h"
#include "gamestate.h"
#include "globals.h"

#include "gsIntro.h"
#include "gsMenu.h"
#include "gsHelp.h"
#include "gsLvlSelect.h"
#include "gsGame.h"

cGameMan::cGameMan(GameStates startState) {
    switch (startState) {
    case STATE_INTRO:
        this->currState = new gsIntro();
        break;
    case STATE_MENU:
        this->currState = new gsMenu();
        break;
    case STATE_HELP:
        this->currState = new gsHelp();
        break;
    case STATE_LVLSELECT:
        this->currState = new gsLvlSelect();
        break;
    case STATE_GAME:
        this->currState = new gsGame(lvl);
        break;
    case STATE_EXIT:
        this->currState = new gsIntro();
        break;
    default:
        this->currState = new gsIntro();
        break;
    }

    this->stateID = startState;
    this->nextState = STATE_NULL;

    lvl = 1;
    snd = true;

    state = STATE_RUNNING;
    sScroll = createSurf(SCR_W * 2, SCR_H * 2, screen);
    tSurf = createSurf(SCR_W, SCR_H, screen);
    fresh(sScroll, true);
}

cGameMan::~cGameMan() {
    delete this->currState;
    SDL_FreeSurface(sScroll);
    SDL_FreeSurface(tSurf);
}

int cGameMan::setNextState(GameStates targetState) {
    if (this->nextState != STATE_EXIT) {
        this->nextState = targetState;
    }

    return 0;
}

int cGameMan::changeState() {
    if (this->nextState != STATE_NULL) {
        if (this->nextState != STATE_EXIT) {
            fresh(sScroll, true);
            fresh(tSurf, true);

            currState->render(sScroll);

            switch (this->nextState) {
            case STATE_INTRO:
                this->currState = new gsIntro();
                break;
            case STATE_MENU:
                this->currState = new gsMenu();
                break;
            case STATE_HELP:
                this->currState = new gsHelp();
                break;
            case STATE_LVLSELECT:
                this->currState = new gsLvlSelect();
                break;
            case STATE_GAME:
                this->currState = new gsGame(lvl);
                break;
            case STATE_EXIT:
                this->currState = new gsIntro();
                break;
            default:
                this->currState = new gsIntro();
                break;
            }

            this->stateID = this->nextState;
            this->nextState = STATE_NULL;

            currState->render(tSurf);
            applySurface(tSurf, sScroll, SCR_W, 0);
            state = STATE_TRANSITION;
            stateTime = SDL_GetTicks();


        } else {
            this->stateID = STATE_EXIT;
        }
    }

    return 0;
}

GameStates cGameMan::getState() {
    return this->stateID;
}

int cGameMan::events() {
    if (state == STATE_RUNNING) {
        currState->events();
    } else if (state == STATE_TRANSITION) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gm->setNextState(STATE_EXIT);
            }
        }
    }

    return 0;
}

int cGameMan::logic() {
    if (state == STATE_RUNNING) {
        currState->logic();
    } else if (state == STATE_TRANSITION) {
        if (SDL_GetTicks() - stateTime >= SCROLLTIME) {
            state = STATE_RUNNING;
        }
    }

    return 0;
}

int cGameMan::render() {
    if (state == STATE_RUNNING) {
        currState->render(screen);
    } else if (state == STATE_TRANSITION) {
        SDL_Rect t;

        float d = (SDL_GetTicks() - stateTime) / (float)SCROLLTIME;

        t.w = SCR_W;
        t.h = SCR_H;
        t.y = 0;
        t.x = d * SCR_W;

        applyClipped(sScroll, screen, 0, 0, &t);
    }

    SDL_Flip(screen);

    return 0;
}
