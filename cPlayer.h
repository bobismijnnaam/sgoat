#ifndef CPLAYER_H
#define CPLAYER_H

class cPlayer {
public:
    cPlayer();
    ~cPlayer();

    int events(SDL_Event* event);
    int logic();
    int render(SDL_Surface* dst);

    int viewViewport(bool p);

private:
    SDL_Surface* playerImg;

    float x, y;
    float xvel, yvel;

    int lastMoveTime;

    bool viewport;
} ;

#endif
