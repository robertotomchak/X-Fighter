#ifndef PLAYER_DEFINES_H
#define PLAYER_DEFINES_H

/* Defines many useful player's constants, such as:
    - Sprites
    - Max Health
    - Stamina
    - States
*/

#include "stdbool.h"

#define SHOW_HITBOXES true

// speed related
#define SPEED_X 10
#define JUMP_SPEED 30

// number of frames that stays in each sprite
#define WALK_FRAMES 10
#define PREP_FRAMES 35
#define PUNCH_FRAMES 15
#define KICK_FRAMES 15

// stamina costs
#define CROUCH_STAMINA 1  // per second
#define JUMP_STAMINA 50
#define PUNCH_STAMINA 100
#define KICK_STAMINA 50
#define STAMINA_SPEED 5

// damage
#define DMG_SUP 200
#define DMG_INF 50

// health/stamina
#define MAX_HEALTH 1000
#define MAX_STAMINA 1000


// sizes
#define PLAYER_WIDTH 225
#define PLAYER_HEIGHT 300

#define OFFSET_SUP 64
#define SIZE_X_SUP 120
#define SIZE_Y_SUP 15

#define OFFSET_INF 0
#define SIZE_X_INF 35
#define SIZE_Y_INF 55



// states
#define STANDING 0
#define CROUCH 1
#define AIR 2
#define PREP 3
#define PUNCH 4
#define KICK 5

// defines of sprites of this project
#define SPRITE_WIDTH 200
#define SPRITE_HEIGHT 120
#define NUM_SPRITES 8
#define CORRECTION_RATIO 1.25  // make sprite have the correct height
#define CROUCH_SIZE 60  // % of normal size

// number of each player
#define RED_PLAYER 0
#define YELLOW_PLAYER 1
#define BLUE_PLAYER 2
#define GREEN_PLAYER 3
#define BOT_PLAYER 4

// path to sprite
#define RED_SPRITE_PATH "assets/sprites/red_sprite.png"
#define YELLOW_SPRITE_PATH "assets/sprites/yellow_sprite.png"
#define BLUE_SPRITE_PATH "assets/sprites/blue_sprite.png"
#define GREEN_SPRITE_PATH "assets/sprites/green_sprite.png"
#define BOT_SPRITE_PATH "assets/sprites/bot_sprite.png"

// name of each sprite
#define NORMAL1_SPRITE 0
#define WALK1_SPRITE 1
#define NORMAL2_SPRITE 2
#define WALK2_SPRITE 3
#define KICK_SPRITE 4
#define PREP_SPRITE 5
#define PUNCH_SPRITE 6
#define CROUCH_SPRITE 7
#define TOTAL_WALK_SPRITES 4

#endif
