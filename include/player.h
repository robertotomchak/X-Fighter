#ifndef PLAYER_H
#define PLAYER_H

/* Defines the Player of the games */
#include <stdbool.h>
#include <stdlib.h>
#include "stdio.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "auxiliary.h"
#include "sprite.h"
#include "player_defines.h"

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
    short state;  // STANDING, AIR, ...
    bool face_right;
    Hit *hit_sup;
    Hit *hit_inf;
    bool hit_dmg; // a hit cannot deal damage multiple times
    short n_frames;  // number of frames in present sprite
    short sprite_status;
    Sprite *img;
    short stamina;
    short stamina_speed;  // recovering speed
};
typedef struct player Player;

// creates a hit object
Hit *create_hit(short size_x, short size_y, short offset, unsigned short damage);

// creates a player
Player *create_player(short up, short left, short down, short right, short k_hit_sup, short k_hit_inf, short size_x, short size_y, short speed_x, short jump_speed, Hit *hit_sup, Hit *hit_inf, const char *sprite_path, bool face_right, short stamina_speed);

// updates player based on event type and key 
// min_screen and max_screen define screen limits
// deals with collision with p_other
void update_player(Player *p, Pair min_screen, Pair max_screen, unsigned int event, unsigned int key, unsigned short gravity, Player *p_other, bool paused);

// draws player
void draw_player(Player *p, bool show_hitboxes);

// defines if match is over, and who won
// return -1, 0 or 1:
    // -1: p1 won
    // 0: match is not over
    // 1: p2 won
short round_over(Player *p1, Player *p2);

// resets player to given coordinates
// restores health
void reset_player(Player *p, int x, int y);

// kills player (probably not in a painfull way) by freeing its memory
void kill_player(Player *p);

#endif


