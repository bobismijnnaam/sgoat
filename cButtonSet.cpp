#include <SDL/SDL.h>
#include "mtwist.h"

#include "buttons.h"

cButtonSet::cButtonSet(TTF_Font* font, bool vert, eBAction fStartAction, int fSX, int fSY, int fDX, int fDY, int fMT, int fDT, fourDir fDir, Uint32 fNClr, Uint32 fHClr, SDL_PixelFormat* fFmt) {
    f = font;

    vertical = vert;
    startAction = fStartAction;

    sX = fSX;
    sY = fSY;

    dX = fDX;
    dY = fDY;

    mT = fMT;
    dT = fDT;

    nClr = fNClr;
    hClr = fHClr;

    dir = fDir;
    fmt = fFmt;
}

cButtonSet::~cButtonSet() {
    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        delete *i;
    }
}

int cButtonSet::handleEvents(SDL_Event* event) {
    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        (*i)->handleEvents(event);
    }

    return 0;
}

int cButtonSet::moveOut() {
    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        (*i)->moveOut();
    }

    return 0;
}

int cButtonSet::moveIn() {
    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        (*i)->moveIn();
    }

    return 0;
}

int cButtonSet::on(int p, bool all) {
    if (all) {
        for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
            (*i)->on();
        }
    } else {
        int c = 0;
        for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
            ++c;
            if (c == p) {
                (*i)->on();
                break;
            }
        }
    }

    return 0;
}

int cButtonSet::off(int p, bool all) {
    if (all) {
        for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
            (*i)->off();
        }
    } else {
        int c = 0;
        for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
            ++c;
            if (c == p) {
                (*i)->off();
                break;
            }
        }
    }

    return 0;
}

bool cButtonSet::onOff(int p) {
    int c = 0;
    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        ++c;
        if (p == c) {
            return (*i)->onOff();
            break;
        }
    }

    return true;
}

int cButtonSet::logic() {
    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        (*i)->logic();
    }

    return 0;
}

int cButtonSet::render(SDL_Surface* dst) {
    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        (*i)->render(dst);
    }

    return 0;
}

int cButtonSet::gClicked() {
    int d = 0;

    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        ++d;
        if ((*i)->gState() == B_MCLICK) {
            return d;
        }
    }

    return 0;
}

int cButtonSet::gPressed() {
    int d = 0;

    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        ++d;
        if ((*i)->gState() == B_MDOWN) {
            return d;
        }
    }

    return 0;
}

int cButtonSet::gReleased() {
    int d = 0;

    for (std::list<cButton*>::iterator i = buttons.begin(), end = buttons.end(); i != end; ++i) {
        ++d;
        if ((*i)->gState() == B_MUP) {
            return d;
        }
    }

    return 0;
}

 eBAction cButtonSet::gState() {
    if (buttons.size() > 0) {
        return buttons.front()->gAction();
    }

    return B_NULL;
 }

int cButtonSet::addB(std::string capt) {
    if (vertical) {
        int d = 0;
        if (dX != 0) {
            d = mt_lrand() % dX;
        }

        int t = 0;
        if (dT != 0) {
            t = mt_lrand() % dT;
        }

        buttons.push_back(new cButton(f, capt, sX - dX * .5 + d, sY + buttons.size() * dY, startAction, dir, mT - dT * .5 + t, nClr, hClr, fmt));
    } else {
        // Horizontaal
        /*
        int d = mt_lrand() % dY;
        int t = mt_lrand() % dT;
        */

        int d = 0;
        if (dY != 0) {
            d = mt_lrand() % dY;
        }

        int t = 0;
        if (dT != 0) {
            t = mt_lrand() % dT;
        }

        buttons.push_back(new cButton(f, capt, sX + buttons.size() * dX, sY - dX * .5 + d, startAction, dir, mT - dT * .5 + t, nClr, hClr, fmt));
    }

    return 0;
}
