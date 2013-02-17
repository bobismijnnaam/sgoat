

#include "cLineRectISCT.h"
#include "functions.h"

cLineRectISCT::cLineRectISCT() {
    // Niks te zien hier
}

cLineRectISCT::~cLineRectISCT() {
    // Doorscrollen aub
}

int cLineRectISCT::setRect(SDL_Rect* srcRect) {
    rect = *srcRect;

    return 0;
}

int cLineRectISCT::setLine(int X1, int Y1, int X2, int Y2) {
    lx1 = X1;
    ly1 = Y1;
    lx2 = X2;
    ly2 = Y2;

    return 0;
}

int cLineRectISCT::set(SDL_Rect* srcRect, int X1, int Y1, int X2, int Y2) {
    rect = *srcRect;

    lx1 = X1;
    ly1 = Y1;
    lx2 = X2;
    ly2 = Y2;

    return 0;
}

bool cLineRectISCT::calc() { // Calculate the two intersection points
    int sides[4][4]; // [Top Right Bottom Left] [X1 Y1 X2 Y2]
    int hits[2][2];
    int tx, ty;
    ctr = 0;

    // Define all the sides as lines
    sides[lineTop][coordX1] = rect.x;
    sides[lineTop][coordY1] = rect.y;
    sides[lineTop][coordX2] = rect.x + rect.w;
    sides[lineTop][coordY2] = rect.y;

    sides[lineRight][coordX1] = rect.x + rect.w;
    sides[lineRight][coordY1] = rect.y;
    sides[lineRight][coordX2] = rect.x + rect.w;
    sides[lineRight][coordY2] = rect.y + rect.h;

    sides[lineBottom][coordX1] = rect.x;
    sides[lineBottom][coordY1] = rect.y + rect.h;
    sides[lineBottom][coordX2] = rect.x + rect.w;
    sides[lineBottom][coordY2] = rect.y + rect.h;

    sides[lineLeft][coordX1] = rect.x;
    sides[lineLeft][coordY1] = rect.y;
    sides[lineLeft][coordX2] = rect.x;
    sides[lineLeft][coordY2] = rect.y + rect.h;

    for (int i = 0; i < 4; ++i) {
        if (lineline(sides[i][coordX1], sides[i][coordY1], sides[i][coordX2], sides[i][coordY2], lx1, ly1, lx2, ly2, &tx, &ty)) {
            if (ctr == 0) {
                x1 = tx;
                y1 = ty;
                ctr = 1;
            } else {
                if (tx != x1 && ty != y1) {
                    x2 = tx;
                    y2 = ty;
                    ctr = 2;
                }
            }
        }
    }

    if (ctr > 0) {
        return true;
    } else {
        return false;
    }
}

int cLineRectISCT::get(int* X1, int* Y1, int* X2, int* Y2) {
    *X1 = x1;
    *Y1 = y1;
    *X2 = x2;
    *Y2 = y2;

    return ctr;
}
