#ifndef SELECT_SCREEN_H
#define SELECT_SCREEN_H

/* screen where players select their players*/

#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "auxiliary.h"
#include "screen_defines.h"
#include "player_defines.h"

#define N_CHOICES 4
#define N_SCENARIOS 2

// useful defines for drawings (all are porcentages)
#define SQUARE_MARGIN_Y 5  // only Y since is centralized in horizontal axis
#define BIG_SQUARE 50  // related to height
#define SMALL_SQUARE 45  // related to big square

#define SCENARIO_MARGIN_Y 60
#define SCENARIO_SIZE_FACTOR 20 // related to screen 

#define PLAYER_HEAD_SIZE 90  // size in original file
#define CHOICE_MARGIN_X 6
#define CHOICE_MARGIN_Y 10
#define CHOICE_SIZE_FACTOR 150 // related to small square

#define FONT_SIZE 80
#define FONT_MARGIN_Y 82

// joystick to select players
struct select_joystick {
    short up;
    short left;
    short down;
    short right;
    short confirm;  // confirm/change your choice
};
typedef struct select_joystick Select_Joystick;

struct select_screen {
    Pair size;
    short n_choices;  // how many choices (defined by constant)
    Select_Joystick select_p1;
    Select_Joystick select_p2;
    short start_key;  // start game when both selected
    short scenario_key;  // switch between scenarios cgoices
    Pair confirmed;  // if players have confirmed their choice
    Pair selected_choices;  // choices of p1 and p2
    ALLEGRO_BITMAP *heads[N_CHOICES+1];  // +1 because of bot
    ALLEGRO_BITMAP *background;
    ALLEGRO_BITMAP *scenario_choices[N_SCENARIOS];
    ALLEGRO_FONT *font;
    short scenario;
    short bot_key;
    bool bot_choice;
};
typedef struct select_screen Select_Screen;

// creates the screen object
Select_Screen *create_select_screen(int width, int height, short up1, short left1, short down1, short right1, short confirm1, short up2, short left2, short down2, short right2, short confirm2, short start_key, short scenario_key, short bot_key);

// updates screen based on given event
// returns type of exit, based on defines
int update_select_screen(Select_Screen *screen, unsigned int event, unsigned int key);

// draws screen
void draw_select_screen(Select_Screen *screen);

// destroys screen and frees its memory
void destroy_select_screen(Select_Screen *screen);

// gets selected choices and puts on given pair
void get_choices(Select_Screen *screen, Pair *choices, short *scenario);

#endif