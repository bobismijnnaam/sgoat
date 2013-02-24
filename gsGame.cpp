#include <SDL/SDL.h>
#include "SDL_gfxPrimitives.h"

#include "buttons.h"
#include "constants.h"
#include "functions.h"
#include "globals.h"
#include "gsGame.h"

/*

            level->addWall(-300, 50, -200, -50); // Bovenste
            level->addWall(-50, 300, 50, 200); // Rechter
            level->addWall(200, 50, 300, -50); // Onderste
            level->addWall(-50, -200, 50, -300); // Linker

            ai->addPointGuard(250, -250, 0, 1.5 * pi, 4000, true);
            ai->addSleeper(0, 0, 200);

            ai->addWaypoint(400, 400);
            ai->addWaypoint(400, 250);
            ai->addWaypoint(252, 145);
            ai->addWaypoint(-136, 135);
            ai->addWaypoint(-136, -121);
            ai->addWaypoint(235, -145);
            ai->addWaypoint(-6, -430);
            ai->addPatrolGuard(-300, 400, 100, 1000, true);
*/

gsGame::gsGame(int flvl) {
    // Initialize game objects
    ai = new cAI();

    // Initialize level
    switch (flvl) {
        case LEVEL_OUVERTURE:
            player = new cPlayer(-265, -265, 265, -265);
            level = new cLevel(screen, -300, 300, 100, -300, player);

            level->addWall(-230, -230, 100, -300);
            level->addWall(-230, 300, 100, 230);
            level->addWall(-300, -20, -100, -120);

            ai->addSleeper(90, -265, 210);
            ai->addPointGuard(-190, 60, .1*pi, .5 * pi, 5000, false);
            ai->addSleeper(165, 40, 165);

            break;
        case LEVEL_ANACRUSIS:
            player = new cPlayer(75/2, 75/2, 1225 + 75 / 2, 75 / 2);
            level = new cLevel(screen, 0, 1300, 1000, 0, player);

            level->addWall(0, 575, 75, 75);
            level->addWall(150, 150, 425, 0);
            level->addWall(425, 425, 575, 0);
            level->addWall(75, 575, 575, 500);
            level->addWall(650, 150, 1000, 75);
            level->addWall(0, 800, 650, 575);
            level->addWall(725, 650, 1000, 575);
            level->addWall(650, 800, 925, 725);
            level->addWall(575, 1300, 1000, 875);
            level->addWall(150, 1225, 500, 875);
            level->addWall(0, 1225, 75, 800);

            ai->addPointGuard(150 + 425 / 2 - 50, 575 + 425 / 2, 1.25 * pi, 2 * pi, 7500, false);
            ai->addSleeper(150 + 350 / 2, 75 + 350 / 2, 350 / 2);

            ai->addWaypoint(1225 + 75 / 2, 75 + 75 / 2);
            ai->addWaypoint(1225 + 75 / 2, 500 + 75 / 2);
            ai->addWaypoint(800 + 75 / 2, 500 + 75 / 2);
            ai->addPatrolGuard(800 + 75 / 2, 75 + 75 / 2, 100, 1000, true);

            break;
        case LEVEL_CRESCENDO:
            player = new cPlayer(-100, 0, 100, 0);
            level = new cLevel(screen, -500, 500, 500, -500, player);
            break;
        case LEVEL_LEDGERLINES:
            player = new cPlayer(-100, 0, 100, 0);
            level = new cLevel(screen, -500, 500, 500, -500, player);
            break;
        case LEVEL_TUTORIAL:
            player = new cPlayer(-100, 0, 100, 0);
            level = new cLevel(screen, -500, 500, 500, -500, player);
            break;
    }

    player->viewViewport(true);
    bgEnabled = true;

    if (bgEnabled) {
        if (flvl == LEVEL_OUVERTURE) sBG = loadImage("Media/Levels/lvl1.png");
        if (flvl == LEVEL_ANACRUSIS) sBG = loadImage("Media/Levels/lvl2.png");
    }

    // Initialize mouse variables
    mx = 0;
    my = 0;

    // State variable
    state = InGameStates_GAME;

    Uint32 nclr = SDL_MapRGB(screen->format, 255, 255, 255);
    Uint32 hclr = SDL_MapRGB(screen->format, 0, 150, 0);
    pauseButtons = new cButtonSet(fButton, true, B_AWAY, 200, 300, 0, 50, 500, 100, DIR_LEFT, nclr, hclr, screen->format);
    pauseButtons->addB("Resume");
    pauseButtons->addB("Level select");
    sPause = createSurf(SCR_W, SCR_H, screen);
    fresh(sPause, true);
    SDL_SetAlpha(sPause, SDL_SRCALPHA, 128);

    endButtons = new cButtonSet(fButton, true, B_AWAY, 200, 400, 0, 50, 500, 100, DIR_LEFT, nclr, hclr, screen->format);
    endButtons->addB("Retry");
    endButtons->addB("Level select");

    SDL_Color c = {255, 255, 255};
    sPaused = TTF_RenderText_Blended(fBig, "You paused the game.", c);

    c.r = 255;
    c.g = 0;
    c.b = 0;
    sLost = TTF_RenderText_Blended(fBig, "You got caught.", c);

    c.g = 255;
    c.r = 0;
    c.b = 0;
    sWon = TTF_RenderText_Blended(fBig, "You got out!", c);
}

gsGame::~gsGame() {
    // Clean up
    delete player;
    delete level;
    delete ai;
    delete pauseButtons;

    SDL_FreeSurface(sPaused);
    SDL_FreeSurface(sPause);
    SDL_FreeSurface(sLost);
    SDL_FreeSurface(sWon);
    if (bgEnabled) SDL_FreeSurface(sBG);
}

int gsGame::events() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT); // Quit game if escape is pressed/red cross is clicked
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                // gm->setNextState(STATE_EXIT);
                if (state == InGameStates_GAME) {
                    state = InGameStates_PAUSED;
                    ai->pause();
                    player->pause();
                    pauseButtons->moveIn();
                } else if (state == InGameStates_PAUSED) {
                    state = InGameStates_GAME;
                    ai->resume();
                    player->resume();
                    pauseButtons->moveOut();
                }
            } else if (event.key.keysym.sym == SDLK_r) { // Niet weggehaald, misschien voor later
                gm->setNextState(STATE_GAME);
            } else if (event.key.keysym.sym == SDLK_2) {

            } else if (event.key.keysym.sym == SDLK_3) {

            }
        } else if (event.type == SDL_MOUSEMOTION) {
            mx = event.motion.x;
            my = event.motion.y;
        }

        switch (state) {
            case InGameStates_GAME:
                player->events(&event);
                level->events(&event);
                ai->events(&event);
                break;
            case InGameStates_PAUSED:
                pauseButtons->handleEvents(&event);
                break;
            case InGameStates_LOSS:
            case InGameStates_WON:
                endButtons->handleEvents(&event);
                break;
        }
    }
    return 0;
}

int gsGame::logic() {
    switch (state) {
        case InGameStates_GAME:
            // Update player
            if (player->logic(level) == PLAYER_GOTOUT) {
                state = InGameStates_WON;
                lossWinTime = SDL_GetTicks();
                endButtons->moveIn();
            }

            // Update level
            level->logic(player);

            // Update AI
            ai->logic(player, level);
            if (ai->isSpotted() && state == InGameStates_GAME) {
                state = InGameStates_LOSS;
                lossWinTime = SDL_GetTicks();
                endButtons->moveIn();
            }
            break;
        case InGameStates_PAUSED:
            switch (pauseButtons->gPressed()) {
                case 1:
                    state = InGameStates_GAME;
                    ai->resume();
                    player->resume();
                    pauseButtons->moveOut();
                    break;
                case 2:
                    gm->setNextState(STATE_LVLSELECT);
                    break;
            }
            break;
        case InGameStates_LOSS:
        case InGameStates_WON:
            switch (endButtons->gPressed()) {
                case 1:
                    gm->setNextState(STATE_GAME);
                    break;
                case 2:
                    gm->setNextState(STATE_LVLSELECT);
                    break;
            }
            break;
    }

    pauseButtons->logic();
    endButtons->logic();

    return 0;
}

int gsGame::render(SDL_Surface* dst) {
    // Clear screen
    SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, 220, 220, 220));

    if (bgEnabled) {
        SDL_Rect tr;
        SDL_Rect vp;
        vp = player->getViewport();
        tr.x = vp.x - level->getEdge(3); // Gets the left edge of the lvl, needed for offsetting the background
        tr.y = vp.y - level->getEdge(0);
        tr.w = 700;
        tr.h = 700;
        applyClipped(sBG, dst, 0, 0, &tr);
    }

    level->render(dst);
    player->render(dst);
    ai->render(dst, level);

    int t;
    switch (state) {
        case InGameStates_GAME:
            break;
        case InGameStates_PAUSED:
            applySurface(sPause, dst, 0, 0);
            pauseButtons->render(dst);
            boxRGBA(dst, 0, 100 + sPaused->h + 10, SCR_W, 90, 0, 0, 0, 255);
            applySurface(sPaused, dst, (SCR_W - sPaused->w) / 2, 100);
            break;
        case InGameStates_LOSS:
            t = ((SDL_GetTicks() - lossWinTime) % 800) / 800.0 * 80 + 0;
            boxRGBA(dst, 0, SCR_H, SCR_W, 0, 255, 0, 0, (Uint8)t);
            boxRGBA(dst, 0, 100 + sLost->h + 10, SCR_W, 90, 0, 0, 0, 255);
            applySurface(sLost, dst, (SCR_W - sLost->w) / 2, 100);
            endButtons->render(dst);
            break;
        case InGameStates_WON:
            t = ((SDL_GetTicks() - lossWinTime) / 10000.0) * 80;
            if (t > 80) t = 80;
            boxRGBA(dst, 0, SCR_H, SCR_W, 0, 0, 80, 0, (Uint8)t);
            boxRGBA(dst, 0, 100 + sWon->h + 10, SCR_W, 90, 0, 0, 0, 255);
            applySurface(sWon, dst, (SCR_W - sWon->w) / 2, 100);
            endButtons->render(dst);
            break;
    }

    return 0;
}

