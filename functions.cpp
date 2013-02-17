#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <string>
#include <sstream>

#include "cLevel.h"
#include "functions.h"
#include "globals.h"

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
