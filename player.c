/* Defines the Player of the games */
#include <stdbool.h>
#include <stdio.h>

#include "auxiliary.h"

#define STANDING 0
#define CROUCH 1
#define AIR 2

// collections of keys
struct joystick {
    short up;
    short left;
    short down;
    short right;
};
typedef struct joystick Joystick;

struct player {
    Joystick joystick;
    Pair coords;
    Pair size;
    Pair speed;
    short health;
    short status;  // STANDING, AIR, ...
};
typedef struct player Player;

// creates a player
Player *create_player(short up, short left, short down, short right, short x, short y, short size_x, short size_y, short speed_x, short jump_speed, short stamina_speed)
{
    Player *p = malloc(sizeof(Player));
    if (!p)
        return NULL;
    p->joystick.up = up;
    p->joystick.left = left;
    p->joystick.down = down;
    p->joystick.right = right;

    p->coords.x = x;
    p->coords.y = y;
    p->size.x = size_x;
    p->size.y = size_y;

    p->speed.x = speed_x;
    p->speed.y = 0;  // starts on ground
    p->status = STANDING;

    p->health = 100; // starts at full health
    p->stamina = 100;  // starts at full stamina;
    p->stamina_speed = stamina_speed;

    return p;
}

// moves player horizontally
void move_player_x(Player *p, short min_x, short max_x, bool left)
{
    if (left)
        p->x = max(min_x, p->x - p->speed.x);
    else
        p->x = min(max_x, p->x + p->speed.x);
}

// moves player vertically and updates its speed
void move_player_y(Player *p, short min_y, short max_y, short gravity)
{
    if (p->status != AIR)
        return;
    if (p->speed.y > 0)
        p->y = min(max_y, p->y + p->speed.y);
    else
        p->y = max(min_y, p->y + p->speed.y);
    p->speed.y += gravity;
}

// updates player based on event type and key 
// min_screen and max_screen define screen limits
void update_player(Player *p, Pair min_screen, Pair max_screen, unsigned int event, unsigned int key, unsigned short gravity)
{
    // if screen update, just move player vertically

    // if key related, make updates based on state machine
}

// draws player
void draw_player(Player *p);

#endif


