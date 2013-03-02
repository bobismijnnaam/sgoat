#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "gamestate.h"

extern SDL_Surface* screen;
extern SDL_Surface* commonBG;

extern cGameMan* gm;

extern TTF_Font* fIntro;
extern TTF_Font* fButton;
extern TTF_Font* fSmall;
extern TTF_Font* fBig;

#endif
