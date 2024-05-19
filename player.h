#ifndef PLAYER_H
#define PLAYER_H

/* Defines the Player of the games */
#include <stdbool.h>

#include <allegro5/allegro5.h>

#include "auxiliary.h"

#define STANDING 0
#define CROUCH 1
#define AIR 2

struct key {
    short keycode;
    unsigned char active;
};
typedef struct key Key;

struct joystick {
    Key up;
    Key left;
    Key down;
    Key right;
};
typedef struct joystick Joystick;

struct player {
    Joystick joystick;
    Pair coords;
    Pair size;
    Pair speed;
    short jump_speed;
    short health;
    short status;  // STANDING, AIR, ...
    bool face_right;
    ALLEGRO_COLOR color;
};
typedef struct player Player;

// creates a player
Player *create_player(short up, short left, short down, short right, short x, short y, short size_x, short size_y, short speed_x, short jump_speed, bool face_right, ALLEGRO_COLOR color);

// updates player based on event type and key 
// min_screen and max_screen define screen limits
void update_player(Player *p, Pair min_screen, Pair max_screen, unsigned int event, unsigned int key, unsigned short gravity);

// draws player
void draw_player(Player *p);

// kills player (probably not in a painfull way) by freeing its memory
void kill_player(Player *p);

#endif


