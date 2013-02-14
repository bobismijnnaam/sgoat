#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <list>

const int B_SCR_W = 750;
const int B_SCR_H = 470;

enum eBAction {
    B_MOUT = 0, // Moving out
    B_MIN, // Moving in
    B_AWAY, // Not on screen
    B_NULL // Standard position
} ;

enum eBState {
    B_MDOWN = 0,
    B_MUP,
    B_MHOVER,
    B_MCLICK,
    B_MNULL
} ;

enum fourDir {
    DIR_UP = 0,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} ;

int applySurfaceButton(SDL_Surface* src, SDL_Surface* dst, int x, int y);

class cButton {
public:
    cButton(TTF_Font* font, std::string fCaption, int fX, int fY, eBAction startAction, fourDir fDir, int fMoveTime, Uint32 fNClr, Uint32 fHClr, SDL_PixelFormat* fmt);
    ~cButton();

    int handleEvents(SDL_Event* event);

    int moveOut();
    int moveIn();
    int on();
    int off();
    bool onOff();

    int logic();
    int render(SDL_Surface* dst);

    eBAction gAction();
    eBState gState();

private:
    TTF_Font* f;
    std::string caption;
    SDL_Color nClr;
    SDL_Color hClr;

    int x, y;
    int r, b;

    fourDir dir;
    int moveTime;
    bool active;

    eBAction action;
    int actionStart;

    eBState state;
    bool stepped;

    SDL_Surface* sNull;
    SDL_Surface* sHover;
};

class cButtonSet {
public:
    cButtonSet(TTF_Font* font, bool vert, eBAction fStartAction, int fSX, int fSY, int fDX, int fDY, int fMT, int fDT, fourDir fDir, Uint32 fNClr, Uint32 fHClr, SDL_PixelFormat* fFmt);
    ~cButtonSet();

    int handleEvents(SDL_Event* event);

    int moveOut();
    int moveIn();
    int on(int p, bool all);
    int off(int p, bool all);
    bool onOff(int p);

    int logic();
    int render(SDL_Surface* dst);

    int gClicked();
    int gPressed();
    int gReleased();

    eBAction gState();

    int addB(std::string capt);
private:
    std::list<cButton*> buttons;

    eBAction startAction;

    TTF_Font* f;
    bool vertical;

    int sX, sY;
    int dX, dY;
    int mT, dT;

    fourDir dir;

    Uint32 nClr, hClr;
    SDL_PixelFormat* fmt;
};

#endif
