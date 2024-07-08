#ifndef SCREEN_STATUS_DEFINES_H
#define SCREEN_STATUS_DEFINES_H
/* Defines important status for screens transitions*/

#define QUIT -1       // quit game
#define STAY 0        // stay on screen
#define SELECTED 1    // for select screen
#define GAME_OVER 2   // when all rounds end
#define VICTORY_P1 3  // who won that round
#define VICTORY_P2 4  // who won that round
#define START 5       // when press start in start screen

// image backgrounds
#define START_BACKGROUND_PATH "assets/backgrounds/start_background.png"
#define SELECT_BACKGROUND_PATH "assets/backgrounds/selection_background.png"
#define FIGHT_ELETRICAL_PATH "assets/backgrounds/eletrical_background.png"
#define FIGHT_MOON_MATH "assets/backgrounds/moon_background.png"

// image dimensions
#define START_IMG_WIDTH 1300
#define START_IMG_HEIGHT 800

#endif