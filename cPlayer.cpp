#include <cmath>
#include <SDL/SDL.h>
#include <string>
#include <sstream>
#include "SDL_gfxPrimitives.h"

#include "cPlayer.h"
#include "cLevel.h"
#include "constants.h"
#include "functions.h"
#include "globals.h"

cPlayer::cPlayer(int Xs, int Ys, int Xe, int Ye) {
    playerImg = loadImage("Media/Graphics/player.png");

    viewport = false;

    x = Xs;
    y = Ys;
    xvel = 0;
    yvel = 0;
    speed = PLAYER_SPDNORMAL;

    lastMoveTime = SDL_GetTicks();

    xs = Xs;
    ys = Ys;
    xe = Xe;
    ye = Ye;

    col = false;
}

cPlayer::~cPlayer() {
    SDL_FreeSurface(playerImg);

}

int cPlayer::events(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_w:
                yvel = -1;
                break;
            case SDLK_a:
                xvel = -1;
                break;
            case SDLK_s:
                yvel = 1;
                break;
            case SDLK_d:
                xvel = 1;
                break;
            case SDLK_LSHIFT:
                speed = PLAYER_SPDSNEAK;
                break;
            case SDLK_SPACE:
                // speed = 5 * PLAYER_SPDNORMAL;
                break;
        }
    } else if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.sym) {
            case SDLK_w:
                if (yvel < 0) yvel = 0;
                break;
            case SDLK_a:
                if (xvel < 0) xvel = 0;
                break;
            case SDLK_s:
                if (yvel > 0) yvel = 0;
                break;
            case SDLK_d:
                if (xvel > 0) xvel = 0;
                break;
            case SDLK_SPACE:
            case SDLK_LSHIFT:
                speed = PLAYER_SPDNORMAL;
                break;
        }
    }

    return 0;
}

int cPlayer::logic(cLevel* level) {
    int thisMoveTime = SDL_GetTicks();
    float dt = (thisMoveTime - lastMoveTime) / 1000.0;

    BOB_Rect playerRect;
    coord centroid;

    // Movement. r2inv = 1/root(2) to account for diagonal movement
    if (xvel != 0 && yvel != 0) {
        x += xvel * speed * dt * r2inv;
        y += yvel * speed * dt * r2inv;
    } else {
        x += xvel * speed * dt;
        y += yvel * speed * dt;
    }

    playerRect.x = x - playerImg->w / 2;
    playerRect.y = y - playerImg->h / 2;
    playerRect.w = playerImg->w;
    playerRect.h = playerImg->h;

    centroid.x = x;
    centroid.y = y;

    // Collision
    if (level->slideCol(&playerRect, &centroid)) {
        x = centroid.x;
        y = centroid.y;
    }

    lastMoveTime = thisMoveTime;

    SDL_Rect t;
    t.x = xe - 25;
    t.y = ye - 25;
    t.w = 50;
    t.h = 50;

    SDL_Rect p = this->playerRect();

    if (rectCol(p, t)) {
        return PLAYER_GOTOUT;
    }

    return 0;
}

int cPlayer::render(SDL_Surface* dst) { // Player is rendered with
    applySurface(playerImg, dst, (SCR_W - playerImg->w) / 2, (SCR_H - playerImg->h) / 2);

    if (col) applySurface(playerImg, dst, 0, 0);

    if (viewport) {
        SDL_Surface* tS;
        std::string coord;
        std::stringstream ss;
        SDL_Color clrWhite = {255, 255, 255};

        ss << (int)x - 350; // Left
        tS = TTF_RenderText_Blended(fSmall, ss.str().c_str(), clrWhite);
        applySurface(tS, dst, 0, 350);
        SDL_FreeSurface(tS);
        ss.str(std::string());
        ss.clear();

        ss << (int)x + 350; // Right
        tS = TTF_RenderText_Blended(fSmall, ss.str().c_str(), clrWhite);
        applySurface(tS, dst, SCR_W - tS->w, 350);
        SDL_FreeSurface(tS);
        ss.str(std::string());
        ss.clear();

        ss << (int)y - 350; // Top
        tS = TTF_RenderText_Blended(fSmall, ss.str().c_str(), clrWhite);
        applySurface(tS, dst, 350, 0);
        SDL_FreeSurface(tS);
        ss.str(std::string());
        ss.clear();

        ss << (int)y + 350; // Top
        tS = TTF_RenderText_Blended(fSmall, ss.str().c_str(), clrWhite);
        applySurface(tS, dst, 350, SCR_H - tS->h);
        SDL_FreeSurface(tS);
        ss.str(std::string());
        ss.clear();

        ss << "(" << (int)x << ", " << (int)y << ")";
        tS = TTF_RenderText_Blended(fSmall, ss.str().c_str(), clrWhite);
        applySurface(tS, dst, 350 - tS->w / 2, 350 - tS->h / 2 - 30);
        SDL_FreeSurface(tS);
    }

    return 0;
}

int cPlayer::viewViewport(bool p) {
    viewport = p;

    return 0;
}

SDL_Rect cPlayer::getViewport() {
    SDL_Rect t;
    t.x = x - 350;
    t.y = y - 350;
    t.w = 700;
    t.h = 700;

    return t;
}

SDL_Rect cPlayer::playerRect() {
    SDL_Rect t;
    t.x = x - playerImg->w / 2;
    t.y = y - playerImg->h / 2;
    t.w = playerImg->w;
    t.h = playerImg->h;

    return t;
}

coord cPlayer::gPlayerStart() {
    coord t;
    t.x = xs;
    t.y = ys;

    return t;
}

coord cPlayer::gPlayerEnd() {
    coord t;
    t.x = xe;
    t.y = ye;

    return t;
}

float cPlayer::gX() {
    return x;
}

float cPlayer::gY() {
    return y;
}

int cPlayer::gSpd() {
    if (xvel == 0 && yvel == 0) {
        return 0;
    } else {
        return speed;
    }
}

int cPlayer::pause() {
    xvel = 0;
    yvel = 0;

    return 0;
}

int cPlayer::resume() {

    return 0;
}

