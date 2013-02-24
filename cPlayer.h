#ifndef CPLAYER_H
#define CPLAYER_H

class cLevel;

struct coord;

class cPlayer {
public:
    cPlayer(int xs, int ys, int xe, int ye);
    ~cPlayer();

    int events(SDL_Event* event);
    int logic(cLevel* level);
    int render(SDL_Surface* dst);

    int viewViewport(bool p);
    SDL_Rect getViewport();

    SDL_Rect playerRect();
    coord gPlayerStart();
    coord gPlayerEnd();

    int pause();
    int resume();

    float gX();
    float gY();
    int gSpd();

private:
    SDL_Surface* playerImg;

    float x, y;
    float xvel, yvel;
    float speed;

    int xs, ys, xe, ye;

    int lastMoveTime;

    bool viewport;
    bool col;
} ;

#endif
