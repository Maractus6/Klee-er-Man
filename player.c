#include "gba.h"
#include "player.h"
#include "klee.h"
#include "mode4.h"
#include "game.h"
#include "print.h"
#include "analogSound.h"

//struct init
PLAYER player;
BLOCK blocks[BLOCKCOUNT];
OBSTACLE obstacles[OBSTACLECOUNT];

//intializes player values
void initPlayer(PLAYER* player) {
    player -> x = 18;
    player -> y = 62;
    player -> xVel = 16;
    player -> yVel = 12;
    player -> height = 12;
    player -> width = 12;
    player -> direction = 'u';
}

//updates player location
void updatePlayer(PLAYER* player) {
    //mgba_printf("player: %d, %d, %d, %d", player -> x, player -> y, player -> width, player -> height);
    if(BUTTON_PRESSED(BUTTON_UP)) {
        if ((player -> y - player -> yVel) > 10) {
            player -> y -= player -> yVel;
            player -> direction = 'u';
            
        }
        if (blockCollision(&player)) {
            player -> y += player -> yVel;
            //mgba_printf("collision");
        }
    }
    if(BUTTON_PRESSED(BUTTON_DOWN)) {
        if ((player -> y + player -> yVel) < 145) {
            player -> y += player -> yVel;
            player -> direction = 'd';
        }
    }
    if(BUTTON_PRESSED(BUTTON_LEFT)) {
        if ((player -> x - player -> xVel) > 10) {
            player -> x -= player -> xVel;
            player -> direction = 'l';
        }
        if (blockCollision(&player)) {
            player -> x += player -> xVel;
        }
    }
    if (BUTTON_PRESSED(BUTTON_RIGHT)) {
        if ((player -> x + player -> xVel) < 230) {
            player -> x += player -> xVel;
            player -> direction = 'r';
        }
    }
}

// block collision btw player and block
int blockCollision(PLAYER* player) {
    //mgba_printf("player: %d, %d, %d, %d", player -> x, player -> y, player -> width, player -> height);
    for (int i = 0; i < BLOCKCOUNT; i++) {
        if (collision(player -> x, player -> y, player -> width, player -> height,
         blocks[i].x, blocks[i].y, blocks[i].width, blocks[i].height) && (blocks[i].active == 1)) {
            REG_SND2CNT = DMG_ENV_VOL(4) | DMG_DIRECTION_DECR |  DMG_STEP_TIME(2);
            REG_SND2FREQ = NOTE_F3 | SND_RESET;
            return 1;
        }
        //mgba_printf("collision: %d", collision(player -> x, player -> y, player -> width, player -> height,
         //blocks[i].x, blocks[i].y, blocks[i].width, blocks[i].height));
        //mgba_printf("player: %d, %d, %d, %d", player -> x, player -> y, player -> width, player -> height);

    }

    return 0;
}

//draws player
void drawPlayer(PLAYER* player) {
    drawImage4(player -> x, player -> y, player -> width, player -> height, kleeBitmap);
}
