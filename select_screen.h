#ifndef SELECT_SCREEN_H
#define SELECT_SCREEN_H

/* screen where players select their players*/

#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "auxiliary.h"
#include "screen_status_defines.h"

#define N_CHOICES 4

// useful defines for drawings (all are porcentages)
#define BIG_SQUARE 50  // related to height
#define SMALL_SQUARE 45  // related to big square

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
    Pair confirmed;  // if players have confirmed their choice
    Pair selected_choices;  // choices of p1 and p2
};
typedef struct select_screen Select_Screen;

// creates the screen object
Select_Screen *create_select_screen(int width, int height, short up1, short left1, short down1, short right1, short confirm1, short up2, short left2, short down2, short right2, short confirm2);

// updates screen based on given event
// returns type of exit, based on defines
int update_select_screen(Select_Screen *screen, unsigned int event, unsigned int key);

// draws screen
void draw_select_screen(Select_Screen *screen);

// destroys screen and frees its memory
void destroy_select_screen(Select_Screen *screen);

// gets selected choices and puts on given pair
void get_choices(Select_Screen *screen, Pair *choices);

#endif