#ifndef CLINERECTISCT_H
#define CLINERECTISCT_H

#include <SDL/SDL.h>

enum enumWhichLine {
    lineTop = 0,
    lineRight,
    lineBottom,
    lineLeft
} ;

enum enumWhichCoord {
    coordX1 = 0,
    coordY1,
    coordX2,
    coordY2
} ;

class cLineRectISCT {
public:
    cLineRectISCT();
    ~cLineRectISCT();

    int setRect(SDL_Rect* srcRect);
    int setLine(int X1, int Y1, int X2, int Y2);
    int set(SDL_Rect* srcRect, int X1, int Y1, int X2, int Y2);

    bool calc();
    int get(int* X1, int* Y1, int* X2, int* Y2);

private:
    SDL_Rect rect;
    int lx1, ly1, lx2, ly2; // Line beginning/ending
    int x1, y1, x2, y2; // Two points of collision
    int ctr;

} ;

#endif
