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
    debug = false;

    // Initialize game objects
    ai = new cAI();

    // Initialize level
    switch (flvl) {
        case LEVEL_OUVERTURE:
            player = new cPlayer(-265, -265, 265, -265);
            level = new cLevel(screen, -300, 300, 100, -300, player);

            // Level
            level->addWall(-230, -230, 100, -300);
            level->addWall(-230, 300, 100, 230);
            level->addWall(-300, -20, -100, -120);

            // AI
            ai->addSleeper(90, -265, 210);
            ai->addPointGuard(-190, 60, .1*pi, .5 * pi, 5000, false);
            ai->addSleeper(165, 40, 165);

            break;
        case LEVEL_ANACRUSIS:
            player = new cPlayer(75/2, 75/2, 1225 + 75 / 2, 75 / 2);
            level = new cLevel(screen, 0, 1300, 1000, 0, player);

            // Level
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

            // AI
            ai->addPointGuard(150 + 425 / 2 - 50, 575 + 425 / 2, 1.25 * pi, 2 * pi, 7500, false);
            ai->addSleeper(150 + 350 / 2, 75 + 350 / 2, 350 / 2);

            ai->addWaypoint(1225 + 75 / 2, 75 + 75 / 2);
            ai->addWaypoint(1225 + 75 / 2, 500 + 75 / 2);
            ai->addWaypoint(800 + 75 / 2, 500 + 75 / 2);
            ai->addPatrolGuard(800 + 75 / 2, 75 + 75 / 2, 100, 1000, true);

            break;
        case LEVEL_CRESCENDO:
            player = new cPlayer(5 * 75 + 75 / 2, 75 * 1.5, 1987, 3813);
            level = new cLevel(screen, 0, 3000, 4275, -100, player);

            int x;
            int y;

            // Level
            level->addWall(7.5 * 75, 15.5 * 75, 15.5 * 75, 7.5 * 75); // Center block

            // Vertical cell block walls
            for (int j = 0; j < 2; ++j) {
                y = j * (1 + 3.5 + 8 + 3.5 + 1 + 3) * 75;
                for (int i = 0; i < 5; ++i) {
                    x = (i + 1) * 3 * 75  + i * 75;
                    level->addWall(y, x + 75, y + 3 * 75, x);
                }
            }

            // Horizontal cell block walls
            for (int j = 0; j < 2; ++j) {
                x = j * (1 + 3.5 + 8 + 3.5 + 1 + 3) * 75;
                for (int i = 0; i < 5; ++i) {
                    y = (i + 1) * 3 * 75 + i * 75;
                    level->addWall(y, x + 3 * 75, y + 75, x);
                }
            }

            // Cell entrance horizontal walls
            for (int j = 0; j < 2; ++j) {
                y = 3 * 75 + (16 * 75) * j;
                for (int i = 0; i < 3; ++i) {
                    x = 6 * 75 + i * 4 * 75;
                    level->addWall(y, x + 3 * 75, y + 75, x);
                }
                level->addWall(y, 5 * 75, y + 75, 3 * 75);
                level->addWall(y, 18 * 75 + 2 * 75, y + 75, 18 * 75);
            }

            // Cell entrance vertical walls
            for (int j = 0; j < 2; ++j) {
                x = 3 * 75 + j * 16 * 75;
                for (int i = 0; i < 3; ++i) {
                    y = 6 * 75 + i * 4 * 75;
                    level->addWall(y, x + 75, y + 3 * 75, x);
                }
                level->addWall(4 * 75, x + 75, 5 * 75, x);
                level->addWall(18 * 75, x + 75, 19 * 75, x);
            }

            level->addWall(23 * 75, 16 * 75, 24 * 75, 0); // Wall closes off bottom cells
            level->addWall(3 * 75, 24 * 75, 19 * 75, 23 * 75); // Wall closes off right cells
            level->addWall(1725, 980, 1750, 350);

            // First room
            level->addWall(1725, 20 * 75, 26 * 75, 19 * 75); // Left
            level->addWall(27 * 75, 25 * 75, 28 * 75, 19 * 75); // Bottom
            level->addWall(20 * 75, 25 * 75, 27 * 75,24 * 75); // Right
            level->addWall(19 * 75, 1725 + 2 * 75, 20 * 75, 1725); // Top

            // Middle section
            level->addWall(27 * 75, 8 * 75, 33 * 75, 7 * 75); // Middle section divisor
            level->addWall(27 * 75, 7 * 75, 28 * 75, 4 * 75); // Cell top
            level->addWall(31 * 75, 7 * 75, 32 * 75, 4 * 75); // Cell bottom
            level->addWall(27 * 75, 4 * 75, 29 * 75, 3 * 75); // Entrance top
            level->addWall(30 * 75, 4 * 75, 32 * 75, 3 * 75); // Entrance bottom
            level->addWall(27 * 75, 15 * 75, 28 * 75, 9 * 75); // Room top
            level->addWall(27 * 75, 16 * 75, 44 * 75, 15 * 75); // Room right
            level->addWall(33 * 75, 16 * 75, 34 * 75, 0); // Border between bottom and middle part of level

            // Weird hallway
            level->addWall(2100, 20 * 75, 2100 + 2 * 75, 19 * 75); // Extension to weird hallway
            level->addWall(2100 + 75, 26 * 75, 2100 + 2 * 75, 20 * 75); // Weird hallway entrance top
            level->addWall(2100 + 3 * 75, 35 * 75, 2100 + 4 * 75, 19 * 75); // Weird hallway entrance bottom

            level->addWall(14 * 75, 27 * 75, 2100 + 2 * 75, 26 * 75); // First hallway up left wall
            level->addWall(16 * 75, 29 * 75, 2100 + 2 * 75, 28 * 75); // First hallway up right wall
            level->addWall(14 * 75, 40 * 75, 15 * 75, 27 * 75); // First hallway right top wall
            level->addWall(16 * 75, 40 * 75, 17 * 75, 29 * 75); // First hallway right bottom wall

            level->addWall(2100 + 75, 37 * 75, 2100 + 2 * 75, 29 * 75); // second hallway top wall
            level->addWall(2100 + 2 * 75, 37 * 75, 2100 + 10 * 75, 36 * 75); // Second hallway right wall
            level->addWall(2100 + 4 * 75, 35 * 75, 2100 + 10 * 75, 34 * 75); // Second hallway left wall
            level->addWall(2100 + 10 * 75, 37 * 75, 2100 + 11 * 75, 34 * 75); // Second hallway end

            // Cooler room
            level->addWall(35 * 75, 2 * 75, (35 + 10 + 1 + 10 + 1) * 75, 75); // First op cooler
            placeCoolers(3 * 75, 35 * 75, 2, level); // Two top coolers
            level->addWall(34 * 75, 8 * 75, 45 * 75, 7 * 75); // Right cooler wall
            placeCoolers(19 * 75, 46 * 75, 6, level); // More coolers, right ones
            level->addWall(44 * 75, 16 * 75, 45 * 75, 8 * 75); // Bottom left entrance wall
            level->addWall(44 * 75, 32 * 75, 45 * 75, 19 * 75); // Bottom right entrance wall
            level->addWall(2100 + 4 * 75, 20 * 75, 44 * 75, 19 * 75); // Top right entrance wall
            placeCoolers(3 * 75, 46 * 75, 7, level); // Coolers on the left
            level->addWall(45 * 75, 32 * 75, 4275, 31 * 75);

            // AI
            ai->addWaypoint(5.5 * 75, 17.5 * 75); // Cellen links
            ai->addPatrolGuard(5.5 * 75, 5.5 * 75, 100, 1000, false);

            ai->addWaypoint(17.5 * 75, 5.5 * 75); // Cellen top, rechts, onder
            ai->addWaypoint(17.5 * 75, 17.5 * 75);
            ai->addWaypoint(9.5 * 75, 17.5 * 75);
            ai->addPatrolGuard(9.5 * 75, 5.5 * 75, 100, 1000, false);

            ai->addWaypoint(1.5 * 75, 25.5 * 75); // Storerooms midden
            ai->addWaypoint(1.5 * 75, 32.5 * 75);
            ai->addPatrolGuard(14.5 * 75, 25.5 * 75, 100, 1000, false);

            ai->addSleeper(22 * 75, 25 * 75, 2 * 75); // Middle section stationaries
            ai->addPointGuard(869, 29 * 75, .25 * pi, .75 * pi, 2000, false);

            ai->addWaypoint(27.5 * 75, 15.5 * 75); // Weird hallway guard
            ai->addWaypoint(38.5 * 75, 15.5 * 75);
            ai->addPatrolGuard(27.5 * 75, 2100 + 75 * .5, 100, 1000, false);

            ai->addWaypoint(6.5 * 75, 45.5 * 75);
            ai->addWaypoint(6.5 * 75, 34.5 * 75);
            ai->addWaypoint(2.5 * 75, 34.5 * 75);
            ai->addWaypoint(2.5 * 75, 45.5 * 75);
            ai->addWaypoint(4.5 * 75, 45.5 * 75);
            ai->addWaypoint(4.5 * 75, 56.5 * 75);
            ai->addWaypoint(14.5 * 75, 56.5 * 75);
            ai->addPatrolGuard(14.5 * 75, 45.5 * 75, 100, 1000, true); // Left cooler guard

            ai->addWaypoint(18.5 * 75, 56.5 * 75);
            ai->addWaypoint(22.5 * 75, 56.5 * 75);
            ai->addWaypoint(22.5 * 75, 45.5 * 75);
            ai->addPatrolGuard(18.5 * 75, 45.5 * 75, 100, 1000, true); // Right-left cooler guard

            ai->addWaypoint(30.5 * 75, 45.5 * 75);
            ai->addWaypoint(30.5 * 75, 56.5 * 75);
            ai->addWaypoint(26.5 * 75, 56.5 * 75);
            ai->addWaypoint(26.5 * 75, 50.8 * 75);
            ai->addWaypoint(26.5 * 75, 50.9 * 75);
            ai->addPatrolGuard(26.5 * 75, 45.5 * 75, 100, 1000, true); // Right-right cooler guard

            break;
        case LEVEL_TUTORIAL:
            player = new cPlayer(107, 192, 58, 1641);
            level = new cLevel(screen, 0, 1000, 1700, 0, player);

            // Level
            level->addWall(500, 925, 575, 0); // Top bottom
            level->addWall(0, 575, 425, 500); // Top middle
            level->addWall(1000, 1000, 1075, 600); // Middle bottom
            level->addWall(650, 600, 1275, 525); // Middle middle

            level->addWall(650, 225, 725, 0); // Entrance left
            level->addWall(650, 525, 725, 300); // Entrance right
            level->addWall(725, 225, 1200, 150); // Left corridor wall
            level->addWall(725, 375, 1200, 300); // Right corridor wall
            level->addWall(1200, 225, 1275, 0); // Left exit
            level->addWall(1200, 525, 1275, 300); // Right exit

            level->addWall(1275, 600, 1700, 595); // Window wall
            level->addWall(1075, 1000, 1700, 825); // Patrol room right wall

            // AI
            ai->addSleeper(790, 239, 390 - 239);
            ai->addPointGuard(815, 730, .25 * pi, .75 * pi, 2000, false);
            ai->addSleeper(75, 962, 225);
            ai->addPointGuard(445, 962, .75 * pi, 1.25 * pi, 2000, false);

            ai->addWaypoint(775, 1117);
            ai->addWaypoint(775, 1280);
            ai->addWaypoint(643, 1280);
            ai->addWaypoint(643, 1456);
            ai->addWaypoint(775, 1456);
            ai->addWaypoint(775, 1669);
            ai->addWaypoint(643, 1669);
            ai->addPatrolGuard(643, 1117, 100, 1000, true);

            break;
    }

    player->viewViewport(debug);
    bgEnabled = !debug;

    if (bgEnabled) {
        if (flvl == LEVEL_OUVERTURE) sBG = loadImage("Media/Levels/lvl1.jpg");
        if (flvl == LEVEL_ANACRUSIS) sBG = loadImage("Media/Levels/lvl2.jpg");
        if (flvl == LEVEL_CRESCENDO) sBG = loadImage("Media/Levels/lvl3.jpg");
        if (flvl == LEVEL_TUTORIAL) sBG = loadImage("Media/Levels/lvlhelp.jpg");
    }

    // Initialize various variables
    mx = 0;
    my = 0;

    Uint32 nclr = SDL_MapRGB(screen->format, 255, 255, 255);
    Uint32 hclr = SDL_MapRGB(screen->format, 0, 150, 0);
    pauseButtons = new cButtonSet(fButton, true, B_AWAY, 200, 300, 0, 80, 500, 100, DIR_LEFT, nclr, hclr, screen->format);
    pauseButtons->addB("Resume");
    pauseButtons->addB("Level select");
    sPause = createSurf(SCR_W, SCR_H, screen);
    fresh(sPause, true);
    SDL_SetAlpha(sPause, SDL_SRCALPHA, 128);

    endButtons = new cButtonSet(fButton, true, B_AWAY, 200, 400, 0, 80, 500, 100, DIR_LEFT, nclr, hclr, screen->format);
    endButtons->addB("Retry");
    endButtons->addB("Level select");

    SDL_Color c = {0, 0, 0};
    sPaused = TTF_RenderText_Blended(fBig, "You paused the game.", c);

    c.r = 255;
    c.g = 0;
    c.b = 0;
    sLost = TTF_RenderText_Blended(fBig, "You got caught.", c);

    c.g = 255;
    c.r = 0;
    c.b = 0;
    sWon = TTF_RenderText_Blended(fBig, "You got out!", c);

    c.b = 255;
    c.r = 0;
    c.g = 0;
    sStart = TTF_RenderText_Blended(fBig, "Press any key to start", c);

    sHeader = loadImage("Media/Graphics/headerbg.jpg");
    sOverlay = loadImage("Media/Graphics/overlay.png");

    // State variable
    state = InGameStates_BOOT;
    lvl = flvl;
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
    SDL_FreeSurface(sStart);
    SDL_FreeSurface(sHeader);
    SDL_FreeSurface(sOverlay);
    if (bgEnabled) SDL_FreeSurface(sBG);
}

int gsGame::events() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT); // Quit game if escape is pressed/red cross is clicked
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE && state != InGameStates_START) {
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

            if (state == InGameStates_START) {
                state = InGameStates_GAME;
                ai->resume();
                player->resume();
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
        case InGameStates_BOOT:
        case InGameStates_START:
        case InGameStates_GAME:
            // Update player
            if (player->logic(level) == PLAYER_GOTOUT && !debug) {
                state = InGameStates_WON;
                lossWinTime = SDL_GetTicks();
                endButtons->moveIn();
            }

            // Update level
            level->logic(player);

            // Update AI
            ai->logic(player, level);
            if (ai->isSpotted() && state == InGameStates_GAME && !debug) {
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

    if (state == InGameStates_BOOT) {
        ai->pause();
        player->pause();
        state = InGameStates_START;
    }

    /*
    int realMx, realMy;
    coord tc;
    tc.x = mx;
    tc.y = my;
    tc = level->toWorld(tc);
    /*
    if (level->raycast(player->gX(), player->gY(), tc.x, tc.y, true)) {
        hit = true;
        tc = level->getRayHit();
        tc = level->toScreen(tc);
        hx = tc.x;
        hy = tc.y;
    } else {
        hit = false;
    }
    */


    return 0;
}

int gsGame::render(SDL_Surface* dst) {
    // Clear screen
    SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, 220, 220, 220));

    SDL_Rect tr;
    SDL_Rect vp;
    if (bgEnabled && lvl != LEVEL_CRESCENDO) {
        vp = player->getViewport();
        tr.x = vp.x - level->getEdge(3); // Gets the left edge of the lvl, needed for offsetting the background
        tr.y = vp.y - level->getEdge(0);
        tr.w = 700;
        tr.h = 700;
        applyClipped(sBG, dst, 0, 0, &tr);
    } else if (bgEnabled && lvl == LEVEL_CRESCENDO) {
        vp = player->getViewport();
        tr.x = vp.x - level->getEdge(3); // Gets the left edge of the lvl, needed for offsetting the background
        tr.y = vp.y - level->getEdge(0);
        tr.w = 700;
        tr.h = 700;
        applyTile(sBG, dst, 0, 0, 700, 700, (int)player->gX() % sBG->w, (int)player->gY() % sBG->h);
    }

    level->render(dst);
    player->render(dst);
    ai->render(dst, level);
    applySurface(sOverlay, dst, 0, 0);

    int t;
    switch (state) {
        case InGameStates_START:
            applySurface(sHeader, dst, 0, 90);
            applySurface(sStart, dst, (SCR_W - sStart->w) / 2, 100);
            break;
        case InGameStates_GAME:
            break;
        case InGameStates_PAUSED:
            applySurface(sPause, dst, 0, 0);
            pauseButtons->render(dst);
            applySurface(sHeader, dst, 0, 90);
            applySurface(sPaused, dst, (SCR_W - sPaused->w) / 2, 100);
            break;
        case InGameStates_LOSS:
            t = ((SDL_GetTicks() - lossWinTime) % 800) / 800.0 * 80 + 0;
            boxRGBA(dst, 0, SCR_H, SCR_W, 0, 255, 0, 0, (Uint8)t);
            applySurface(sHeader, dst, 0, 90);
            applySurface(sLost, dst, (SCR_W - sLost->w) / 2, 100);
            endButtons->render(dst);
            break;
        case InGameStates_WON:
            t = ((SDL_GetTicks() - lossWinTime) / 10000.0) * 80;
            if (t > 80) t = 80;
            boxRGBA(dst, 0, SCR_H, SCR_W, 0, 0, 80, 0, (Uint8)t);
            applySurface(sHeader, dst, 0, 90);
            applySurface(sWon, dst, (SCR_W - sWon->w) / 2, 100);
            endButtons->render(dst);
            break;
    }

    return 0;
}

