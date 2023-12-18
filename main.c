#include "gba.h"
#include <stdio.h>
#include "mode4.h"
#include "print.h"
#include "game.h"
#include "player.h"
#include "klee.h"
#include "background.h"
#include "bomb.h"
#include "font.h"
#include "mode4.h"
#include "kleestand.h"
#include "kleeFire.h"
#include "kleePost.h"
#include "analogSound.h"

// Add makefile

// prototypes
void initialize();

// state prototypes
void goToStart();
void start();
void goToGame();
void game();
void goToPause();
void pause();
void goToWin();
void win();
void goToLose();
void lose();
void scoreboard();
void goToScoreboard();

unsigned short oldButtons;
unsigned short buttons;
int score = 0;
int frameCount = 0;
int highScore = 0;
int gameTimer = 0;

//text buffer
char buffer[50];

//state machine variables
enum
{
    START,
    GAME,
    PAUSE,
    WIN,
    SCOREBOARD
};
int state;


int main() {

    initialize();

    while (1) {

        oldButtons = buttons;
        buttons = REG_BUTTONS;
        //state machine
        switch (state) {
            case START:
                start();
                break;
            case GAME:
                game();
                break;
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
            case SCOREBOARD:
                scoreboard();
                break;
        }

    }

}
//initalizes the gba stuff
void initialize() {
    REG_DISPCTL = MODE(4) | BG2_ENABLE | DISP_BACKBUFFER;
    DMANow(3, backgroundPal, BG_PALETTE, DMA_DESTINATION_INCREMENT | (backgroundPalLen / 2));
    mgba_open();
    //sound stuffs
    REG_SOUNDCNT_X = SND_ENABLED;
    REG_SOUNDCNT_L = DMG_VOL_LEFT(5) |
                DMG_VOL_RIGHT(5) |
                DMG_SND1_LEFT |
                DMG_SND1_RIGHT |
                DMG_SND2_LEFT |
                DMG_SND2_RIGHT |
                DMG_SND3_LEFT |
                DMG_SND3_RIGHT |
                DMG_SND4_LEFT |
                DMG_SND4_RIGHT;
    REG_SOUNDCNT_H = DMG_MASTER_VOL(2);

    buttons = REG_BUTTONS;
    oldButtons = 0;
    initGame();
    goToStart();
}
// sets state to start
void goToStart() {
    fillScreen4(255);
    drawString4( 8, 80,"BOMBERMAN", 253);
    drawImage4(80, 80, 40, 40, kleeStandBitmap);
    waitForVBlank();
    flipPage();
    frameCount++;
    state = START;
}

//runs every frame of the start state
void start() {
    score = 0;
    if (frameCount > 300) {
        frameCount = 0;
    }
    frameCount++;
    //1st screen
    fillScreen4(255);
    drawString4( 8, 80,"BOMBERMAN", 253);
    drawImage4(100, 80, 40, 40, kleeStandBitmap);
    //next animation:
    if (frameCount > 70) {
        drawImage4(100, 80, 44, 40, kleeFireBitmap);
    }
    //last animation
    if (frameCount > 100) {
        fillScreen4(118);
        drawImage4(100, 80, 40, 40, kleePostBitmap);
        drawString4( 6, 80,"KLEE-ERMAN", 253);
    }
    waitForVBlank();
    if (frameCount % 50 == 0) {

    }
    flipPage();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToGame();
        initGame();
    }
}

// sets up the game state
void goToGame() {
    playAnalogSound(9);
    initGame();
    state = GAME;
}
// Runs every frame of the game state
void game() {
    gameTimer++;
    if (gameTimer > 300) {
        playAnalogSound(9);
        goToWin();
    }
    drawFullscreenImage4(backgroundBitmap);
    updateGame();
    drawGame();

    // update the score here
    drawString4(180, 1, "score: ", BG_PALETTE[256]);
    sprintf(buffer, "%d", score);
    drawString4(220, 1, buffer, BG_PALETTE[256]);

    waitForVBlank();
    flipPage();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    }

}

// Sets up the pause state
void goToPause() {
    fillScreen4(BG_PALETTE[255]);
    drawString4(80, 18, "PAUSED", BG_PALETTE[254]); 
    drawString4(10, 30, "PRESS SELECT TO GO TO START", BG_PALETTE[256]);
    drawString4(10, 50, "PRESS A TO SEE SCOREBOARD", BG_PALETTE[256]); 
    playAnalogSound(9); 
    
    waitForVBlank();
    flipPage();


    state = PAUSE;
}

//runs every frame of the pause state
void pause() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToGame();
    } else if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToStart();
    } else if (BUTTON_PRESSED(BUTTON_A)) {
        goToScoreboard();
    }
}

// Sets up the win state
void goToWin() {
    fillScreen4(255);
    drawString4(154, 8, "GAME COMPLETE", 253);
    drawString4(180, 1, "final score: ", 200);
    sprintf(buffer, "%d", score);
    drawString4(240, 1, buffer, 200);

    if (highScore < score) {
        highScore = score;
    } 
    gameTimer = 0;
    waitForVBlank();
    flipPage();   

    state = WIN;
}

// Runs every frame of the win state
void win() {
    fillScreen4(255);
    drawString4(80, 20, "GAME COMPLETE", 253);
    drawString4(60, 50, "final score: ", 200);
    sprintf(buffer, "%d", score);
    drawString4(140, 50, buffer, 200);
    waitForVBlank();
    flipPage();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}


// sets the scoreboard state
void goToScoreboard() {
    playAnalogSound(9);
    fillScreen4(254);
    drawString4(50, 50, "High Score: ", 200);
    sprintf(buffer, "%d", highScore);
    drawString4(120, 50, buffer, 200);
    waitForVBlank();
    flipPage();

    state = SCOREBOARD;

}
//runs every frame of scoreboard
void scoreboard() {
    fillScreen4(254);
    drawString4(50, 50, "High Score: ", 200);
    sprintf(buffer, "%d", highScore);
    drawString4(120, 50, buffer, 200);
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_A)) {
        goToPause();
    } 
}