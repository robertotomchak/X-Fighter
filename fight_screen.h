#ifndef FIGHT_SCREEN_H
#define FIGHT_SCREEN_H

/* Defines the screen where the players are fighting */

#include <stdlib.h>

#include "auxiliary.h"
#include "player.h"

#define STAY 0
#define QUIT 1
#define GAME_OVER 2       // when all rounds end
#define VICTORY_P1 3  // who won that round
#define VICTORY_P2 4  // who won that round

struct fight_screen {
    Pair size;
    short n_rounds;  // how many rounds will be played
    Pair score;  // score of p1 and p2
    Player *p1;
    Player *p2;
    int player_x_offset;  // distance from side of screen that players start;
    int gravity;
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