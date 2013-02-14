#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>

#include "buttons.h"

// mousedown = pressed
// mouse continously down = mclick

// gPressed is only in mousedown
// gClicked is constantly after mousedown

int applySurfaceButton(SDL_Surface* src, SDL_Surface* dst, int x, int y) {
    SDL_Rect tRect;
    int t = 0;

    tRect.x = x;
    tRect.y = y;

    t = SDL_BlitSurface(src, NULL, dst, &tRect);

    return t;
}

cButton::cButton(TTF_Font* font, std::string fCaption, int fX, int fY, eBAction startAction, fourDir fDir, int fMoveTime, Uint32 fNClr, Uint32 fHClr, SDL_PixelFormat* fmt) {
    f = font;
    x = fX;
    y = fY;
    caption = fCaption;
    action = startAction;
    actionStart = -1;

    if (action == B_NULL) {
        active = true;
    } else {
        active = false;
    }

    dir = fDir;
    moveTime = fMoveTime;

    Uint8 r, g, b;
    SDL_GetRGB(fHClr, fmt, &r, &g, &b);
    hClr = {r, g, b};

    SDL_GetRGB(fNClr, fmt, &r, &g, &b);
    nClr = {r, g, b};

    sNull = TTF_RenderText_Blended(f, caption.c_str(), nClr);
    sHover = TTF_RenderText_Blended(f, caption.c_str(), hClr);

    r = x + sNull->w;
    b = y + sNull->h;

    state = B_MNULL;
}

cButton::~cButton() {
    SDL_FreeSurface(sNull);
    SDL_FreeSurface(sHover);
}

int cButton::handleEvents(SDL_Event* event) {
    int mX, mY;

    if (event->type == SDL_MOUSEMOTION) {
        if (!(state == B_MDOWN || state == B_MCLICK)) {
            mX = event->motion.x;
            mY = event->motion.y;

            if (mX > x && mY > y && mX < x + sNull->w && mY < y + sNull->h && active) {
                state = B_MHOVER;
            } else {
                state = B_MNULL;
            }
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            mX = event->button.x;
            mY = event->button.y;

            if (mX > x && mY > y && mX < x + sNull->w && mY < y + sNull->h && active) {
                state = B_MDOWN;
            } else {
                state = B_MNULL;
            }
        }
    }

    if (event->type == SDL_MOUSEBUTTONUP) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            mX = event->button.x;
            mY = event->button.y;

            if (mX > x && mY > y && mX < x + sNull->w && mY < y + sNull->h && active) {
                state = B_MUP;
            } else {
                state = B_MNULL;
            }
        }
    }

    return 0;
}

int cButton::moveOut() {
    action = B_MOUT;
    actionStart = SDL_GetTicks();
    active = false;

    state = B_MNULL;
    return 0;
}

int cButton::moveIn() {
    action = B_MIN;
    actionStart = SDL_GetTicks();
    active = false;

    state = B_MNULL;
    return 0;
}

int cButton::on() {
    active = true;
    return 0;
}

int cButton::off() {
    active = false;
    return 0;
}

bool cButton::onOff() {
    return active;
}

int cButton::logic() {
    if (actionStart == -1) {
        actionStart = SDL_GetTicks();
    }

    // Make sure the mousedown event happens ONCE, after that it's just a keyressed (B_MCLICK)
    int mX, mY;
    SDL_GetMouseState(&mX, &mY);
    if (mX > x && mY > y && mX < x + sNull->w && mY < y + sNull->h && active) {
        if (state == B_MUP) {
            state = B_MHOVER;
        } else if (state == B_MDOWN) {
            state = B_MCLICK;
        }
    } else {
        state = B_MNULL;
    }

    if (SDL_GetTicks() - actionStart >= moveTime) {
        switch (action) {
            case B_MIN:
                action = B_NULL;
                active = true;
                break;
            case B_MOUT:
                action = B_AWAY;
                active = false;
                break;
        }
    }

    return 0;
}

int cButton::render(SDL_Surface* dst) {
    if (action == B_AWAY) return 0;

    bool render = true;
    int pX = 0, pY = 0;
    SDL_Surface* sButton = NULL;
    float progress = 0;

    if (action == B_MOUT || action == B_MIN) {
        progress = (SDL_GetTicks() - actionStart) / (float)moveTime;
    }

    if (action == B_MOUT) {
        progress = 1 - progress;
    }

    switch (state) {
        case B_MDOWN:
            sButton = sNull;
            break;
        case B_MUP:
            sButton = sHover;
            break;
        case B_MHOVER:
            sButton = sHover;
            break;
        case B_MCLICK:
            sButton = sNull;
            break;
        case B_MNULL:
            sButton = sNull;
            break;
    }

    switch (action) {
        case B_MIN:
        case B_MOUT:
            switch (dir) {
                case DIR_UP:
                    pX = x;
                    pY = 0 - sButton->h + progress * (y + sButton->h);
                    break;
                case DIR_DOWN:
                    pX = x;
                    pY = y + (1 - progress) * (B_SCR_H + sButton->h - y);
                    break;
                case DIR_LEFT:
                    pX = 0 - sButton->w + progress * (x + sButton->w);
                    pY = y;
                    break;
                case DIR_RIGHT:
                    pX = x + (1 - progress) * (sButton->w + B_SCR_W - x);
                    pY = y;
                    break;
            }
            break;
        case B_AWAY:
            render = false;
            break;
        case B_NULL:
            pX = x;
            pY = y;
            break;
    }

    if (render) {
        applySurfaceButton(sButton, dst, pX, pY);
    }
    return 0;
}

eBAction cButton::gAction() {
    return action;
}

eBState cButton::gState() {
    return state;
}
