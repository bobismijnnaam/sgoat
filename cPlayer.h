#ifndef CPLAYER_H
#define CPLAYER_H

class cLevel;

class cPlayer {
public:
    cPlayer();
    ~cPlayer();

    int events(SDL_Event* event);
    int logic(cLevel* level);
    int render(SDL_Surface* dst);

    int viewViewport(bool p);
    SDL_Rect getViewport();

    SDL_Rect playerRect();

    float gX();
    float gY();

private:
    SDL_Surface* playerImg;

    float x, y;
    float xvel, yvel;
    float speed;

    int lastMoveTime;

    bool viewport;
    bool col;
} ;

#endif
