#include "gba.h"
#include "game.h"
#include "player.h"
#include "klee.h"
#include "background.h"
#include "bomb.h"
#include "font.h"
#include "mode4.h"
#include "box.h"
#include "analogSound.h"

//structs init
PLAYER player;
BLOCK blocks[BLOCKCOUNT];
OBSTACLE obstacles[OBSTACLECOUNT];
BOMB bomb;

//variables
int score;
unsigned short colors[NUMCOLORS] = {BLACK, WHITE, SALMON, BABYBLUE};

//initalizes game
void initGame() {
    score = 0;
    bomb.active = 0;
    initPlayer(&player);
    initBlocks();

    for (int i = 0; i < NUMCOLORS; i++) {
        BG_PALETTE[256-NUMCOLORS+i] = colors[i];
    }
}

//init bomb values
void createBomb(PLAYER* player) {
    bomb.height = 12;
    bomb.width = 12;
    bomb.active = 1;
    if (player -> direction == 'u') {
        bomb.x = player -> x;
        bomb.y = player -> y - 12;
    } else if (player -> direction == 'd') {
        bomb.x = player -> x;
        bomb.y = player -> y + 12;
    } else if (player -> direction == 'l') {
        bomb.x = player -> x - 15;
        bomb.y = player -> y;
    }  else if (player -> direction == 'r') {
        bomb.x = player -> x + 14;
        bomb.y = player -> y;
    }
}

//init blocks values
void initBlocks() {
    int x = 48;
    int y = 37;
    for (int i = 0; i < 50; i++) {
        blocks[i].height = BLOCKHEIGHT;
        blocks[i].width = BLOCKWIDTH;
        blocks[i].color = BG_PALETTE[256];
        blocks[i].active = 1;
        blocks[i].x = 48 + (i/10) * BLOCKWIDTH * 2;
        blocks[i].y = 26 + (i%10) * BLOCKHEIGHT;
    }
}


//draws the bomb onto screen
void drawBomb(BOMB* bomb) {
    drawImage4(bomb -> x, bomb -> y, bomb -> width, bomb -> height, bombBitmap);
}
//draws the blocks onto screen
void drawBlocks() {
    for (int i = 0; i < BLOCKCOUNT; i++) {
        if (blocks[i].active == 1) {
            drawImage4(blocks[i].x, blocks[i].y, blocks[i].width, blocks[i].height, boxBitmap);
        }
    }
}

//updates the game logic
void updateGame() {
    if (BUTTON_PRESSED(BUTTON_A)) {
        createBomb(&player);
    }
    updatePlayer(&player);
    if (bomb.active == 1) {
        drawBomb(&bomb);
    }
    bombCollision(&bomb);
    resetBlocks();
}
//add this if i want to add bomb animation
void explosion(BOMB* bomb);

//respawn blocks 
void resetBlocks() {
    if (score % 50 == 0) {
        initBlocks();
    }
}

//draws the game
void drawGame() {
    drawBlocks();
    drawPlayer(&player);
}

//does the bomb collision logic
void bombCollision(BOMB* bomb) {
    for (int i = 0; i < BLOCKCOUNT; i++) {
        if (collision(bomb -> x, bomb -> y, bomb -> width, bomb -> height,
        blocks[i].x, blocks[i].y, blocks[i].width, blocks[i].height)  && (blocks[i].active == 1)) {
            blocks[i].active = 0;
            bomb -> active = 0;
            score++;
            REG_SND2CNT = DMG_ENV_VOL(4) | DMG_DIRECTION_DECR |  DMG_STEP_TIME(2);
            REG_SND2FREQ = NOTE_G2 | SND_RESET;
            //return 1;
        }
    }
    //return 0;
}




