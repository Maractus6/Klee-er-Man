#ifndef GAME_H
#define GAME_H

#define BLOCKCOUNT 50
#define BLOCKWIDTH 16
#define BLOCKHEIGHT 11
#define NUMCOLORS 4
#define OBSTACLECOUNT 30

#include "player.h"

enum {BLACKID=(256-NUMCOLORS), WHITEID, SALMONID, BABYBLUEID};
extern unsigned short colors[NUMCOLORS];

extern PLAYER player;


typedef struct {
    int x;
    int y;
    int height;
    int width;
    int range;
    int active;
} BOMB;


//block strict
typedef struct {
    int x;
    int y;
    int height;
    int width;
    unsigned short color;
    int active;
    int erased;
} BLOCK;

//obstacle strict
typedef struct {
    int x;
    int y;
    int height;
    int width;
    unsigned short color;
} OBSTACLE;



void createBomb(PLAYER* player);
void initBlocks();
void initGame();
void updateGame();
void explosion();
void drawGame();
void drawBomb(BOMB* bomb);
void drawBlocks();
void bombCollision(BOMB* bomb);
void resetBlocks();


#endif