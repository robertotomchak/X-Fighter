#ifndef FIGHT_SCREEN_H
#define FIGHT_SCREEN_H

/* Defines the screen where the players are fighting */

#include <stdlib.h>
#include <stdio.h>

#include "auxiliary.h"
#include "player.h"
#include "screen_status_defines.h"
#include "health_bar.h"

// health bar defines (all are %)
#define HP_BAR_MARGIN_X 10
#define HP_BAR_MARGIN_Y 10
#define HP_BAR_SIZE_X 35
#define HP_BAR_SIZE_Y 5

struct fight_screen {
    Pair size;
    short n_rounds;  // how many rounds will be played
    Pair score;  // score of p1 and p2
    Player *p1;
    Player *p2;
    int player_x_offset;  // distance from side of screen that players start;
    int gravity;
    Health_Bar *p1_hp;
    Health_Bar *p2_hp;
};
typedef struct fight_screen Fight_Screen;

// creates the screen object
Fight_Screen *create_fight_screen(int width, int height, short n_rounds, Player *p1, Player *p2, int player_x_offset, int gravity);

// updates screen based on given event
// returns type of exit, based on defines
int update_fight_screen(Fight_Screen *screen, unsigned int event, unsigned int key);

// draws screen
void draw_fight_screen(Fight_Screen *screen);

// checks if game is over
// returns -1, 0 or 1:
//  -1: p1 won most rounds
//   0: rounds not over (or tie, only possible if n_rounds is even)
//   1: p2 won most rounds
int game_over(Fight_Screen *screen);

// destroys screen and frees its memory
void destroy_fight_screen(Fight_Screen *screen);

#endif
