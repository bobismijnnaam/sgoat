#include <cmath>
#include "SDL_gfxPrimitives.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <string>
#include <sstream>

#include "cLevel.h"
#include "constants.h"
#include "functions.h"
#include "globals.h"

// http://math.stackexchange.com/questions/110080/shortest-way-to-achieve-target-angle
float toAng(float s, float e, float t) { // Moves to shortest angle
    bool clockwise;
    float d, diff;
    float a, b, c;
    a = e - s;
    b = e - s + 2 * pi;
    c = e - s - 2 * pi;

    float fa, fb, fc;
    fa = fabs(a);
    fb = fabs(b);
    fc = fabs(c);

    if (fa <= fb && fa <= fc) {
        d = a;
    } else if (fb <= fa && fb <= fc) {
        d = b;
    } else {
        d = c;
    }

    if (d > 0) {
        clockwise = true;
    } else {
        clockwise = false;
    }

    if (!clockwise) {
        if (e < s) { // Counter clockwise
            diff = s - e;
        } else {
            diff = s + (2 * pi - e);
        }

        return s - diff * t;
    } else {
        if (s < e) { // Clockwise
            diff = e - s;
            return s + diff * t;
        } else {
            diff = e + (2 * pi - s);
            return s + diff * t;
        }
    }
}

float lerpAng(float f, float s, float t, bool clockwise) { // t = [0, 1]
    float F = f;
    float S = s;
    float ang, diff;

    // To keep the angle in domain [0, 2pi] because of atan2
    while (F < 0) {
        F += 2 * pi;
    }
    while (F > 2 * pi) {
        F -= 2 * pi;
    }
    while (S < 0) {
        S += 2 * pi;
    }
    while (S > 2 * pi) {
        S -= 2 * pi;
    }

    F = Bmin(F, S);
    S = Bmax(F, S);

    if (clockwise) {
        diff = F + (2 * pi - S);
        ang = F -  diff * t;
    } else {
        diff = S - F;
        ang = F + diff * t;
    }

    return ang;
}

float diffAng(float f, float s) {
    float F = f;
    float S = s;

    // To keep the angle in domain [0, 2pi] because of atan2
    while (F < 0) {
        F += 2 * pi;
    }
    while (F > 2 * pi) {
        F -= 2 * pi;
    }
    while (S < 0) {
        S += 2 * pi;
    }
    while (S > 2 * pi) {
        S -= 2 * pi;
    }

    float diffCClock = fabs(F - S); // Verschil tegen de klok in.
    float diffClock = Bmin(F, S) + (2.0 * pi - Bmax(F, S)); // Verschil met de klok mee

    // Degrees away from looking angle
    float dAng = Bmin(diffCClock, diffClock);

    return dAng;
}

float tweenInOut(float t) { // give value between 0 and 1. Will smooth it, go slow in the beginning, fast in the middle and slow in the end
    return 6 * t * t * t * t * t - 15 * t * t * t * t + 10 * t * t * t;
}

float Bmin(float f, float s) {
    if (f < s) {
        return f;
    } else {
        return s;
    }
}

float Bmax(float f, float s) {
    if (f > s) {
        return f;
    } else {
        return s;
    }
}

int cross(SDL_Surface* dst, int x, int y, int size, int r, int g, int b, int a) { // Draws a cross
    lineRGBA(dst, x - size, y - size, x + size, y + size, r, g, b, a);
    lineRGBA(dst, x - size, y + size, x + size, y - size, r, g, b, a);

    return 0;
}

int Bmin(int f, int s) { // Returns lowest
    if (f < s) {
        return f;
    } else {
        return s;
    }
}

int Bmax(int f, int s) { // Returns highest
    if (f > s) {
        return f;
    } else {
        return s;
    }
}

// http://en.wikipedia.org/wiki/Line-line_intersection
bool lineline(int L1X1, int L1Y1, int L1X2, int L1Y2, int L2X1, int L2Y1, int L2X2, int L2Y2, int* X, int* Y) { // Returns the point of intersection of two lines
    int D = (L1X1 - L1X2) * (L2Y1 - L2Y2) - (L1Y1 - L1Y2) * (L2X1 - L2X2); // Denominator. If zero then no intersection

    if (D == 0) { // Parallel and possibly overlapping
        return false;
    } else {
        *X = ( (L1X1 * L1Y2 - L1Y1 * L1X2) * (L2X1 - L2X2) - (L1X1 - L1X2) * (L2X1 * L2Y2 - L2Y1 * L2X2) ) / D; // Calculate x
        *Y = ( (L1X1 * L1Y2 - L1Y1 * L1X2) * (L2Y1 - L2Y2) - (L1Y1 - L1Y2) * (L2X1 * L2Y2 - L2Y1 * L2X2) ) / D; // Calculate y

        if (*X >= Bmin(L1X1, L1X2) && *X <= Bmax(L1X1, L1X2) && *Y >= Bmin(L1Y1, L1Y2) && *Y <= Bmax(L1Y1, L1Y2)) {
            // Intersection is on first line
            if (*X >= Bmin(L2X1, L2X2) && *X <= Bmax(L2X1, L2X2) && *Y >= Bmin(L2Y1, L2Y2) && *Y <= Bmax(L2Y1, L2Y2)) {
                // Intersection is on second line
                return true;
            } else {
                // Intersection is on first, but not on second line
                return false;
            }
        } else {
            // Intersection is not on first line.
            return false;
        }

        return true;
    }
}

bool BOBrectCol(BOB_Rect f, SDL_Rect s) {
    bool ret = false;

    float t1, r1, b1, l1;
    float t2, r2, b2, l2;

    t1 = f.y;
    r1 = f.x + f.w;
    b1 = f.y + f.h;
    l1 = f.x;

    t2 = s.y;
    r2 = s.x + s.w;
    b2 = s.y + s.h;
    l2 = s.x;

    // 2 = SDL, 1 = BOB

    // Er zijn vier situaties mogelijk: - rechtsonder1 overlapt linksboven2
    //                                  - linksonder1 overlapt rechtsboven2
    //                                  - linksboven1 overlapt rechtsonder2
    //                                  - rechtsboven1 overlapt linksonder2
    // Die worden hier elk individueel gechecked
    // Als een van de 4 een botsing oplevert geeft de functie sowieso waar.

    if (r1 > l2 && r1 < r2) { // rechts
        if (b1 > t2 && b1 < b2) { // onder1 overlapt linksboven2
            ret = true;
        } else if (t1 > t2 && t1 < b2) { // boven overlapt overlapt linksonder2
            ret = true;
        }
    } else if (l1 > l2 && l1 < r2) { // links
        if (b1 > t2 && b1 <= b2) { // onder1 overlapt rechtsboven2
            ret = true;
        } else if (t1 > t2 && t1 < b2) { // boven1 overlapt met rechtsonder2
            ret = true;
        }
    }

    // Als deze vier situaties niet van toepassing zijn kan het ook nog dat
    // de SDL_Rect omvat wordt door de BOB_Rect. Zeg maar zo:
    //      _____________
    //      |         __|________________
    //      |        |_______SDL_Rect____|
    //      | BOB_Rect  |
    //      |___________|
    // In dat geval werkt de voorgenoemde detectie niet omdat de hoeken van BOB_rect
    // Niet in SDL_Rect vallen. Er moeten dus ook nog vier gevallen gemaakt worden voor als
    // De SDL_Rect alleen de randen snijdt van de BOB_Rect.

    if (t2 > t1 && b2 < b1) {
        if (l2 < r1 && r2 > r1) {
            ret = true;
        } else if (r2 > l1 && l2 < l1) {
            ret = true;
        }
    } else if (l2 > l1 && r2 < r1) {
        if (t2 < b1 && b2 > b1) {
            ret = true;
        } else if (t2 < t1 && b2 > t1) {
            ret = true;
        }
    }

    return ret;
}


bool rectCol(SDL_Rect f, SDL_Rect s) {
    bool ret = false;

    int t1, r1, b1, l1;
    int t2, r2, b2, l2;

    t1 = f.y;
    r1 = f.x + f.w;
    b1 = f.y + f.h;
    l1 = f.x;

    t2 = s.y;
    r2 = s.x + s.w;
    b2 = s.y + s.h;
    l2 = s.x;

    // Er zijn vier situaties mogelijk: - rechtsonder1 overlapt linksboven2
    //                                  - linksonder1 overlapt rechtsboven2
    //                                  - linksboven1 overlapt rechtsonder2
    //                                  - rechtsboven1 overlapt linksonder2
    // Die worden hier elk individueel gechecked

    if (r1 > l2 && r1 < r2) { // rechts
        if (b1 > t2 && b1 < b2) { // onder1 overlapt linksboven2
            ret = true;
        } else if (t1 > t2 && t1 < b2) { // boven overlapt overlapt linksonder2
            ret = true;
        }
    } else if (l1 > l2 && l1 < r2) { // links
        if (b1 > t2 && b1 <= b2) { // onder1 overlapt rechtsboven2
            ret = true;
        } else if (t1 > t2 && t1 < b2) { // boven1 overlapt met rechtsonder2
            ret = true;
        }
    }

    return ret;
}

SDL_Surface* createSurf(int w, int h, SDL_Surface* dst) {
    SDL_Surface* dummy;
    const SDL_PixelFormat* fmt = dst->format;

    dummy = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h,fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

    return dummy;
}

int fresh(SDL_Surface* dst, bool black) {
    int c;

    if (black) {
        c = 0;
    } else {
        c = 255;
    }

    SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, c, c, c));

    return 0;
}

bool inRangeEx (int p, int min, int max) {
    if ((p < max) && (p > min)) {
        return true;
    }

    return false;
}

int applyClipped(SDL_Surface* src, SDL_Surface* dst, int x, int y, SDL_Rect* clip) {
    SDL_Rect tRect;
    int t = 0;

    tRect.x = x;
    tRect.y = y;

    t = SDL_BlitSurface(src, clip, dst, &tRect);

    return 0;
}

int applySurface(SDL_Surface* src, SDL_Surface* dst, int x, int y) {
    SDL_Rect tRect;
    int t = 0;

    tRect.x = x;
    tRect.y = y;

    t = SDL_BlitSurface(src, NULL, dst, &tRect);

    return t;
}

bool pointInRect(int x, int y, SDL_Rect r) {
    if (x >= r.x && x <= r.x + r.w) {
        if (y >= r.y && y <= r.y + r.h) {
            return true;
        }
    }

    return false;
}

SDL_Surface* loadImage(std::string src) {
    SDL_Surface* loaded = NULL;
    SDL_Surface* converted = NULL;

    loaded = IMG_Load(src.c_str());

    if (loaded != NULL) {
        converted = SDL_DisplayFormatAlpha(loaded);
        SDL_FreeSurface(loaded);
    }

    return converted;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
