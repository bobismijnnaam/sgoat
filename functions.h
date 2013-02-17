#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>

#include "cLevel.h"

int cross(SDL_Surface* dst, int x, int y, int size, int r, int g, int b, int a);
int Bmin(int f, int s);
int Bmax(int f, int s);
bool lineline(int L1X1, int L1Y1, int L1X2, int L1Y2, int L2X1, int L2Y1, int L2X2, int L2Y2, int* X, int* Y);
bool BOBrectCol(BOB_Rect f, SDL_Rect s);
bool rectCol(SDL_Rect f, SDL_Rect s);
SDL_Surface* createSurf(int w, int h, SDL_Surface* dst);
int fresh(SDL_Surface* dst, bool black); // Dit maakt het scherm/surface helemaal wit (dit is niet moeilijk maar het scheelt typwerk 8D)
bool inRangeEx (int p, int min, int max); // Deze functie checked of een variabele binnen een bepaald bereik is
int applyClipped(SDL_Surface* src, SDL_Surface* dst, int x, int y, SDL_Rect* clip);
int applySurface(SDL_Surface* src, SDL_Surface* dst, int x, int y); // Hiermee kun je een surface op een ander aanbrengen
bool pointInRect(int x, int y, SDL_Rect r); // Dit checked of een punt in een vierkant is (scheelt typwerk!)
SDL_Surface* loadImage(std::string src); // Dit laad een plaatje en brengt in het goede schermformaat (bits per pixel etc_

Uint32 getpixel(SDL_Surface *surface, int x, int y); // Dit is een functie van SDL om pixels uit te lezen (snap ik verder niet)
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel); // Dit is een functie van SDL om pixels te plotten (snap ik verder ook niet)

#endif
