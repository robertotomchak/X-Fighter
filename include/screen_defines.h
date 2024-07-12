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
#define BEGIN 6        // when opens game or goes to start screen

// image backgrounds
#define START_BACKGROUND_PATH "assets/backgrounds/start_background.png"
#define SELECT_BACKGROUND_PATH "assets/backgrounds/selection_background.png"
#define SCENARIO_ELETRICAL_PATH "assets/backgrounds/eletrical_background.png"
#define SCENARIO_MOON_PATH "assets/backgrounds/moon_background.png"

// player's heads (for select screen)
#define RED_HEAD_PATH "assets/images/red_head.png"
#define YELLOW_HEAD_PATH "assets/images/yellow_head.png"
#define BLUE_HEAD_PATH "assets/images/blue_head.png"
#define GREEN_HEAD_PATH "assets/images/green_head.png"
#define BOT_HEAD_PATH "assets/images/bot_head.png"

// baby crewmate (for points)
#define BABY_PATH "assets/images/baby.png"

// image dimensions
#define START_IMG_WIDTH 1300
#define START_IMG_HEIGHT 800
#define SELECT_IMG_WIDTH 2560
#define SELECT_IMG_HEIGHT 1440
#define ELETRICAL_IMG_WIDTH 725
#define ELETRICAL_IMG_HEIGHT 416
#define MOON_IMG_WIDTH 626
#define MOON_IMG_HEIGHT 348
#define BABY_IMG_WIDTH 481
#define BABY_IMG_HEIGHT 517

// scenarios
#define ELETRICAL_SCENARIO 0
#define MOON_SCENARIO 1

// font
#define FONT_PATH "assets/fonts/in_your_face_joffrey.ttf" 

// gravity
#define GRAVITY_ELETRICAL 2
#define GRAVITY_MOON 1

#endif