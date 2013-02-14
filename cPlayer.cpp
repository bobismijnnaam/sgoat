#include <SDL/SDL.h>
#include <string>
#include <sstream>

#include "cPlayer.h"
#include "constants.h"
#include "functions.h"
#include "globals.h"

cPlayer::cPlayer() {
    playerImg = loadImage("Media/Graphics/player.png");

    viewport = false;

    x = 0;
    y = 0;
    xvel = 0;
    yvel = 0;

    lastMoveTime = SDL_GetTicks();
}

cPlayer::~cPlayer() {
    SDL_FreeSurface(playerImg);

}

int cPlayer::events(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_w:
                yvel = -10;
                break;
            case SDLK_a:
                xvel = -10;
                break;
            case SDLK_s:
                yvel = 10;
                break;
            case SDLK_d:
                xvel = 10;
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
        }
    }

    return 0;
}

int cPlayer::logic() {
    int thisMoveTime = SDL_GetTicks();
    float dt = (thisMoveTime - lastMoveTime) / 1000.0;

    x += xvel * dt;
    y += yvel * dt;

    // Collision moet hier nog!

    lastMoveTime = thisMoveTime;

    return 0;
}

int cPlayer::render(SDL_Surface* dst) {
    applySurface(playerImg, dst, (SCR_W - playerImg->w) / 2, (SCR_H - playerImg->h) / 2);

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

    }

    return 0;
}

int cPlayer::viewViewport(bool p) {
    viewport = p;

    return 0;
}

