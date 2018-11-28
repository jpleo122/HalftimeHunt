#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"

#define STARTING_DUCKS 20
#define STARTING_CHAR_ROW 125
#define STARTING_CHAR_COL 80
#define MIN_CHAR_ROW 121
#define STARTING_CHAR_SPEED 2
//Bullet global variables
#define NUM_BULLETS 10
#define B_WIDTH 2
#define B_HEIGHT 4
#define B_VEL 3
#define B_TIMER 60
#define B_BOUNDS 10
//Bird global variables
#define BIRD_SPAWN_LOW_BOUND 10
#define BIRD_SPAWN_HIGH_BOUND 64
#define NUM_BIRDS 15
#define BIRD_VEL 3
#define BIRD_LOW_TIMER 120
#define BIRD_HIGH_TIMER 300
#define SWITCH_TIME 60

typedef struct {
    int row;
    int col;
    int velocity;
    int wingSwitch;
    int hit;
    const u16 *wingUp;
    const u16 *wingDown;
} Bird;

//creates a struct for the character image
typedef struct {
    int row;
    int col;
    int width;
    int height;
    int speed;
    int movedRight;
    int movedDown;
    const u16 *image;
} Character;

typedef struct {
    int row;
    int col;
    int width;
    int height;
    int velocity;
    int hit;
    int isUsed;
} Bullet;

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    int ducksLeft;
    int score;
    Bullet bullets[NUM_BULLETS];
    int bulletsFront;
    int bulletsBack;
    int lastBulletFiredTimer;
    Bird birds[NUM_BIRDS];
    int birdsFront;
    int birdsBack;
    int lastBirdSpawned;
    Character* character;

    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */

} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
