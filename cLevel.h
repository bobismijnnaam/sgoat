#ifndef CLEVEL_H
#define CLEVEL_H

struct coord {
    int x;
    int y;
    bool hit;
} ;

class cLevel {
public:
    cLevel();
    ~cLevel();

    coord raycast(int x1, int y1, int x2, int y2);
    int addWall(int t, int r, int b, int l);

    // Slide col command?

private:



} ;

#endif
