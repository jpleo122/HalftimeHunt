#include "logic.h"
#include "images/testSprite.h"
#include "images/fBWUpRight.h"
#include "images/fBWUpLeft.h"
#include "images/fBWDownRight.h"
#include "images/fBWDownLeft.h"

#include <stdio.h>
#include <stdlib.h>

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.

    // creates the character pointer
    Character* character = malloc(sizeof(Character));
    character->row = STARTING_CHAR_ROW;
    character->col = STARTING_CHAR_COL;
    character->width = TESTSPRITE_WIDTH;
    character->height = TESTSPRITE_HEIGHT;
    character->speed = STARTING_CHAR_SPEED;
    character->movedRight = 0;
    character->movedDown = 0;
    character->image = testSprite;
    //creates the bullets array
    // Bullet bullets[10];
    // for (int i = 0; i < 10; i++) {
    //     bullets[i] = {0, 0, 0, 0, 0, 0};
    // }

    *appState = (AppState){0, STARTING_DUCKS, 0, 
        {{0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}, {0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}, 
        {0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}, {0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}, 
        {0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}, {0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}, 
        {0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}, {0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}, 
        {0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}, {0,0,B_WIDTH,B_HEIGHT,B_VEL,0,0}},
        0, 0, 0, 
        {{0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}},
        0,0,0,
        character};
    // appState = (AppState*)malloc(sizeof(AppState));
    // appState->gameOver = 0;
    // appState->ducksLeft = 0;
    // appState->score = 0;
    // appState->character = character;
    // for (int i = 0; i < 10; i++) {
    //     appState->bullets[i] = (Bullet){0, 0, 0, 0, 0, 0};
    // }
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    AppState nextAppState = *currentAppState;

    //checks to see if the player should move sideways
    if (KEY_DOWN(BUTTON_LEFT, keysPressedNow) && !KEY_DOWN(BUTTON_RIGHT, keysPressedNow) 
        && vBlankCounter % nextAppState.character->speed == 0 
        && nextAppState.character->col + 10 > 0) {
        nextAppState.character->col -= 2;
        nextAppState.character->movedRight = -1;
    } else if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow) && !KEY_DOWN(BUTTON_LEFT, keysPressedNow) 
        && vBlankCounter % nextAppState.character->speed == 0 
        && nextAppState.character->col + nextAppState.character->width - 13< WIDTH) {
        nextAppState.character->col += 2;
        nextAppState.character->movedRight = 1;
    } else {
        nextAppState.character->movedRight = 0;
    }

    //checks to see if the player moved up or down
    if (KEY_DOWN(BUTTON_UP, keysPressedNow) && !KEY_DOWN(BUTTON_DOWN, keysPressedNow)
        && vBlankCounter % nextAppState.character->speed == 0 
        && nextAppState.character->row > MIN_CHAR_ROW) {
        nextAppState.character->row -= 1;
        nextAppState.character->movedDown = -1;
    } else if (KEY_DOWN(BUTTON_DOWN, keysPressedNow) && !KEY_DOWN(BUTTON_UP, keysPressedNow)
        && vBlankCounter % nextAppState.character->speed == 0 
        && nextAppState.character->row + nextAppState.character->height < HEIGHT) {
        nextAppState.character->row += 1;
        nextAppState.character->movedDown = 1;
    } else {
        nextAppState.character->movedDown = 0;
    }

    //Checks to see if a bullet was fired, and if it can be fired at that time
    if (KEY_DOWN(BUTTON_B, keysPressedNow) && nextAppState.lastBulletFiredTimer == 0) {
        nextAppState.bullets[nextAppState.bulletsBack % NUM_BULLETS].row = nextAppState.character->row - B_HEIGHT;
        nextAppState.bullets[nextAppState.bulletsBack % NUM_BULLETS].col = nextAppState.character->col + 12;
        nextAppState.bullets[nextAppState.bulletsBack % NUM_BULLETS].isUsed = 1;
        nextAppState.bullets[nextAppState.bulletsBack % NUM_BULLETS].hit = 0;
        nextAppState.bulletsBack++;
        nextAppState.lastBulletFiredTimer = B_TIMER;
    } else if (nextAppState.lastBulletFiredTimer > 0){
        nextAppState.lastBulletFiredTimer--;
    }

    //randomly spawns birds
    if (nextAppState.lastBirdSpawned == 0 && nextAppState.ducksLeft > 0) {
        // random digit to see if bird will go left or right
        if (randint(0,2)) { // bird will go right
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].row = randint(BIRD_SPAWN_LOW_BOUND, BIRD_SPAWN_HIGH_BOUND);
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].col = 0;
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].velocity = randint(1, 4); //how many the pixels the bird will move
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].wingSwitch = 0;
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].hit = 0;
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].wingUp = fBWUpRight;
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].wingDown = fBWDownRight;
        } else { // bird will go left
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].row = randint(BIRD_SPAWN_LOW_BOUND, BIRD_SPAWN_HIGH_BOUND);
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].col = WIDTH - FBWUPLEFT_WIDTH;
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].velocity = randint(-1 , -3); //how many pixels the bird will move
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].wingSwitch = 0;
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].hit = 0;
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].wingUp = fBWUpLeft;
            nextAppState.birds[nextAppState.birdsBack % NUM_BIRDS].wingDown = fBWDownLeft;
        }
        nextAppState.birdsBack++;
        nextAppState.lastBirdSpawned = randint(BIRD_LOW_TIMER, BIRD_HIGH_TIMER);
        // if (nextAppState.birdsBack == 2) {
        //     nextAppState.lastBirdSpawned = -1;
        // } else {
        //     nextAppState.lastBirdSpawned = randint(BIRD_LOW_TIMER, BIRD_HIGH_TIMER);
        // }
        nextAppState.ducksLeft--;
    } else if (nextAppState.lastBirdSpawned > 0) {
        nextAppState.lastBirdSpawned--;
    }

    //moves any bullets on the screen
    if (vBlankCounter % B_VEL == 0
        && nextAppState.bulletsFront != nextAppState.bulletsBack) {
        //int bounds = nextAppState.bulletsFront + NUM_BULLETS;
        for (int i = 0; i < NUM_BULLETS; i++) {
            if (nextAppState.bullets[i % NUM_BULLETS].isUsed) {
                // if (nextAppState.bullets[i % NUM_BULLETS].row - B_HEIGHT < B_BOUNDS) {
                //     nextAppState.bullets[i % NUM_BULLETS].isUsed = 0;
                //     nextAppState.bulletsFront++;
                // } else {
                //     nextAppState.bullets[i % NUM_BULLETS].row -= 4;
                // }
                nextAppState.bullets[i % NUM_BULLETS].row -= 3;
            }
        }
    }

    if (vBlankCounter % BIRD_VEL == 0
        && nextAppState.birdsFront != nextAppState.birdsBack) {
        for (int i = 0; i < NUM_BIRDS; i++) {
            if (nextAppState.birds[i % NUM_BIRDS].velocity != 0) {
                nextAppState.birds[i % NUM_BIRDS].col += nextAppState.birds[i % NUM_BIRDS].velocity;
            }
            // } else {
            //     break;
            // }
        }
    }

    if (vBlankCounter % BIRD_VEL == 0 && vBlankCounter % B_VEL == 0) {
        for (int i = 0; i < NUM_BULLETS; i++) {
            if (nextAppState.bullets[i % NUM_BULLETS].isUsed != 0) {
                for (int j = 0; j < NUM_BIRDS; j++) {
                    if (nextAppState.birds[j % NUM_BIRDS].velocity != 0) {
                        if (nextAppState.bullets[i % NUM_BULLETS].row > nextAppState.birds[j % NUM_BIRDS].row &&
                            nextAppState.bullets[i % NUM_BULLETS].row < nextAppState.birds[j % NUM_BIRDS].row + FBWUPLEFT_HEIGHT &&
                            nextAppState.bullets[i % NUM_BULLETS].col > nextAppState.birds[j % NUM_BIRDS].col &&
                            nextAppState.bullets[i % NUM_BULLETS].col < nextAppState.birds[j % NUM_BIRDS].col + FBWUPLEFT_WIDTH) {
                            nextAppState.birds[j % NUM_BIRDS].hit = 1;
                            nextAppState.bullets[i % NUM_BIRDS].hit = 1;
                            nextAppState.score += abs(nextAppState.birds[j % NUM_BIRDS].velocity);
                        }
                    }
                }
            }
        }
    }

    if (nextAppState.ducksLeft == 0 && nextAppState.birdsFront == nextAppState.birdsBack) {
        nextAppState.gameOver = 1;
    }

    UNUSED(keysPressedBefore);
    UNUSED(keysPressedNow);

    return nextAppState;
}
