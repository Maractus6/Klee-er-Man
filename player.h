#ifndef PLAYER_H
#define PLAYER_H



// structs
typedef struct {
    int x;
    int y;
    int xVel;
    int yVel;
    int height;
    int width;
    char direction;
} PLAYER;


void initPlayer(PLAYER* player);
void updatePlayer(PLAYER* player);
void drawPlayer(PLAYER* player);
int blockCollision(PLAYER* player);
int obstacleCollision(PLAYER* player);

#endif