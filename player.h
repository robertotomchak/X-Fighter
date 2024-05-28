#ifndef PLAYER_H
#define PLAYER_H

/* Defines the Player of the games */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "auxiliary.h"

#define STANDING 0
#define CROUCH 1
#define AIR 2

#define NO_HIT 0
#define SUP_HIT 1
#define INF_HIT 2

#define MAX_HEALTH 1000

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
    Key hit_sup;
    Key hit_inf;
};
typedef struct joystick Joystick;

// defines the objects for hits in the game
struct hit {
    Pair coords;
    Pair size;
    short offset; // offset from coords of player (in %, 0-100)
    unsigned short damage;
};
typedef struct hit Hit;

struct player {
    Joystick joystick;
    Pair coords;  // coordinates of bottom-left corner
    Pair size;
    Pair speed;
    short jump_speed;
    short health;
    short status;  // STANDING, AIR, ...
    bool face_right;
    Hit *hit_sup;
    Hit *hit_inf;
    short hit_status; // NO_HIT, SUP_HIT, INF_HIT, ...
    ALLEGRO_COLOR color;
};
typedef struct player Player;

// creates a hit object
Hit *create_hit(short size_x, short size_y, short offset, unsigned short damage);

// creates a player
Player *create_player(short up, short left, short down, short right, short k_hit_sup, short k_hit_inf, short x, short y, short size_x, short size_y, short speed_x, short jump_speed, Hit *hit_sup, Hit *hit_inf, ALLEGRO_COLOR color);

// updates player based on event type and key 
// min_screen and max_screen define screen limits
// deals with collision with p_other
void update_player(Player *p, Pair min_screen, Pair max_screen, unsigned int event, unsigned int key, unsigned short gravity, Player *p_other);

// draws player
void draw_player(Player *p);

// kills player (probably not in a painfull way) by freeing its memory
void kill_player(Player *p);

#endif


